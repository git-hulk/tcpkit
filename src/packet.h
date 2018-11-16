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

#include <time.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pcap/pcap.h>

typedef struct {
    const struct timeval *tv;
    struct in_addr sip;
    struct in_addr dip;
    uint16_t sport;
    uint16_t dport;
    uint8_t flags;
    unsigned int seq;
    unsigned int ack;
    char *payload;
    unsigned int size;
    int request;
} tcp_packet;

typedef struct {
    const struct timeval *tv;
    struct in_addr sip;
    struct in_addr dip;
    uint16_t sport;
    uint16_t dport;
    char *payload;
    unsigned int size;
    int request;
} udp_packet;

void free_tcp_req_packet(void *ptr);
void extract_packet_handler(unsigned char *user,
                            const struct pcap_pkthdr *header,
                            const unsigned char *packet);
#endif //TCPKIT_PACKET_H
