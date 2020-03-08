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
