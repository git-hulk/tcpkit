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
#include <stdarg.h>
#include <stdio.h>
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

int process_tcp_packet(const struct timeval tv,
        const struct ip* ip_packet,
        int caplen,
        int wirelen,
        struct user_packet *packet) {

    const struct tcphdr *tcphdr;
    int iphdr_size, tcphdr_size, iplen, captured;

    iphdr_size = IP_HL(ip_packet)*4;
    if (iphdr_size < (int)sizeof(struct ip)) return -1;
    if (caplen < iphdr_size + (int)sizeof(struct tcphdr)) return -1;

    tcphdr = (const struct tcphdr *)((const unsigned char *)ip_packet + iphdr_size);
    packet->tv = tv;
    packet->ip_src = ip_packet->ip_src;
    packet->ip_dst = ip_packet->ip_dst;
#if defined(__FAVOR_BSD) || defined(__APPLE__)
    packet->seq = ntohl(tcphdr->th_seq);
    packet->ack = ntohl(tcphdr->th_ack);
    packet->flags = tcphdr->th_flags;
    packet->port_src = ntohs(tcphdr->th_sport);
    packet->port_dst = ntohs(tcphdr->th_dport);
    packet->window = ntohs(tcphdr->th_win);
    tcphdr_size = tcphdr->th_off * 4;
#else
    packet->seq = ntohl(tcphdr->seq);
    packet->ack = ntohl(tcphdr->ack_seq);
    packet->flags = tcphdr->fin | (tcphdr->syn<<1) | (tcphdr->rst<<2) | (tcphdr->psh<<3);
    if (tcphdr->ack) packet->flags |= 0x10;
    packet->port_src = ntohs(tcphdr->source);
    packet->port_dst = ntohs(tcphdr->dest);
    packet->window = ntohs(tcphdr->window);
    tcphdr_size = tcphdr->doff * 4;
#endif
    if (tcphdr_size < (int)sizeof(struct tcphdr)) return -1;
    if (caplen < iphdr_size + tcphdr_size) return -1;

    /* ip_len covers the IP header and everything after it, but a malformed or
     * truncated packet can claim more than was captured. */
    iplen = ntohs(ip_packet->ip_len);
    if (iplen < iphdr_size + tcphdr_size || iplen > wirelen) return -1;

    packet->wire_payload_size = iplen - iphdr_size - tcphdr_size;
    captured = caplen - iphdr_size - tcphdr_size;
    packet->payload = (const char *)tcphdr + tcphdr_size;
    packet->payload_size = packet->wire_payload_size < captured
        ? packet->wire_payload_size : captured;
    packet->is_tcp = 1;
    return 0;
}

int process_udp_packet(const struct timeval tv,
        const struct ip* ip_packet,
        int caplen,
        int wirelen,
        struct user_packet *packet) {

    const struct udphdr *udphdr;
    int iphdr_size, udp_len, captured;

    iphdr_size = IP_HL(ip_packet)*4;
    if (iphdr_size < (int)sizeof(struct ip)) return -1;
    if (caplen < iphdr_size + (int)sizeof(struct udphdr)) return -1;

    udphdr = (const struct udphdr *)((const unsigned char *)ip_packet + iphdr_size);
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
    /* uh_ulen counts the udp header too. */
    if (udp_len < (int)sizeof(struct udphdr)) return -1;
    if (iphdr_size + udp_len > wirelen) return -1;

    packet->wire_payload_size = udp_len - (int)sizeof(struct udphdr);
    captured = caplen - iphdr_size - (int)sizeof(struct udphdr);
    packet->payload = (const char *)udphdr + sizeof(struct udphdr);
    packet->payload_size = packet->wire_payload_size < captured
        ? packet->wire_payload_size : captured;
    packet->is_tcp = 0;
    return 0;
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
        lua_table_push_int(state, "window", upacket->window);
    }
    lua_table_push_cstring(state, "payload", upacket->payload, upacket->payload_size);
    lua_table_push_int(state, "size", upacket->payload_size);
    if (lua_pcall(state, 1, 1, 0) != 0) {
        log_message(FATAL, "%s", lua_tostring(state, -1));
    }
    lua_tonumber(state, -1);
    lua_pop(state, 1);
    lua_need_gc(state);

}

static void process_request_packet(struct sniffer *sniffer, struct user_packet *upacket) {
    char key[64];
    struct request *req;

    snprintf(key, sizeof(key), "%u:%d %u:%d",
            upacket->ip_src.s_addr, upacket->port_src,
            upacket->ip_dst.s_addr, upacket->port_dst);
    if (hashtable_get(sniffer->requests, key)) return;

    req = malloc(sizeof(*req));
    if (!req) return;
    req->tv = upacket->tv;
    req->seq = upacket->seq;
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
    if (!req->payload) {
        free(req);
        return;
    }
    req->size = strlen(req->payload);
    hashtable_add(sniffer->requests, key, req);
}

static void process_response_packet(struct sniffer *sniffer, struct user_packet *upacket) {
    char key[64], target[32], t_buf[64], sip_buf[64], dip_buf[64];
    struct request *req;
    int64_t delta;
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

        strftime(t_buf, sizeof(t_buf), "%Y-%m-%d %H:%M:%S",localtime(&upacket->tv.tv_sec));
        /* inet_ntoa returns a static buffer, so both ends need their own copy. */
        snprintf(sip_buf, sizeof(sip_buf), "%s", inet_ntoa(upacket->ip_src));
        snprintf(dip_buf, sizeof(dip_buf), "%s", inet_ntoa(upacket->ip_dst));
        color_printf(GREEN, "%s.%06d %s:%d => %s:%d | %.3f ms | %s\n",
                    t_buf, upacket->tv.tv_usec,
                    dip_buf, upacket->port_dst,
                    sip_buf, upacket->port_src,
                    delta/1000.0, req->payload);
        hashtable_del(sniffer->requests, key);
    }
}

static void append_fmt(char *buf, int cap, int *n, const char *fmt, ...) {
    va_list ap;
    int written;

    if (*n >= cap - 1) return;
    va_start(ap, fmt);
    written = vsnprintf(buf + *n, cap - *n, fmt, ap);
    va_end(ap);
    if (written < 0) return;
    /* vsnprintf reports what it would have written, so clamp to the buffer. */
    *n = *n + written > cap - 1 ? cap - 1 : *n + written;
}

void print_user_packet(struct sniffer *sniffer, struct user_packet *upacket) {
    int n = 0, length;
    char buf[512], t_buf[32];

    length = upacket->wire_payload_size;
    strftime(t_buf, sizeof(t_buf), "%H:%M:%S", localtime(&upacket->tv.tv_sec));
    append_fmt(buf, sizeof(buf), &n, "%s", t_buf);
    append_fmt(buf, sizeof(buf), &n, ".%06d", upacket->tv.tv_usec);
    append_fmt(buf, sizeof(buf), &n, " IP %s.%d",
            inet_ntoa(upacket->ip_src), upacket->port_src);
    append_fmt(buf, sizeof(buf), &n, " > %s.%d:",
            inet_ntoa(upacket->ip_dst), upacket->port_dst);
    if (upacket->is_tcp) {
        append_fmt(buf, sizeof(buf), &n, " Flags [");
        if (upacket->flags & 0x01) append_fmt(buf, sizeof(buf), &n, "F");
        if (upacket->flags & 0x02) append_fmt(buf, sizeof(buf), &n, "S");
        if (upacket->flags & 0x04) append_fmt(buf, sizeof(buf), &n, "R");
        if (upacket->flags & 0x08) append_fmt(buf, sizeof(buf), &n, "P");
        if (upacket->flags & 0x10) append_fmt(buf, sizeof(buf), &n, ".");
        append_fmt(buf, sizeof(buf), &n, "],");
        if (length > 0) {
            append_fmt(buf, sizeof(buf), &n, " seq %u:%u,",
                    upacket->seq, upacket->seq + length);
        }
        if (upacket->flags & 0x10) {
            append_fmt(buf, sizeof(buf), &n, " ack %u,", upacket->ack);
        }
        append_fmt(buf, sizeof(buf), &n, " window %d,", upacket->window);
    }
    append_fmt(buf, sizeof(buf), &n, " length %d", length);
    if (!sniffer->ascii) {
        printf("%s\n", buf);
    } else {
        printf("%s %.*s\n", buf, upacket->payload_size, upacket->payload);
    }
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
