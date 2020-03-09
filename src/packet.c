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
#include <time.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <string.h>
#include <pcap.h>

#include "log.h"
#include "lua.h"
#include "packet.h"
#include "stats.h"
#include "protocol.h"

#ifdef _IP_VHL
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#else
#define IP_HL(ip) (((ip)->ip_hl) & 0x0f)
#endif

void process_tcp_packet(struct sniffer *sniffer,
        const struct timeval tv,
        const struct ip* ip_packet,
        struct user_packet *packet) {

    struct tcphdr *tcphdr;
    unsigned int iplen, iphdr_size, tcphdr_size, framehdr_size;

    iphdr_size = IP_HL(ip_packet)*4;
    iplen = htons(ip_packet->ip_len);
    framehdr_size = packet->size - iplen;
    packet->payload_size -= framehdr_size;

    packet->tv = tv;
    packet->ip_src = ip_packet->ip_src;
    packet->ip_dst = ip_packet->ip_dst;
    tcphdr = (struct tcphdr *)((unsigned char *)ip_packet + iphdr_size);
    #if defined(__FAVOR_BSD) || defined(__APPLE__)
    packet->seq = htonl(tcphdr->th_seq);
    packet->ack = htonl(tcphdr->th_ack);
    packet->flags = tcphdr->th_flags;
    packet->port_src = ntohs(tcphdr->th_sport);
    packet->port_dst = ntohs(tcphdr->th_dport);
    tcphdr_size = tcphdr->th_off * 4;
#else
    packet->seq = htonl(tcphdr->seq);
    packet->ack = htonl(tcphdr->ack_seq);
    packet->flags = tcphdr->fin | (tcphdr->syn<<1) | (tcphdr->rst<<2) | (tcphdr->psh<<3);
    if (tcphdr->ack) packet->flags |= 0x10;
    packet->port_src = ntohs(tcphdr->source);
    packet->port_dst = ntohs(tcphdr->dest);
    tcphdr_size = tcphdr->doff * 4;
#endif
    packet->payload = (char *)tcphdr + tcphdr_size;
    if (iplen < packet->payload_size) packet->payload_size = iplen;
    if (packet->payload_size > iphdr_size + tcphdr_size) {
        packet->payload_size -= (iphdr_size + tcphdr_size);
    } else {
        packet->payload_size = 0; 
    }
    packet->is_tcp = 1;
}

void process_udp_packet(struct sniffer *sniffer,
        const struct timeval tv,
        const struct ip* ip_packet,
        struct user_packet *packet) {

    struct udphdr *udphdr;
    int iphdr_size, framehdr_size, udp_len, payload_size;
    
    iphdr_size = IP_HL(ip_packet)*4;
    udphdr = (struct udphdr *)((unsigned char *)ip_packet + iphdr_size);
    packet->tv = tv;
    packet->ip_src = ip_packet->ip_src;
    packet->ip_dst = ip_packet->ip_dst;
#if defined(__FAVOR_BSD) || defined(__APPLE__)
    packet->port_src = ntohs(udphdr->uh_sport);
    packet->port_dst = ntohs(udphdr->uh_dport);
    udp_len = ntohs(udphdr->uh_ulen);
#else
    packet->port_src = ntohs(udphdr->source);
    packet->port_dst = ntohs(udphdr->dest);
    udp_len = ntohs(udphdr->len);
#endif
    framehdr_size = packet->size - iphdr_size - udp_len;
    payload_size = packet->payload_size - (framehdr_size+iphdr_size);
    packet->payload_size = udp_len > payload_size ? payload_size : udp_len;
    packet->payload = (char *)udphdr + sizeof(struct udphdr);
    packet->is_tcp = 0;
}

static int packet_direction(struct sniffer *sniffer, struct user_packet *upacket) {
    char key[32];
    struct query_stats *stats;

    snprintf(key, sizeof(key), "%u:%d", upacket->ip_src.s_addr, upacket->port_src);
    if ((stats = hashtable_get(sniffer->syn_tab, key)) != NULL) {
        stats_incr(stats, 0, upacket->size);
        return 0;
    }
    snprintf(key, sizeof(key), "%u:%d", upacket->ip_dst.s_addr, upacket->port_dst);
    if ((stats = hashtable_get(sniffer->syn_tab, key)) != NULL)  {
        stats_incr(stats, 1, upacket->size);
        return 1;
    }
    return -1;
}

static void push_packet_to_lua_state(lua_State *state, struct user_packet *upacket) {
    lua_getglobal(state, "process");
    lua_newtable(state);
    lua_table_push_int(state, "tv_sec", upacket->tv.tv_sec);
    lua_table_push_int(state, "tv_usec", upacket->tv.tv_usec);
    lua_table_push_string(state, "sip", inet_ntoa(upacket->ip_src));
    lua_table_push_int(state, "sport", upacket->port_src);
    lua_table_push_string(state, "dip", inet_ntoa(upacket->ip_dst));
    lua_table_push_int(state, "dport", upacket->port_dst);
    if (upacket->is_tcp) {
        lua_table_push_int(state, "seq", upacket->seq);
        lua_table_push_int(state, "ack", upacket->ack);
        lua_table_push_int(state, "flags", upacket->flags);
    }
    lua_table_push_cstring(state, "payload", upacket->payload, upacket->payload_size);
    lua_table_push_int(state, "size", upacket->payload_size);
    if (lua_pcall(state, 1, 1, 0) != 0) {
        log_message(FATAL, "%s", lua_tostring(state, -1));
    }
    lua_tonumber(state, -1);
    lua_pop(state, -1);
    lua_need_gc(state);

}

static void process_request_packet(struct sniffer *sniffer, struct user_packet *upacket) {
    char key[64];
    struct request *req;

    snprintf(key, sizeof(key), "%u:%d %u:%d",
            upacket->ip_src.s_addr, upacket->port_src,
            upacket->ip_dst.s_addr, upacket->port_dst);
    if (!hashtable_get(sniffer->requests, key)) {
        req = malloc(sizeof(*req));
        req->tv = upacket->tv;
        req->seq = upacket->seq;
        req->payload = NULL;
        switch(sniffer->protocol) {
            case ProtocolRedis:
                req->payload = format_redis(upacket->payload, upacket->payload_size); break;
            case ProtocolMemcached:
                req->payload = format_memcached(upacket->payload, upacket->payload_size); break;
            case ProtocolHTTP:
                req->payload = format_http(upacket->payload, upacket->payload_size); break;
            default:
                req->payload = format_raw(upacket->payload, upacket->payload_size); break;
        }
        req->size = strlen(req->payload);
        hashtable_add(sniffer->requests, key, req);
    }
}

static void process_response_packet(struct sniffer *sniffer, struct user_packet *upacket) {
    char key[64], target[32], t_buf[64], sip_buf[64], dip_buf[64];
    struct request *req;
    int64_t delta;
    char *ip_src, *ip_dst;
    struct query_stats *stats;

    snprintf(key, sizeof(key), "%u:%d %u:%d",
            upacket->ip_dst.s_addr, upacket->port_dst,
            upacket->ip_src.s_addr, upacket->port_src);
    if ((req = hashtable_get(sniffer->requests, key)) != NULL) {
        delta = (upacket->tv.tv_sec - req->tv.tv_sec) * 1000000
            + (upacket->tv.tv_usec - req->tv.tv_usec);
        snprintf(target, sizeof(target), "%u:%d", upacket->ip_src.s_addr, upacket->port_src);
        if ((stats = hashtable_get(sniffer->syn_tab, target)) != NULL) {
            stats_observer_latency(stats, delta);
        }
        if (sniffer->threshold && delta < sniffer->threshold*1000) {
            hashtable_del(sniffer->requests, key);
            return;
        }

        strftime(t_buf, 64, "%Y-%m-%d %H:%M:%S",localtime(&upacket->tv.tv_sec));
        ip_src = inet_ntoa(upacket->ip_src);
        snprintf(sip_buf, sizeof(sip_buf), ip_src, strlen(ip_src));
        ip_dst = inet_ntoa(upacket->ip_dst);
        snprintf(dip_buf, sizeof(dip_buf), ip_dst, strlen(ip_dst));
        color_printf(GREEN, "%s.%06d %s:%d => %s:%d | %.3f ms | %s\n",
                    t_buf, upacket->tv.tv_usec,
                    dip_buf, upacket->port_dst,
                    sip_buf, upacket->port_src,
                    delta/1000.0, req->payload);
        hashtable_del(sniffer->requests, key);
    }
}

void print_user_packet(struct sniffer *sniffer, struct user_packet *upacket) {
}

void process_user_packet(struct sniffer *sniffer, struct user_packet *upacket) {
    int src;
    uint8_t syn_mask = 0x02, ack_mask = 0x10;
    char key[32];
    struct query_stats *stats;

    // push to lua state if script exists 
    if (sniffer->lua_state) {
        push_packet_to_lua_state(sniffer->lua_state, upacket);
        return;
    } else if (sniffer->protocol == ProtocolRaw) {
        print_user_packet(sniffer, upacket);
        return;
    }
    if (upacket->payload_size == 0) {
        if ((upacket->flags & syn_mask) != 0) {
            src = (upacket->flags & ack_mask) != 0; 
            if (src) {
                snprintf(key, sizeof(key), "%u:%d", upacket->ip_src.s_addr, upacket->port_src);
            } else {
                snprintf(key, sizeof(key), "%u:%d", upacket->ip_dst.s_addr, upacket->port_dst);
            }
            if (!hashtable_get(sniffer->syn_tab, key)) {
                stats = calloc(1, sizeof(*stats));
                stats->ip = src ? upacket->ip_src : upacket->ip_dst;
                stats->port = src ? upacket->port_src : upacket->port_dst;
                hashtable_add(sniffer->syn_tab, key, stats);
            }
        }
    } else {
        switch(packet_direction(sniffer, upacket)) {
            case 0:
                return process_response_packet(sniffer, upacket);
            case 1:
                return process_request_packet(sniffer, upacket);
        }
    }
}
