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

#ifndef TCPKIT_SNIFFER_H
#define TCPKIT_SNIFFER_H

#include <pcap.h>
#include <lua.h>
#include "tcpkit.h"
#include "stats.h"
#include "hashtable.h"

struct sniffer {
    pcap_t *pcap;
    char *dev;
    char *filter;
    int protocol;
    int threshold;
    int ascii;

    struct hashtable *syn_tab;
    struct hashtable *requests;
    lua_State *lua_state;
    struct bpf_program *bpf;
};

struct request {
    struct timeval tv;
    int seq;
    int size;
    char *payload;
};


struct sniffer *sniffer_create(struct options *opts, char *err);
int sniffer_run(struct sniffer *sniffer);
pcap_t *sniffer_offline(const char *file, char *err);
pcap_t *sniffer_online(const char *dev, int snaplen, int buf_size, char *err);
int sniffer_loop(pcap_t *pcap, const char *filter, pcap_handler handler, void *user); 
struct bpf_program *sniffer_compile(pcap_t *pcap, const char *filter, char *err); 
void sniffer_terminate(struct sniffer *sniffer);
void sniffer_destroy(struct sniffer *sniffer); 
#endif
