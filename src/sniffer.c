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
#include <pcap/pcap.h>

#include "sniffer.h"

pcap_t *sniffer_packet_offline(const char *file, char *err_buf) {
    FILE *fp;

    if (!(fp = fopen(file, "r"))) {
        strcpy(err_buf, strerror(errno));
        return NULL;
    }
    return pcap_fopen_offline(fp, err_buf);
}

pcap_t *sniffer_packet_online(char **device, int buffer_size, char *err_buf) {
    pcap_t *pcap;
    bpf_u_int32 net = 0, mask = 0;
    const int timeout = 1, capture_length = 1024;
    char *new_device;

    if (pcap_lookupnet(*device, &net, &mask, err_buf) == -1) {
        return NULL;
    }
    pcap = pcap_open_live(*device, capture_length, 0, timeout, err_buf);
    if(pcap || strcasecmp(*device, "any") != 0) {
        if(pcap) pcap_set_buffer_size(pcap, buffer_size);
        return pcap;
    }

    new_device = pcap_lookupdev(err_buf);
    if (new_device) {
        if (*device) free(*device);
        *device = strdup(new_device);
        pcap = pcap_open_live(*device, capture_length, 0, timeout, err_buf);
    }
    if (pcap) {
        pcap_set_buffer_size(pcap, buffer_size);
    }
    return pcap;
}

// returns 0 if cnt is exhausted or if, when reading from a ``savefile'', no more packets are available
// returns -1 if an error occurs
// returns -2 if  the loop terminated due to a call to pcap_breakloop() before any packets were processed
int sniffer_loop(pcap_t *sniffer, const char *filter, pcap_handler handler, void *user) {
    struct bpf_program bpf;
    if (pcap_compile(sniffer, &bpf, filter, 0, 1) != 0
        || pcap_setfilter(sniffer, &bpf) != 0) {
        return -1;
    }
    return pcap_loop(sniffer, -1, handler, (unsigned char *)user);
}

void sniffer_terminate(pcap_t *sniffer) {
    pcap_breakloop(sniffer);
}
