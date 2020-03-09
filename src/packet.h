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

#ifndef TCPKIT_PACKET_H
#define TCPKIT_PACKET_H

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

#include "sniffer.h"

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
    int payload_size;
    int hdr_size;
    const char *payload;
};

void process_tcp_packet(struct sniffer *sniffer,
        const struct timeval tv,
        const struct ip* ip_packet,
        struct user_packet *packet);

void process_udp_packet(struct sniffer *sniffer,
        const struct timeval tv,
        const struct ip* ip_packet,
        struct user_packet *packet);

void process_user_packet(struct sniffer *sniffer, struct user_packet *upacket); 
#endif
