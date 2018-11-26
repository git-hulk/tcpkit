/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 **/

#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <pcap/sll.h>

#include "packet.h"
#include "tcpkit.h"
#include "array.h"
#include "redis.h"
#include "logger.h"
#include "vm.h"

#define NULL_HDRLEN 4
#ifdef _IP_VHL
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#else
#define IP_HL(ip) (((ip)->ip_hl) & 0x0f)
#endif

static int port_in_target(server *srv, int dport) {
    int i;

    struct array *ports = srv->opts->ports;
    for (i = 0; i < array_used(ports); i++) {
        if (dport == *(int*)array_pos(ports, i)) {
            return i;
        }
    }
    return -1;
}

static user_packet* gen_tcp_packet(const struct timeval *tv, const struct ip *ip_packet) {
    struct tcphdr *tcphdr;
    user_packet* packet;
    unsigned int size, iphdr_size, tcphdr_size;

    packet = malloc(sizeof(*packet));
    packet->tv = tv;
    iphdr_size = IP_HL(ip_packet)*4;
    size = htons(ip_packet->ip_len);
    packet->sip = ip_packet->ip_src;
    packet->dip = ip_packet->ip_dst;
    tcphdr = (struct tcphdr *)((unsigned char *)ip_packet + iphdr_size);
#if defined(__FAVOR_BSD) || defined(__APPLE__)
    packet->seq = htonl(tcphdr->th_seq);
    packet->ack = htonl(tcphdr->th_ack);
    packet->flags = tcphdr->th_flags;
    packet->sport = ntohs(tcphdr->th_sport);
    packet->dport = ntohs(tcphdr->th_dport);
    tcphdr_size = tcphdr->th_off * 4;
#else
    packet->seq = htonl(tcphdr->seq);
    packet->ack = htonl(tcphdr->ack_seq);
    packet->flags = tcphdr->fin | (tcphdr->syn<<1) | (tcphdr->rst<<2) | (tcphdr->psh<<3);
    if (tcphdr->ack) packet->flags |= 0x10;
    packet->sport = ntohs(tcphdr->source);
    packet->dport = ntohs(tcphdr->dest);
    tcphdr_size = tcphdr->doff * 4;
#endif
    packet->payload = (char *)tcphdr + tcphdr_size;
    packet->size = size - iphdr_size - tcphdr_size;
    packet->tcp = 1;

    // update the st
    return packet;
}

static user_packet* gen_udp_packet(const struct timeval *tv, const struct ip *ip_packet) {
    user_packet *packet;
    struct udphdr *udphdr;
    int iphdr_size;

    iphdr_size = IP_HL(ip_packet)*4;
    udphdr = (struct udphdr *)((unsigned char *)ip_packet + iphdr_size);

    packet = malloc(sizeof(*packet));
    packet->tv = tv;
    packet->sip = ip_packet->ip_src;
    packet->dip = ip_packet->ip_dst;
#if defined(__FAVOR_BSD) || defined(__APPLE__)
    packet->sport = ntohs(udphdr->uh_sport);
    packet->dport = ntohs(udphdr->uh_dport);
    packet->size= ntohs(udphdr->uh_ulen);
#else
    packet->sport = ntohs(udphdr->source);
    packet->dport = ntohs(udphdr->dest);
    packet->size = ntohs(udphdr->len);
#endif
    packet->payload = (char *)udphdr + sizeof(struct udphdr);
    packet->tcp = 0;
    return packet;
}

static void push_packet_to_vm(lua_State *vm, user_packet *packet) {
    lua_getglobal(vm, "process");
    lua_newtable(vm);
    vm_push_table_int(vm, "tv_sec", packet->tv->tv_sec);
    vm_push_table_int(vm, "tv_usec", packet->tv->tv_usec);
    vm_push_table_string(vm, "sip", inet_ntoa(packet->sip));
    vm_push_table_int(vm, "sport", packet->sport);
    vm_push_table_string(vm, "dip", inet_ntoa(packet->dip));
    vm_push_table_int(vm, "dport", packet->dport);
    if (packet->tcp) {
        vm_push_table_int(vm, "seq", packet->seq);
        vm_push_table_int(vm, "ack", packet->ack);
        vm_push_table_int(vm, "flags", packet->flags);
    }
    vm_push_table_boolean(vm, "request", packet->request);
    vm_push_table_cstring(vm, "payload", packet->payload, packet->size);
    vm_push_table_int(vm, "size", packet->size);
    if (lua_pcall(vm, 1, 1, 0) != 0) {
       // log error
        alog(ERROR, "%s\n", lua_tostring(vm, -1));
    }
    lua_tonumber(vm, -1);
    lua_pop(vm, -1);
    vm_need_gc(vm);
}

static void push_packet_to_user(server *srv, user_packet *packet) {
    char t_buf[64], *type = "REQ";
    if (srv->vm) {
        push_packet_to_vm(srv->vm, packet);
        return;
    }
    if (packet->size == 0) return;

    strftime(t_buf,64,"%Y-%m-%d %H:%M:%S",localtime(&packet->tv->tv_sec));
    if (!packet->request) type = "RSP";
    if (packet->tcp) {
        rlog("%s %s:%d=>%s:%d %s %u %u %d %u %.*s",
             t_buf,
             inet_ntoa(packet->sip),
             packet->sport,
             inet_ntoa(packet->dip),
             packet->dport,
             type,
             packet->seq,
             packet->ack,
             packet->flags,
             packet->size,
             packet->size,
             packet->payload
        );
    } else {
        rlog("%s %s:%d=>%s:%d %s %u %.*s",
             t_buf,
             inet_ntoa(packet->sip),
             packet->sport,
             inet_ntoa(packet->dip),
             packet->dport,
             type,
             packet->size,
             packet->size,
             packet->payload
        );
    }
}

static int is_noreply(int mode, char *req) {
    int i, size, n;

    size = strlen(req);
    if (mode == P_REDIS) {
        return size >= 12 && !strncasecmp(req, "REPLCONF ACK", 12);
    } else if (mode == P_MEMCACHED) {
        char *noreply_keys[] = {"get", "gets", "stats", "stat", "watch", "lru", "set", "add", "incr", "decr", "delete",
            "replace", "append", "prepend", "cas", "touch", "flushall"};
        n = sizeof(noreply_keys)/sizeof(noreply_keys[0]);
        for (i = 0; i < n; i++) {
            if (!strncasecmp(req, noreply_keys[i], strlen(noreply_keys[i]))) {
                // request line didn't end with 'noreply'
                if (i >= 6 && size > 7 && !strncasecmp(&req[size-7], "noreply", 7)) {
                    return 1;
                }
                return 0;
            }
        }
        // non memcached command line, treat as noreply
        return 1;
    }
    return 0;
}

static void record_simple_latency(server *srv, user_packet *packet) {
    int latency_us;
    size_t size;
    char key[64], t_buf[64];
    char *sip, *dip, sip_buf[16], dip_buf[16];

    if (packet->size == 0) return; // ignore the syn/fin packet

    sip = inet_ntoa(packet->sip);
    size = strlen(sip);
    memcpy(sip_buf, sip, size);
    sip_buf[size] = '\0';
    dip = inet_ntoa(packet->dip);
    size = strlen(dip);
    memcpy(dip_buf, dip, size);
    dip_buf[size] = '\0';

    if (packet->request) {
        snprintf(key, 64, "%s:%d => %s:%d", sip_buf, packet->sport, dip_buf, packet->dport);
        request *req = parse_redis_request(packet->payload, packet->size);
        if (req) {
            if (is_noreply(srv->opts->mode, req->buf)) {
                free(req);
                return; // don't store the noreply request
            }
            req->tv = *packet->tv;
            if (!hashtable_add(srv->req_ht, key, req)) {
               free(req);
            }
        }
    } else {
        snprintf(key, 64, "%s:%d => %s:%d", dip_buf, packet->dport, sip_buf, packet->sport);
        request *req = hashtable_get(srv->req_ht, key);
        if (req) {
            latency_us = (packet->tv->tv_sec - req->tv.tv_sec) * 1000000 + (packet->tv->tv_usec - req->tv.tv_usec);
            int ind = port_in_target(srv, packet->sport);
            stats_update_latency(srv->st, ind, latency_us);
            if (latency_us >= srv->opts->threshold_ms*1000) {
                if (srv->opts->threshold_ms>0) stats_incr_slow_count(srv->st, ind);
                strftime(t_buf,64,"%Y-%m-%d %H:%M:%S",localtime(&packet->tv->tv_sec));
                rlog("%s.%06d %.44s | %.3f ms | %s", t_buf, packet->tv->tv_usec, key, latency_us/1000.0, req->buf);
            }
            hashtable_del(srv->req_ht, key);
        }
    }
}

static void process_tcp_packet(server *srv, user_packet *packet) {
    if (srv->opts->mode == P_RAW) {
        push_packet_to_user(srv, packet);
    } else {
        record_simple_latency(srv, packet);
    }
}

static void process_udp_packet(server *srv, user_packet *packet) {
    push_packet_to_user(srv, packet);
}

static int8_t is_request(server *srv, user_packet *packet) {
    int i;

    if (packet->sport != packet->dport) {
        return port_in_target(srv, packet->dport) != -1;
    }
    for (i = 0; i < srv->n_server; i++) {
        if (srv->servers[i].s_addr == packet->dip.s_addr) return 1;
    }
    return 0;
}

void extract_packet_handler(unsigned char *user,
                       const struct pcap_pkthdr *header,
                       const unsigned char *packet) {
    const struct ip* ip_packet;
    user_packet *upacket;

    server *srv = (server*) user;
    switch (pcap_datalink((pcap_t*)srv->sniffer)) {
        case DLT_NULL:
            ip_packet = (const struct ip *)(packet + NULL_HDRLEN); break;
        case DLT_LINUX_SLL:
            ip_packet = (const struct ip *)(packet + sizeof(struct sll_header)); break;
        case DLT_EN10MB:
            ip_packet = (const struct ip *)(packet + sizeof(struct ether_header)); break;
        case DLT_RAW:
            ip_packet = (const struct ip *)packet; break;
        default:
            return; // do nothing with unknown link type packet
    }
    if (ip_packet->ip_p != IPPROTO_TCP && ip_packet->ip_p != IPPROTO_UDP) {
        return;
    }
    switch (ip_packet->ip_p) {
        case IPPROTO_TCP:
            upacket = gen_tcp_packet(&header->ts, ip_packet);
            upacket->request = is_request(srv, upacket);
            process_tcp_packet(srv, upacket);
            stats_update_bytes(srv->st, upacket->request, upacket->size);
            free(upacket);
            break;
        case IPPROTO_UDP:
            upacket = gen_udp_packet(&header->ts, ip_packet);
            upacket->request = is_request(srv, upacket);
            process_udp_packet(srv, upacket);
            stats_update_bytes(srv->st, upacket->request, upacket->size);
            free(upacket);
            break;
        default:
            break;
    }
}
