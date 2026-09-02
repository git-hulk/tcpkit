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
    struct bpf_program *bpf;
    pcap_dumper_t *file;
    pcap_t *pcap;
};

struct dumper *dumper_create(struct options *opts, char *err); 
void dumper_terminate(struct dumper *d);
int dumper_run(struct dumper *d);
void dumper_destroy(struct dumper *d);

#endif
