#include <stdlib.h>
#include <string.h>

#include "packet.h"
#include "stats.h"
#include "tcpkit.h"
#include "util.h"
#include "local_addresses.h"

#define NULL_HDRLEN 4
#ifdef _IP_VHL
    #define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#else
    #define IP_HL(ip) (((ip)->ip_hl) & 0x0f)
#endif


static int
is_client_mode() {
    struct options *opts = get_options();
    return opts->server != NULL;
}

// return type for every packet
// 1 for incoming
// 0 for outgoing
static int
get_packet_direct(struct in_addr src, int sport, struct in_addr dst, int dport) {
	struct options *opts = get_options();
    if (is_local_address(dst) && is_local_address(src)) {
        return is_client_mode() ? sport == opts->port : dport == opts->port;
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
    size = htons(ip->ip_len);
    packet->src = ip->ip_src;
    packet->dst = ip->ip_dst;
    tcphdr = (struct tcphdr *)((unsigned char *)ip + iphdr_size);
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
    packet->direct = get_packet_direct(packet->src, packet->sport, packet->dst, packet->dport);
    return packet;
}

static void
push_tcp_packet(struct tcp_packet *packet) {
    lua_State *vm = get_lua_vm();
    lua_newtable(vm);
    script_pushtableinteger(vm, "tv_sec", packet->tv->tv_sec);
    script_pushtableinteger(vm, "tv_usec", packet->tv->tv_usec);
    script_pushtableinteger(vm, "incoming", packet->direct);
    script_pushtableinteger(vm, "len" , packet->payload_size);
    script_pushtablestring(vm,  "src", inet_ntoa(packet->src));
    script_pushtablestring(vm,  "dst", inet_ntoa(packet->dst));
    script_pushtableinteger(vm, "sport", packet->sport);
    script_pushtableinteger(vm, "dport", packet->dport);
    script_pushtableinteger(vm, "seq", packet->seq);
    script_pushtableinteger(vm, "ack", packet->ack);
    script_pushtableinteger(vm, "flags", packet->flags);
    script_pushtableinteger(vm, "is_client", is_client_mode());
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
    packet->direct = get_packet_direct(packet->src, packet->sport, packet->dst, packet->dport);
    return packet;
}

static void
push_udp_packet(const struct udp_packet *packet) {
    lua_State *vm = get_lua_vm();
    lua_newtable(vm);
    script_pushtableinteger(vm, "tv_sec",  packet->tv->tv_sec);
    script_pushtableinteger(vm, "tv_usec", packet->tv->tv_usec);
    script_pushtableinteger(vm, "len", packet->payload_size);
    script_pushtablestring(vm,  "src", inet_ntoa(packet->src));
    script_pushtablestring(vm,  "dst", inet_ntoa(packet->dst));
    script_pushtableinteger(vm, "sport", packet->sport);
    script_pushtableinteger(vm, "dport", packet->dport);
    script_pushtableinteger(vm, "incoming", packet->direct);
    script_pushtableinteger(vm, "udp", 1);
    if (packet->payload_size > 0) {
        script_pushtablelstring(vm, "payload", packet->payload, packet->payload_size);
    }
}

static void
push_packet_to_vm(void *packet, int tcp) {
    lua_State *vm;
    vm = get_lua_vm();
    lua_getglobal(vm, DEFAULT_CALLBACK);
    if (tcp) {
        push_tcp_packet((struct tcp_packet*)packet);
    } else {
        push_udp_packet((struct udp_packet*)packet);
    }
    free(packet);
    if (lua_pcall(vm, 1, 1, 0) != 0) {
        logger(ERROR, "%s", lua_tostring(vm, -1));
    }
    script_need_gc(vm);
    lua_tonumber(vm, -1);
    lua_pop(vm, -1);
}

static void
push_packet_to_user(const struct ip *ip, const struct timeval *tv) {
    struct tcp_packet *tp;
    struct udp_packet *up;
    struct stats *stats = get_stats();
    //why add 18, Dst Mac(6)+Src Mac(6)+Length(2)+Fcs(4)
    int size = htons(ip->ip_len) + 18;
    switch (ip->ip_p) {
        case IPPROTO_TCP:
            tp = create_tcp_packet(ip, tv);
            update_stats(stats, tp->direct, size);
            push_packet_to_vm(tp, 1);
            break;
        case IPPROTO_UDP:
            up = create_udp_packet(ip, tv);
            update_stats(stats, up->direct, size);
            push_packet_to_vm(up, 0);
            break;
    }
}

const struct ip*
extract_ip_packet(int link_type, const unsigned char *packet) {
    switch (link_type) {
    case DLT_NULL:
         return (struct ip *)(packet + NULL_HDRLEN); // BSD loopback
    case DLT_LINUX_SLL:
        return (const struct ip *)(packet + sizeof(struct sll_header));
    case DLT_EN10MB:
        return (const struct ip *)(packet + sizeof(struct ether_header));
    case DLT_RAW:
        return (const struct ip *)packet;
     default:
        return NULL; 
    }
}

void
stats_packet_handler(unsigned char *user, const struct pcap_pkthdr *header,
                const unsigned char *packet) {
    int size, link_type;
    struct tcp_packet *tp;
    struct udp_packet *up;
    const struct ip *ip_packet;
    struct stats *stats = get_stats();

    link_type= pcap_datalink((pcap_t*)user);
    ip_packet = extract_ip_packet(link_type, packet);
    size = htons(ip_packet->ip_len) + 18;
    switch (ip_packet->ip_p) {
        case IPPROTO_TCP:
            tp = create_tcp_packet(ip_packet, &header->ts);
            update_stats(stats, tp->direct, size);
            free(tp);
            break;
        case IPPROTO_UDP:
            up = create_udp_packet(ip_packet, &header->ts);
            update_stats(stats, up->direct, size);
            free(up);
            break;
    }
}

void
analyze_packet_handler(unsigned char *user, const struct pcap_pkthdr *header,
                const unsigned char *packet) {
    int link_type;
    const struct ip *ip_packet;

    link_type= pcap_datalink((pcap_t*)user);
    ip_packet = extract_ip_packet(link_type, packet);
    push_packet_to_user(ip_packet, &header->ts); 
}
