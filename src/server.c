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
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tcpkit.h"
#include "server.h"
#include "sniffer.h"
#include "dumper.h"
#include "log.h"
#include "stats.h"
#include "cJSON.h"
#include "hashtable.h"

static int server_spwan_dumper_thread(struct server *srv);
static void *server_stats_loop(void *arg);
static int server_spwan_stats_thread(struct server *srv);

struct server *server_create(struct options *opts, char *err) {
    struct server *srv;
    struct dumper *d;

    srv = calloc(1, sizeof(*srv));
    if (!srv) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "out of memory");
        return NULL;
    }
    srv->opts = opts;
    srv->sniffer = sniffer_create(opts, err);
    if (!srv->sniffer) goto error;

    if (!opts->offline_file && opts->save_file) {
        if (!(d = dumper_create(opts, err))) goto error;
        srv->dumper = d;
        if (server_spwan_dumper_thread(srv) != 0) {
            snprintf(err, MAX_ERR_BUFF_SIZE, "create the dumper thread: %s", strerror(errno));
            goto error;
        }
    }
    if (opts->protocol != ProtocolRaw) {
        if (server_spwan_stats_thread(srv) != 0) {
            snprintf(err, MAX_ERR_BUFF_SIZE, "create the stats thread: %s", strerror(errno));
            goto error;
        }
    }
    return srv;

error:
    server_terminate(srv);
    if (srv->dumper_tid) pthread_join(srv->dumper_tid, NULL);
    if (srv->stats_tid) pthread_join(srv->stats_tid, NULL);
    server_destroy(srv);
    return NULL;
}

int server_run(struct server *srv, char *err) {
    struct pcap_stat stat;

    if (sniffer_run(srv->sniffer) == -1) {
        server_terminate(srv);
        snprintf(err, MAX_ERR_BUFF_SIZE, "%s", pcap_geterr(srv->sniffer->pcap));
        return -1;
    }
    server_terminate(srv);
    if (srv->dumper_tid) pthread_join(srv->dumper_tid, NULL);
    if (srv->stats_tid) pthread_join(srv->stats_tid, NULL);
    if (!srv->opts->offline_file) {
        if (pcap_stats(srv->sniffer->pcap, &stat) == 0) {
            printf("\n======================== interface stats ========================\n");
            printf("%u packets received by filter\n", stat.ps_recv);
            printf("%u packets dropped by kernel\n", stat.ps_drop);
            printf("%u packets dropped by interface\n", stat.ps_ifdrop);
            printf("======================== interface stats ========================\n");
        } else {
            log_message(WARN, "Failed to read the interface stats, err: %s",
                    pcap_geterr(srv->sniffer->pcap));
        }
    }

    return 0;
}

void server_terminate(struct server *srv) {
    if (!srv || srv->stopped) return;
    srv->stopped = 1;
    sniffer_terminate(srv->sniffer);
    if (srv->dumper) dumper_terminate(srv->dumper);
}

void server_destroy(struct server *srv) {
    if (!srv) return;
    sniffer_destroy(srv->sniffer);
    if (srv->dumper) dumper_destroy(srv->dumper);
    free(srv);
}

static void* server_dump_loop(void *arg) {
    struct dumper *d;
    char err[MAX_ERR_BUFF_SIZE];
    
    d = (struct dumper *) arg;
    if (dumper_run(d) == -1) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "%s", pcap_geterr(d->pcap));
    }
    return NULL;
}

static int server_spwan_dumper_thread(struct server *srv) {
    int ret;
    pthread_t tid;

    ret = pthread_create(&tid, NULL, server_dump_loop, srv->dumper);
    if (ret != 0) {
        return ret;
    }
    srv->dumper_tid = tid;
    return 0; 
}

static int server_spwan_stats_thread(struct server *srv) {
    int ret;
    pthread_t tid;

    ret = pthread_create(&tid, NULL, server_stats_loop, srv);
    if (ret != 0) {
        return ret;
    }
    srv->stats_tid = tid;
    return 0;
}

static int server_listen(int port) {
    int listen_fd, rc;
    struct sockaddr_in sin;

    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)  {
        log_message(ERROR, "Failed to setup the stats listener, err: %s", strerror(errno));
        return -1;
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0);
    sin.sin_port = htons(port);
    rc = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
    if (rc < 0) {
        log_message(ERROR, "Failed to bind stats port(%d), err: %s", port, strerror(errno));
        close(listen_fd);
        return -1;
    }
    rc = listen(listen_fd, 511);
    if (rc < 0) {
        log_message(ERROR, "Failed to listen on stats port(%d), err: %s", port, strerror(errno));
        close(listen_fd);
        return -1;
    }
    return listen_fd;
}

static void write_all(int fd, const char *buf, size_t len) {
    ssize_t n;
    size_t off = 0;

    while (off < len) {
        n = write(fd, buf + off, len - off);
        if (n > 0) {
            off += (size_t)n;
        } else if (n < 0 && errno == EINTR) {
            continue;
        } else {
            return;
        }
    }
}

static char *server_stats_to_json(struct server *srv) {
    cJSON *object;
    void **values;
    int i, cnt = 0;
    struct query_stats *stats;
    char buf[64], *stats_json_str;

    object = cJSON_CreateObject();
    values = hashtable_values(srv->sniffer->syn_tab, &cnt);
    for (i = 0; i < cnt; i++) {
        stats = (struct query_stats *)values[i];        
        if (!stats) continue;
        snprintf(buf, 64, "%s:%d", inet_ntoa(stats->ip), stats->port);
        cJSON_AddItemToObject(object, buf, create_stats_object(stats));
    }
    stats_json_str = cJSON_Print(object);
    cJSON_Delete(object);
    free(values);
    return stats_json_str;
}

static void *server_stats_loop(void *arg) {
    int rc, listen_fd, new_fd;
    struct server *srv;
    char *stats_buf;
    struct pollfd fds[1];

    srv = (struct server*)arg;
    listen_fd = server_listen(srv->opts->stats_port);
    if (listen_fd == -1) {
        log_message(WARN, "The latency stats are not served on port %d",
                srv->opts->stats_port);
        return NULL;
    }
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
    while(!srv->stopped) {
        rc = poll(fds, 1, 100);
        if (rc <= 0) continue;
        new_fd = accept(listen_fd, NULL, NULL);
        if (new_fd < 0) continue;
        stats_buf = server_stats_to_json(srv);
        if (stats_buf) {
            write_all(new_fd, stats_buf, strlen(stats_buf));
            free(stats_buf);
        }
        close(new_fd);
    }
    close(listen_fd);
    return NULL;
}
