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

#ifndef TCPKIT_SERVER_H
#define TCPKIT_SERVER_H

#include <pthread.h>
#include <signal.h>
#include "tcpkit.h"

#if defined(__GNUC__)
#define TK_LOAD(p)      __atomic_load_n((p), __ATOMIC_ACQUIRE)
#define TK_STORE(p, v)  __atomic_store_n((p), (v), __ATOMIC_RELEASE)
#else
#define TK_LOAD(p)      (*(p))
#define TK_STORE(p, v)  (*(p) = (v))
#endif

struct server {
    struct options *opts;
    struct sniffer *sniffer;
    pthread_t stats_tid;
    /* Set by server_terminate, which also runs from the signal handler, and
     * read by the stats thread. */
    sig_atomic_t stopped;
};

struct server *server_create(struct options *opts, char *err); 
int server_run(struct server *srv, char *err);
void server_terminate(struct server *srv); 
void server_destroy(struct server *srv);
#endif
