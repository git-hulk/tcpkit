/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   SPDX-License-Identifier: MIT
 *
 *   Use of this source code is governed by the MIT license that can be found
 *   in the LICENSE file at the root of this repository.
 *
 **/

#ifndef TCPKIT_PACKET_H
#define TCPKIT_PACKET_H

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

#include "sniffer.h"

#ifdef _IP_VHL
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)
#else
#define IP_HL(ip) (((ip)->ip_hl) & 0x0f)
#define IP_V(ip) ((ip)->ip_v)
#endif

struct user_packet {
    uint8_t flags;
    int8_t is_tcp;
    int8_t is_request;
    struct timeval tv;
    struct in_addr ip_src;
    struct in_addr ip_dst;
    uint16_t port_src;
    uint16_t port_dst;
    uint16_t window;
    unsigned int seq;
    unsigned int ack;
    bpf_u_int32 size;
    /* Payload bytes that were actually captured, so safe to read. */
    int payload_size;
    /* Payload bytes the headers account for, which a short snaplen truncates. */
    int wire_payload_size;
    const char *payload;
};

/* Both return -1 when the headers promise more than the frame can hold.
 * caplen counts the bytes captured from ip_packet onwards, wirelen the bytes
 * that were on the wire; a short snaplen makes caplen the smaller of the two. */
int process_tcp_packet(const struct timeval tv,
        const struct ip* ip_packet,
        int caplen,
        int wirelen,
        struct user_packet *packet);

int process_udp_packet(const struct timeval tv,
        const struct ip* ip_packet,
        int caplen,
        int wirelen,
        struct user_packet *packet);

void process_user_packet(struct sniffer *sniffer, struct user_packet *upacket); 
#endif
