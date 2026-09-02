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
#include "tcpkit.h"

struct server {
    struct options *opts;
    struct sniffer *sniffer;
    struct dumper* dumper;
    pthread_t dumper_tid;
    pthread_t stats_tid;
    int stopped;
};

struct server *server_create(struct options *opts, char *err); 
int server_run(struct server *srv, char *err);
void server_terminate(struct server *srv); 
void server_destroy(struct server *srv);
#endif
