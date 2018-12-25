/**
 *   tcpkit --  toolkit to analyze tcp packets
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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <lua.h>

#include "vm.h"
#include "util.h"
#include "packet.h"
#include "array.h"
#include "tcpkit.h"
#include "sniffer.h"
#include "server.h"
#include "logger.h"

#define VERSION "1.0.1"

static server srv;

void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        server_print_stats(&srv);
        server_deinit(&srv);
        exit(0);
    }
}

static void usage() {
    fprintf(stderr, "TCPKIT is a tool to capture tcp packets and analyze the packets with lua.\n");
    fprintf(stderr, "\t-s which server ip to monitor, e.g. 192.168.1.2,192.168.1.3\n");
    fprintf(stderr, "\t-p which n_latency to monitor, e.g. 6379,6380\n");
    fprintf(stderr, "\t-P stats listen port, default is 33333\n");
    fprintf(stderr, "\t-i network card interface, e.g. bond0, lo, em0... see 'ifconfig'\n");
    fprintf(stderr, "\t-d daemonize, run process in background\n");
    fprintf(stderr, "\t-r set offline file captured by tcpdump or tcpkit\n");
    fprintf(stderr, "\t-t request latency threshold(unit in millisecond), default is 50ms\n");
    fprintf(stderr, "\t-m protocol mode, raw,redis,memcached,http\n");
    fprintf(stderr, "\t-w dump packets to 'savefile'\n");
    fprintf(stderr, "\t-S lua script path, default is ../scripts/example.lua\n");
    fprintf(stderr, "\t-B operating system capture buffer size, in units of KiB (1024 bytes)\n");
    fprintf(stderr, "\t-o log output file\n");
    fprintf(stderr, "\t-u udp\n");
    fprintf(stderr, "\t-v version\n");
    fprintf(stderr, "\t-h help\n");
}

static struct array *parse_ports(char *input) {
    int i, port;
    char *elem;
    struct array *ports, *tokens;

    tokens = split_string(input, ',');
    if (!tokens) return NULL;
    ports = array_alloc(sizeof(int), array_used(tokens));
    for (i = 0; i < array_used(tokens); i++) {
        port = atoi(*(char **)array_pos(tokens, i));
        if (port > 0 && port < 65535) {
            elem = array_push(ports);
            memcpy(elem, (char *)&port, sizeof(int));
        }
    }
    free_split_string(tokens);
    return ports;
}

static int parse_protocol(const char *protocol) {
   if (!strncasecmp(protocol, "redis", strlen(protocol))) {
       return P_REDIS;
   } else if (!strncasecmp(protocol, "memcached", strlen(protocol))) {
       return P_MEMCACHED;
   } else if (!strncasecmp(protocol, "http", strlen(protocol))) {
       return P_HTTP;
   }
   return P_RAW;
}

static options* parse_options(int argc, char **argv) {
    int ch, show_usage = 0;
    options *opts;

    opts = calloc(1, sizeof(*opts));
    opts->tcp = 1;
    opts->mode = P_RAW;
    opts->threshold_ms = 0;
    opts->stats_port = 33333;
    opts->threshold_ms = 50;
    opts->buffer_size = 2000 * 1024 * 1024;
    while((ch = getopt(argc, argv, "s:p:P:r:t:m:w:i:S:B:o:udh")) != -1) {
        switch(ch) {
            case 's': opts->servers = split_string(optarg, ','); break;
            case 'p': opts->ports = parse_ports(optarg); break;
            case 'P': opts->stats_port = atoi(optarg); break;
            case 'r': opts->offline_file = strdup(optarg); break;
            case 't': opts->threshold_ms = atoi(optarg); break;
            case 'm': opts->mode = parse_protocol(optarg); break;
            case 'w': opts->save_file = strdup(optarg); break;
            case 'i': opts->device = strdup(optarg); break;
            case 'S': opts->script = strdup(optarg); break;
            case 'o': opts->logfile = strdup(optarg); break;
            case 'B': opts->buffer_size = atoi(optarg) * 1024; break;
            case 'd': opts->daemonize = 1; break;
            case 'u': opts->tcp = 0; break;
            case 'h': show_usage = 1; break;
            default: break;
        }
    }
    if (show_usage) {
        usage();
        exit(0);
    }
    if (!opts->device) opts->device = strdup("any");
    return opts;
}

static char *gen_filter(server *srv) {
    int i, n = 0, size = 0;
    char *buf;
    options *opts;

    opts = srv->opts;
    size = 2*32*array_used(opts->ports)+2*64*srv->n_server;
    buf = malloc(size);

    buf[n++] = '(';
    buf[n++] = '(';
    for (i = 0; i < srv->n_server; i++) {
        n += snprintf(buf+n, size-n, "src host %s or ", inet_ntoa(srv->servers[i]));
    }
    n -= 4;
    buf[n++] = ')';
    n += snprintf(buf+n, size-n, " and (");
    for (i = 0; i < array_used(opts->ports); i++) {
        n += snprintf(buf+n, size-n, "src port %d or ", *(int*)array_pos(opts->ports, i));
    }
    n -= 4;
    buf[n++] = ')';
    n += snprintf(buf+n, size-n, ") or ((");
    for (i = 0; i < srv->n_server; i++) {
        n += snprintf(buf+n, size-n, "dst host %s or ", inet_ntoa(srv->servers[i]));
    }
    n -= 4;
    buf[n++] = ')';
    n += snprintf(buf+n, size-n, " and (");
    for (i = 0; i < array_used(opts->ports); i++) {
        n += snprintf(buf+n, size-n, "dst port %d or ", *(int*)array_pos(opts->ports, i));
    }
    n -= 4;
    buf[n++] = ')';
    buf[n++] = ')';
    buf[n++] = '\0';
    return buf;
}

static void daemonize() {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        alog(ERROR, "Failed to fork the process");
    }
    if (pid > 0) exit(EXIT_SUCCESS); // parent process
    // change the file mode
    umask(0);
    if (setsid() < 0) {
        alog(ERROR, "Failed to set session id, err: %s", strerror(errno));
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char **argv) {
    char err_buf[MAX_ERR_BUFF_SIZE];
    pcap_t *sniffer;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    set_log_fp(stdout);
    alog(INFO, "TCPKIT @version %s, developed by @git-hulk", VERSION);
    srv.opts = parse_options(argc, argv);
    if (getuid() != 0 && !srv.opts->offline_file) {
        alog(FATAL, "You need to be root to capture the packets online.");
    }
    if (array_used(srv.opts->ports) <= 0) {
        alog(FATAL, "Please use -p [port1,port2,...] to specify the server ports");
    }
    if (server_init(&srv) == -1) {
        alog(FATAL, "Failed to init server");
    }
    if (srv.n_server == 0) {
        alog(FATAL, "Please use -s [server1,server2,...] to specify the server ips");
    }
    if (srv.opts->script) {
        srv.vm = vm_open_with_script(srv.opts->script, err_buf);
        if (!srv.vm) alog(FATAL, "Failed to open vm, err: %s", err_buf);
    }
    if (!srv.opts->offline_file) {
        sniffer = sniffer_packet_online(&srv.opts->device, srv.opts->buffer_size, err_buf);
    } else {
        sniffer = sniffer_packet_offline(srv.opts->offline_file, err_buf);
    }
    if (!sniffer) alog(FATAL, "Failed to setup the sniffer, err: %s", err_buf);
    srv.sniffer = sniffer;
    srv.filter = gen_filter(&srv);
    alog(INFO, "Running in [%s] side with device %s with filter[%s]",
            srv.is_server_mode?"server":"client", srv.opts->device, srv.filter);
    if (srv.opts->logfile) {
        FILE *fp = fopen(srv.opts->logfile, "a");
        if (!fp) alog(FATAL, "Failed to open log file, err:%s", strerror(errno));
        set_log_fp(fp);
    }
    if (srv.opts->daemonize) daemonize();
    // don't setup stats port while offline or raw mode
    if (!srv.opts->offline_file && srv.opts->mode != P_RAW) {
        server_create_stats_thread(&srv);
    }
    if (sniffer_loop(sniffer, srv.filter, extract_packet_handler, &srv) == -1) {
        alog(FATAL, "Failed to start the sniffer, err: %s", pcap_geterr(sniffer));
    }
    server_print_stats(&srv);
    server_deinit(&srv);
    return 0;
}
