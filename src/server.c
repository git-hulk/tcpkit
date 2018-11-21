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

#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>

#include <lua.h>
#include <pcap/pcap.h>
#include <netinet/in.h>

#include "server.h"
#include "tcpikt.h"
#include "sniffer.h"
#include "array.h"
#include "stats.h"
#include "util.h"
#include "logger.h"

int server_init(server *srv) {
    srv->stop = 0;
    srv->req_ht = hashtable_create(102400);
    if (!srv->req_ht) return -1;
    srv->st = stats_create(array_used(srv->opts->ports));
    if (!srv->st) return -1;
    server_create_stats_thread(srv);
    return 0;
}

void server_deinit(server *srv) {
    srv->stop = 1;
    sniffer_terminate(srv->sniffer);
    pthread_join(srv->stats_tid, NULL);
    if (srv->sniffer) pcap_close(srv->sniffer);
    if (srv->vm) lua_close(srv->vm);
    // wait for stats thread
    if (srv->filter) free(srv->filter);
    if (srv->st) stats_destroy(srv->st);
    if (srv->req_ht) hashtable_destroy(srv->req_ht);

    options *opts = srv->opts;
    if (opts) {
        if (opts->script) free(opts->script);
        if (opts->save_file) free(opts->save_file);
        if (opts->ports) array_dealloc(opts->ports);
        if (opts->servers) free_split_string(opts->servers);
        if (opts->offline_file) free(opts->offline_file);
        if (opts->logfile) free(opts->logfile);
        if (opts->device) free(opts->device);
        free(opts);
    }
}

static void server_print_latency_stats(server *srv) {
    int i, j;
    int64_t average_latency;

    rlog("========================= latency stats =========================");
    for (i = 0; i < array_used(srv->opts->ports); i++) {
        if (srv->st->latencies[i].total_reqs == 0) continue;
        average_latency = srv->st->latencies[i].total_costs / srv->st->latencies[i].total_reqs;
        rlog("tcp port: %d, total requests: %" PRId64 ", average latency: %.3f ms, slow threshod: %d ms, slow requests: %" PRId64,
             *(int *) array_pos(srv->opts->ports, i),
             srv->st->latencies[i].total_reqs,
             average_latency/1000.0,
             srv->opts->threshold_ms,
             srv->st->latencies[i].slow_counts
        );

        for (j = 0; j < N_BUCKET; j++) {
            if (srv->st->latencies[i].buckets[j] == 0) continue;
            if (j >= 1) {
                rlog("%s~%s: %lld", latency_buckets_name[j-1], latency_buckets_name[j],
                     srv->st->latencies[i].buckets[j]);
            } else {
                rlog("0ms~%s: %lld", latency_buckets_name[j], srv->st->latencies[i].buckets[j]);
            }
        }
    }
}

void server_print_stats(server *srv) {
    struct pcap_stat stat;

    if (srv->opts->mode != P_RAW) {
        server_print_latency_stats(srv);
    }
    pcap_stats(srv->sniffer, &stat);
    rlog("======================== interface stats ========================");
    rlog("%lld packets captured", srv->st->req_packets + srv->st->rsp_packets);
    rlog("%u packets received by filter", stat.ps_recv);
    rlog("%u packets dropped by kernel", stat.ps_drop);
    rlog("%u packets dropped by interface", stat.ps_ifdrop);
    rlog("=================================================================");
}

char *server_stats_to_json(server *svr) {
    int i, j, size, n = 0;
    char *buf;

    stats *st = svr->st;
    // latencies
    size = (N_BUCKET* 20 + 2 * 20 + 128) * st->n_latency+256;
    buf = malloc(size);
    n += snprintf(buf, size,
                  "{\"in_packets\":%" PRId64 ","
                  "\"out_packets\":%" PRId64 ","
                  "\"in_bytes\":%" PRId64 ","
                  "\"out_bytes\":%" PRId64 ","
                  " \"ports\":",
                  st->req_packets,
                  st->rsp_packets,
                  st->req_bytes,
                  st->rsp_bytes);
    buf[n++] = '[';
    for (i = 0; i < st->n_latency; i++) {
        n += snprintf(buf+n, size-n,
                      "{\"%d\":{\"total_reqs\": %" PRId64 ",\"total_costs\":%" PRId64 ", \"slow_reqs\":%" PRId64 ",\"latencies\":[",
                      *(int*)array_pos(svr->opts->ports, i),
                      st->latencies[i].total_reqs,
                      st->latencies[i].total_costs,
                      st->latencies[i].slow_counts);
        for (j = 0; j < N_BUCKET; j++) {
            n += snprintf(buf+n, size-n, "%" PRId64 ",", st->latencies[i].buckets[j]);
        }
        buf[n-1] = ']';
        buf[n++] = '}';
        buf[n++] = '}';
        buf[n++] = ',';
    }
    buf[n-1] = ']';
    buf[n] = '}';
    buf[n+1] = '\0';
    return buf;
}

static void *server_stats_loop(void *arg) {
    int rc, listen_fd, new_fd;
    server *srv;
    char *stats_buf;
    struct pollfd fds[1];

    srv = (server*)arg;
    listen_fd  = server_listen(srv->opts->stats_port);
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
    while(!srv->stop) {
        rc = poll(fds, 1, 100);
        if (rc <= 0) continue;
        new_fd = accept(listen_fd, NULL, NULL);
        if (new_fd < 0) {
           // log error
            continue;
        }
        stats_buf = server_stats_to_json(srv);
        write(new_fd, stats_buf, strlen(stats_buf));
        close(new_fd);
        free(stats_buf);
    }
    return NULL;
}

void server_create_stats_thread(server *srv) {
    pthread_create(&srv->stats_tid, NULL, server_stats_loop, srv);
}

int server_listen(int port) {
    int listen_fd, rc, enable = 1;
    struct sockaddr_in sin;

    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)  {
        return -1;
    }
    rc = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,(char*)&enable, sizeof(enable));
    if (rc < 0) {
        close(listen_fd);
       return -1;
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0);
    sin.sin_port = htons(port);
    rc = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
    if (rc < 0) {
       return -1;
    }
    rc = listen(listen_fd, 511);
    if (rc < 0) {
       return -1;
    }
    return listen_fd;
}
