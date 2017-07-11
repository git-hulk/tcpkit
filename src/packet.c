#include "packet.h"
#include "tcpkit.h"
#include "util.h"
#include "local_addresses.h"
#include <stdlib.h>
#include <string.h>
#include "bandwidth.h"

#define NULL_HDRLEN 4

#ifdef _IP_VHL
    #define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#else
    #define IP_HL(ip) (((ip)->ip_hl) & 0x0f)
#endif


static int
is_client_mode(struct options *opts) {
    return opts->server != NULL;
}

// return type for every packet
// 1 for incoming
// 0 for outgoing
static int
get_packet_type(struct in_addr src, int sport, struct in_addr dst, int dport) {
	struct options *opts = get_options();
    if (is_local_address(dst) && is_local_address(src)) {
        return is_client_mode(opts) ? sport == opts->port : dport == opts->port;
    }
    return is_local_address(dst);
}

static struct tcp_packet *
create_tcp_packet(const struct ip *ip, const struct timeval *tv) {
    struct tcphdr *tcphdr;
    struct tcp_packet *packet;
    unsigned int size, iphdr_size, tcphdr_size;

    packet = malloc(sizeof(*packet));
    packet->tv = tv;
    iphdr_size = IP_HL(ip)*4;
    tcphdr = (struct tcphdr *)((unsigned char *)ip + iphdr_size);
    size = htons(ip->ip_len);
    packet->src = ip->ip_src;
    packet->dst = ip->ip_dst;
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
    packet->payload_size = size - iphdr_size - tcphdr_size;
    packet->payload = (char *)tcphdr + tcphdr_size;
    return packet;
}

static void
push_params(struct tcp_packet *packet) {
    int incoming;
    lua_State *vm;
    struct options *opts;

    opts = get_options();
    incoming = get_packet_type(packet->src, packet->sport, packet->dst, packet->dport);
    vm = get_lua_vm();
    lua_newtable(vm);
    script_pushtableinteger(vm, "tv_sec", packet->tv->tv_sec);
    script_pushtableinteger(vm, "tv_usec", packet->tv->tv_usec);
    script_pushtableinteger(vm, "incoming", incoming);
    script_pushtableinteger(vm, "len" , packet->payload_size);
    script_pushtablestring(vm,  "src", inet_ntoa(packet->src));
    script_pushtablestring(vm,  "dst", inet_ntoa(packet->dst));
    script_pushtableinteger(vm, "sport", packet->sport);
    script_pushtableinteger(vm, "dport", packet->dport);
    script_pushtableinteger(vm, "seq", packet->seq);
    script_pushtableinteger(vm, "ack", packet->ack);
    script_pushtableinteger(vm, "flags", packet->flags);
    script_pushtableinteger(vm, "is_client", is_client_mode(opts));
    script_pushtableinteger(vm, "udp", 0);
    if (packet->payload_size > 0) {
        script_pushtablelstring(vm, "payload", packet->payload, packet->payload_size);
    }
}

static struct udp_packet *
create_udp_packet(const struct ip *ip, const struct timeval *tv) {
    struct udp_packet *packet;
    struct udphdr *udphdr;
    int iphdr_size;

    iphdr_size = IP_HL(ip)*4;
    udphdr = (struct udphdr *)((unsigned char *)ip + iphdr_size);

    packet = malloc(sizeof(*packet));
    packet->tv = tv;
    packet->src = ip->ip_src;
    packet->dst = ip->ip_dst;
#if defined(__FAVOR_BSD) || defined(__APPLE__)
    packet->sport = ntohs(udphdr->uh_sport);
    packet->dport = ntohs(udphdr->uh_dport);
    packet->payload_size = ntohs(udphdr->uh_ulen);
#else
    packet->sport = ntohs(udphdr->source);
    packet->dport = ntohs(udphdr->dest);
    packet->payload_size = ntohs(udphdr->len);
#endif
    packet->payload = (char *)udphdr + sizeof(struct udphdr);
    return packet;
}

// handle udp packet
static void udp_packet_callback(const struct udp_packet *packet) {
    int incoming;
    lua_State *vm;

    vm = get_lua_vm();
    lua_getglobal(vm, DEFAULT_CALLBACK);
    lua_newtable(vm);
    script_pushtableinteger(vm, "tv_sec",  packet->tv->tv_sec);
    script_pushtableinteger(vm, "tv_usec", packet->tv->tv_usec);
    script_pushtableinteger(vm, "len", packet->payload_size);
    script_pushtablestring(vm,  "src", inet_ntoa(packet->src));
    script_pushtablestring(vm,  "dst", inet_ntoa(packet->dst));
    script_pushtableinteger(vm, "sport", packet->sport);
    script_pushtableinteger(vm, "dport", packet->dport);
    incoming = get_packet_type(packet->src, packet->sport, packet->dst, packet->dport);
    script_pushtableinteger(vm, "incoming", incoming);
    script_pushtableinteger(vm, "udp", 1);
    if (packet->payload_size > 0) {
        // udp header always = 8 bytes
        script_pushtablelstring(vm, "payload", packet->payload, packet->payload_size);
    }
    if (lua_pcall(vm, 1, 1, 0) != 0) {
        logger(ERROR, "%s", lua_tostring(vm, -1));
    }
    script_need_gc(vm);
    lua_tonumber(vm, -1);
    lua_pop(vm,-1);
}

static void
tcp_packet_callback(struct tcp_packet *packet) {
    lua_State *vm;
    vm = get_lua_vm();
    lua_getglobal(vm, DEFAULT_CALLBACK);
    push_params(packet);
    if (lua_pcall(vm, 1, 1, 0) != 0) {
        logger(ERROR, "%s", lua_tostring(vm, -1));
    }
    script_need_gc(vm);
    lua_tonumber(vm, -1);
    lua_pop(vm, -1);
}

static void
process_ip_packet(const struct ip *ip, const struct timeval *tv) {
    switch (ip->ip_p) {
        case IPPROTO_TCP:
            tcp_packet_callback(create_tcp_packet(ip, tv));
            break;
        case IPPROTO_UDP:
            udp_packet_callback(create_udp_packet(ip, tv));
            break;
    }
}

void
process_packet(unsigned char *user, const struct pcap_pkthdr *header,
                const unsigned char *packet) {
    const struct ip *ip;
    unsigned short packet_type;
    const struct sll_header *sll;
    const struct ether_header *ether_header;

    switch (pcap_datalink(((pcap_wrapper *)user)->pcap)) {
    case DLT_NULL:
        ip = (struct ip *)(packet + NULL_HDRLEN); // BSD loopback
        break;
    case DLT_LINUX_SLL:
        sll = (struct sll_header *) packet;
        packet_type = ntohs(sll->sll_protocol);
        ip = (const struct ip *) (packet + sizeof(struct sll_header));
        break;
    case DLT_EN10MB:
        ether_header = (struct ether_header *) packet;
        packet_type = ntohs(ether_header->ether_type);
        ip = (const struct ip *) (packet + sizeof(struct ether_header));
        break;
    case DLT_RAW:
        packet_type = ETHERTYPE_IP; //Raw ip
        ip = (const struct ip *) packet;
        break;
     default: return; 
    }
    
    // prevent warning
    packet_type = 0;
    process_ip_packet(ip, &header->ts); 
}
