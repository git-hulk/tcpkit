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

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pcap/pcap.h>

#include "logger.h"
#include "sniffer.h"

pcap_t *sniffer_packet_offline(const char *file, char *err_buf) {
    FILE *fp;

    if (!(fp = fopen(file, "r"))) {
        strcpy(err_buf, strerror(errno));
        return NULL;
    }
    return pcap_fopen_offline(fp, err_buf);
}

pcap_t *sniffer_create(const char *device, int snaplen, int promisc, int to_ms, char *errbuf) {
    pcap_t *p;
    int status;

    p = pcap_create(device, errbuf);
    if (p == NULL)
        return (NULL);
    status = pcap_set_snaplen(p, snaplen);
    if (status < 0)
        goto fail;
    status = pcap_set_promisc(p, promisc);
    if (status < 0)
        goto fail;
    status = pcap_set_timeout(p, to_ms);
    if (status < 0)
        goto fail;
    if ((status = pcap_set_tstamp_type(p, PCAP_TSTAMP_ADAPTER)) < 0) {
        alog(WARN, "Failed to set timestamp type, err:%s", pcap_statustostr(status));
    }
    status = pcap_activate(p);
    if (status < 0)
        goto fail;
    return (p);
fail:
    if (status == PCAP_ERROR)
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s", device, pcap_statustostr(status));
    else if (status == PCAP_ERROR_NO_SUCH_DEVICE ||
        status == PCAP_ERROR_PERM_DENIED ||
        status == PCAP_ERROR_PROMISC_PERM_DENIED)
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s", device,
            pcap_statustostr(status));
    else
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s", device,
            pcap_statustostr(status));
    pcap_close(p);
    return (NULL);
}

pcap_t *sniffer_packet_online(char **device, int buffer_size, char *err_buf) {
    pcap_t *pcap;
    bpf_u_int32 net = 0, mask = 0;
    const int timeout = 1, capture_length = 256;
    char *new_device;

    if (pcap_lookupnet(*device, &net, &mask, err_buf) == -1) {
        return NULL;
    }
    pcap = sniffer_create(*device, capture_length, 0, timeout, err_buf);
    if (pcap) {
        pcap_set_buffer_size(pcap, buffer_size);
    }
    if(pcap || strcasecmp(*device, "any") != 0) {
        return pcap;
    }

    new_device = pcap_lookupdev(err_buf);
    if (new_device) {
        if (*device) free(*device);
        *device = strdup(new_device);
        pcap = sniffer_create(*device, capture_length, 0, timeout, err_buf);
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
