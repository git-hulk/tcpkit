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
