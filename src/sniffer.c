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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <pcap/sll.h>

#include "lua.h"
#include "packet.h"
#include "sniffer.h"

#define NULL_HDRLEN 4

static void free_stats(void *v) {
    struct query_stats *stats;

    if (!v) return;
    stats = (struct query_stats *)v;
    free(stats);
}

static void free_request(void *v) {
    struct request *req;

    if (!v) return;
    req  = (struct request *)v;
    if (req->payload) free(req->payload);
    free(req);
}

struct bpf_program *sniffer_compile(pcap_t *pcap, const char *filter, char *err) {
    struct bpf_program *bpf;
   
    bpf = malloc(sizeof(*bpf)); 
    if (pcap_compile(pcap, bpf, filter, 0, 1) != 0) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "%s", pcap_geterr(pcap));
        free(bpf);
        return NULL;
    }
    if (pcap_setfilter(pcap, bpf) != 0) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "%s", pcap_geterr(pcap));
        pcap_freecode(bpf);
        free(bpf);
        return NULL;
    }
    return bpf;
}

struct sniffer *sniffer_create(struct options *opts, char *err) {
    pcap_t *pcap;
    struct sniffer *sniffer;
    char *dev;
    lua_State *lua_state = NULL;
    struct bpf_program *bpf;

    sniffer = malloc(sizeof(*sniffer));
    sniffer->protocol = opts->protocol;
    sniffer->threshold = opts->threshold;
    sniffer->ascii = opts->ascii;
    sniffer->dev = strdup(opts->dev);
    sniffer->filter = strdup(opts->filter);
    sniffer->syn_tab = hashtable_create(16);
    sniffer->syn_tab->free = free_stats;
    sniffer->requests = hashtable_create(102400);
    sniffer->requests->free = free_request;
    sniffer->bpf = NULL;
    sniffer->lua_state = NULL;
    sniffer->pcap = NULL;

    if (opts->offline_file) {
        pcap = sniffer_offline(opts->offline_file, err);
    } else {
        pcap = sniffer_online(opts->dev, opts->snaplen, opts->buf_size, err);
        if (!strcmp(opts->dev, "any") && !pcap) {
            if ((dev = pcap_lookupdev(err)) != NULL) {
                pcap = sniffer_online(dev, opts->snaplen, opts->buf_size, err);
                free(opts->dev);
                opts->dev = strdup(dev);
            }
        }
    }
    if (!pcap) goto error; 
    sniffer->pcap = pcap;

    if (!sniffer->filter) {
        bpf = sniffer_compile(sniffer->pcap, sniffer->filter, err);
        if (!bpf) goto error;
        sniffer->bpf = bpf;
    }

    if (opts->script) {
        lua_state = lua_state_create(opts->script, err);
        if (!lua_state) goto error;
    }
    sniffer->lua_state = lua_state;
    return sniffer;

error:
    sniffer_destroy(sniffer);
    return NULL;
}

void sniffer_destroy(struct sniffer *sniffer) {
    pcap_close(sniffer->pcap);
    free(sniffer->dev);
    free(sniffer->filter);
    hashtable_destroy(sniffer->syn_tab);
    hashtable_destroy(sniffer->requests);
    if (sniffer->bpf) {
        pcap_freecode(sniffer->bpf);
        free(sniffer->bpf);
    }
    if (sniffer->lua_state) lua_close(sniffer->lua_state);
    free(sniffer);
}

pcap_t *sniffer_offline(const char *file, char *err) {
    FILE *fp;

    if (!(fp = fopen(file, "r"))) {
        strcpy(err, strerror(errno));
        return NULL;
    }
    return pcap_fopen_offline(fp, err);
}

pcap_t *sniffer_online(const char *dev, int snaplen, int buf_size, char *err) {
    int status;
    pcap_t *pcap;
    bpf_u_int32 net = 0, mask = 0;
    const char *new_dev = dev;

    if (pcap_lookupnet(dev, &net, &mask, err) == -1) {
        return NULL;
    }
    pcap = pcap_create(new_dev, err);
    if (!pcap) return NULL;
    status = pcap_set_snaplen(pcap, snaplen);
    if (status < 0) goto error;
    status = pcap_set_timeout(pcap, 1);
    if (status < 0) goto error;
    status = pcap_set_buffer_size(pcap, buf_size);
    if (status < 0) goto error;
    status = pcap_set_tstamp_type(pcap, PCAP_TSTAMP_ADAPTER);
    if (status < 0) goto error;
    status = pcap_activate(pcap);
    if (status < 0) goto error;
    return pcap;

error:
    snprintf(err, PCAP_ERRBUF_SIZE, "%s: %s", dev, pcap_statustostr(status));
    pcap_close(pcap);
    return NULL;
}

static void packet_handler(unsigned char *user,
        const struct pcap_pkthdr *header,
        const unsigned char *packet) {

    const struct ip* ip_packet;
    struct sniffer *sniffer = (struct sniffer*) user;

    switch(pcap_datalink((pcap_t*)sniffer->pcap)) {
        case DLT_NULL:
            ip_packet = (const struct ip *)(packet + NULL_HDRLEN);
            break;
        case DLT_LINUX_SLL: 
            ip_packet = (const struct ip *)(packet + sizeof(struct sll_header));
            break;
        case DLT_EN10MB:
            ip_packet = (const struct ip *)(packet + sizeof(struct ether_header));
            break;
        case DLT_RAW:
            ip_packet = (const struct ip *)packet;
            break;
        default:
            // do nothing when the link type was unknown
            return;
    }

    struct user_packet upacket;
    upacket.size = header->len;
    upacket.payload_size = (int)header->caplen;
    if (ip_packet->ip_p == IPPROTO_TCP) {
        process_tcp_packet(sniffer, header->ts, ip_packet, &upacket);
        process_user_packet(sniffer, &upacket);
    } else if (ip_packet->ip_p == IPPROTO_UDP) {
        process_udp_packet(sniffer, header->ts, ip_packet, &upacket);
        process_user_packet(sniffer, &upacket);
    }
    // skip the packet if not TCP or UDP
}

int sniffer_run(struct sniffer *sniffer) {
    return pcap_loop(sniffer->pcap, -1, packet_handler, (unsigned char *)sniffer);
}

void sniffer_terminate(struct sniffer *sniffer) {
    pcap_breakloop(sniffer->pcap);
}
