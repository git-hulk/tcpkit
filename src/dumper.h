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

#ifndef TCPKIT_DUMPER_H
#define TCPKIT_DUMPER_H

#include <pcap.h>
#include "tcpkit.h"

struct dumper {
    pcap_dumper_t *file;
};

/* Writes to `path` through `pcap`, the handle the packets are captured on. */
struct dumper *dumper_create(pcap_t *pcap, const char *path, char *err);
void dumper_write(struct dumper *d, const struct pcap_pkthdr *header,
        const unsigned char *packet);
void dumper_destroy(struct dumper *d);

#endif
