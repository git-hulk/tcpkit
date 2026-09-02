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

#ifndef TCPKIT_SNIFFER_H
#define TCPKIT_SNIFFER_H

#include <pcap.h>
#include <pthread.h>
#include <sys/time.h>
#include <lua.h>
#include "tcpkit.h"
#include "stats.h"
#include "hashtable.h"
#include "dumper.h"

struct sniffer {
    pcap_t *pcap;
    char *dev;
    char *filter;
    int protocol;
    int threshold;
    int ascii;

    /* syn_tab and the query_stats it owns are read by the stats thread while
     * the capture thread updates them, so both sides hold stats_lock.
     * requests belongs to the capture thread alone and needs no lock. */
    pthread_mutex_t stats_lock;
    int lock_ready;
    struct hashtable *syn_tab;
    struct hashtable *requests;
    struct timeval last_expire;
    int expire_primed;
    lua_State *lua_state;
    struct bpf_program *bpf;
    struct dumper *dumper;
};

static inline void sniffer_stats_lock(struct sniffer *sniffer) {
    pthread_mutex_lock(&sniffer->stats_lock);
}

static inline void sniffer_stats_unlock(struct sniffer *sniffer) {
    pthread_mutex_unlock(&sniffer->stats_lock);
}

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
