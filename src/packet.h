#ifndef _PACKET_H_
#define _PACKET_H_

#include "tcpkit.h"
#include <pcap/sll.h>
 #include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>

void process_packet(unsigned char *user, const struct pcap_pkthdr *header, const unsigned char *packet);
#endif
