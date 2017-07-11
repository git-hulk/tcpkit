#ifndef _PACKET_H_
#define _PACKET_H_

#include "tcpkit.h"
#include <pcap/sll.h>
 #include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

struct tcp_packet {
    const struct timeval *tv;
    // IP
    struct in_addr src; // source ip
    struct in_addr dst; // destination ip
    // TCP
    uint8_t flags;
    uint16_t sport; // source port
    uint16_t dport; // destination port
    unsigned int seq;
    unsigned int ack;

    unsigned int payload_size;
    char *payload;
};

struct udp_packet {
    const struct timeval *tv;
    // IP
    struct in_addr src; // source ip
    struct in_addr dst; // destination ip
    // UDP
    uint16_t sport; // source port
    uint16_t dport; // destination port

    unsigned int payload_size;
    char *payload;
};

void process_packet(unsigned char *user, const struct pcap_pkthdr *header, const unsigned char *packet);
#endif
