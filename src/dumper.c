/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   SPDX-License-Identifier: MIT
 *
 *   Use of this source code is governed by the MIT license that can be found
 *   in the LICENSE file at the root of this repository.
 *
 **/

#include <stdio.h>
#include <stdlib.h>

#include "dumper.h"

struct dumper *dumper_create(pcap_t *pcap, const char *path, char *err) {
    struct dumper *d;

    d = calloc(1, sizeof(*d));
    if (!d) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "out of memory");
        return NULL;
    }
    d->file = pcap_dump_open(pcap, path);
    if (!d->file) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "%s", pcap_geterr(pcap));
        free(d);
        return NULL;
    }
    return d;
}

void dumper_write(struct dumper *d, const struct pcap_pkthdr *header,
        const unsigned char *packet) {
    pcap_dump((unsigned char *)d->file, header, packet);
}

void dumper_destroy(struct dumper *d) {
    if (!d) return;
    if (d->file) pcap_dump_close(d->file);
    free(d);
}
