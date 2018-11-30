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

#ifndef TCPKIT_TCPIKT_H
#define TCPKIT_TCPIKT_H

#include <pthread.h>
#include <lua.h>
#include <pcap/pcap.h>

#include "stats.h"
#include "hashtable.h"

#define MAX_ERR_BUFF_SIZE 256

typedef struct {
    struct array *servers;
    struct array *ports;
    int stats_port;
    int tcp;
    int threshold_ms;
    char *logfile;
    char *offline_file;
    char *save_file;
    char *script;
    char *device;
    int mode;
    int daemonize;
    int buffer_size;
}options;

typedef struct {
    options *opts;
    pcap_t *sniffer;
    char *filter;
    lua_State *vm;
    stats *st;
    hashtable *req_ht;
    pthread_t stats_tid;
    int stop;
    struct in_addr *servers;
    int n_server;
    int is_server_mode;
} server;

typedef enum {
   P_RAW = 0,
   P_REDIS,
   P_MEMCACHED,
   P_HTTP,
}PROTOCOL_TYPE;
#endif //TCPKIT_TCPIKT_H
