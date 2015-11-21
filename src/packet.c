#include "packet.h"
#include "tcpkit.h"
#include "cJSON.h"
#include "local_addresses.h"
#include <stdlib.h>
#include <string.h>

lua_State *L;

void
process_packet(unsigned char *user, const struct pcap_pkthdr *header,
                const unsigned char *packet) {
    const struct sll_header *sll;
    const struct ether_header *ether_header;
    const struct ip *ip;
    unsigned short packet_type;
    pcap_wrapper *pw;

    pw  = (pcap_wrapper *) user;

    switch (pcap_datalink(pw->pcap)) {
    
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
        packet_type = ETHERTYPE_IP; //This is raw ip
        ip = (const struct ip *) packet;

        break;

     default: return; 
    }
    
    process_ip_packet(ip, header->ts); 
}


char *ip_to_json(const struct ip *ip, unsigned dlen,  struct timeval tv)
{
    int off, tcp_hdr_size, direct = 0;
    char buf[64];
    struct tcphdr *tcp;
    uint16_t sport, dport;
    char *json_str, *addr;

    off = strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S.",localtime(&tv.tv_sec)); 
    snprintf(buf+off,sizeof(buf)-off,"%03d",(int)tv.tv_usec/1000);


    tcp = (struct tcphdr *) ((unsigned char *) ip + sizeof(struct ip));
#if defined(__FAVOR_BSD) || defined(__APPLE__)
        sport = ntohs(tcp->th_sport);
        dport = ntohs(tcp->th_dport);
        tcp_hdr_size = tcp->th_off * 4;
#else
        sport = ntohs(tcp->source);
        dport = ntohs(tcp->dest);
        tcp_hdr_size = tcp->doff * 4;
#endif
        if (is_local_address(ip->ip_dst)) {
            // 0 means incoming packet
            direct = 1;
        }

        cJSON *cjson = cJSON_CreateObject();

        cJSON_AddStringToObject(cjson, "timestamp", buf);
        addr = inet_ntoa(ip->ip_src);
        cJSON_AddStringToObject(cjson, "src", addr);
        addr = inet_ntoa(ip->ip_dst);
        cJSON_AddStringToObject(cjson, "dst", addr);
        cJSON_AddNumberToObject(cjson, "sport" ,sport);
        cJSON_AddNumberToObject(cjson, "dport" ,dport);
        cJSON_AddNumberToObject(cjson, "len" , dlen);
        cJSON_AddNumberToObject(cjson, "direct" ,direct);
        if (dlen > 0) {
            // -----------+-----------+----------+-----....-----+
            // | ETHER    |  IP       | TCP      | payload      |
            // -----------+-----------+----------+--------------+
            cJSON_AddLStringToObject(cjson, "payload", (char *)tcp + tcp_hdr_size, dlen);
        }

        json_str = cJSON_Print(cjson);
        cJSON_Delete(cjson);

        return json_str; 
}

int
process_ip_packet(const struct ip *ip, struct timeval tv) {
    switch (ip->ip_p) {
        struct tcphdr *tcp;
        unsigned len, datalen; 
        char *json_str;

    case IPPROTO_TCP:
        
        tcp = (struct tcphdr *) ((unsigned char *) ip + sizeof(struct ip));
        len = htons(ip->ip_len);
#if defined(__FAVOR_BSD) || defined(__APPLE__)
        datalen = len - sizeof(struct ip) - tcp->th_off * 4;    // 4 bits offset 
#else
        datalen = len - sizeof(struct ip) - tcp->doff * 4;
#endif
        // ignore tcp flow packet
        if(datalen == 0) break;

        // lua process handler
        lua_getglobal(L, DEFAULT_CALLBACK);
        json_str = ip_to_json(ip, datalen, tv);    
        lua_pushstring(L, json_str);
        lua_pcall(L, 1, 1, 0);
        lua_tonumber(L, -1);
        lua_pop(L,-1);
        free(json_str);

        break;
    default: return 0;
    }

    return 0;
}
