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
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pcap.h>

#include "log.h"
#include "tcpkit.h"
#include "server.h"

struct server *srv;

void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        server_terminate(srv);
    }
}

void usage() {
    const char *usage_literal = ""
        "the tcpkit was designed to make network packets programable with LUA by @git-hulk\n"
        "   -h, Print the tcpkit version strings, print a usage message, and exit\n"
        "   -i interface, Listen on network card interface\n"
        "   -A Print each packet (minus its link level header) in ASCII.  Handy for capturing web pages.\n"
        "   -r file, Read packets from file (which was created with the -w option or by other tools that write pcap)\n"
        "   -B buffer_size, Set the operating system capture buffer size to buffer_size, in units of KiB (1024 bytes)\n"
        "   -s snaplen, Snarf snaplen bytes of data from each packet rather than the default of 1500 bytes\n" 
        "   -S file, Push packets to lua state if the script was specified\n"
        "   -t threshold, Print the request lantecy which slower than the threshold, in units of Millisecond\n"
        "   -w file, Write the raw packets to file\n" 
        "   -p protocol, Parse the packet if the protocol was specified (supports: redis, memcached, http, raw)\n"
        "   -P stats port, Listen port to fetch the latency stats, default is 33333\n\n\n"
        ""
        "For example:\n\n"
        "   `tcpkit -i eth0 tcp port 6379 -p redis` was used to monitor the redis reqeust latency\n\n"
        "   `tcpkit -i eth0 tcp port 6379 -p redis -w 6379.pcap` would also dump the packets to `6379.pcap`\n\n"
        "   `tcpkit -i eth0 tcp port 6379 -p redis -t 10` would only print the request latency slower than 10ms\n";
    color_printf(GREEN, "%s\n", usage_literal);
    exit(0);
}

void init_options(struct options *opts) {
    opts->dev = strdup("any");
    opts->ascii = 0;
    opts->filter = NULL;
    opts->offline_file = NULL;
    opts->save_file = NULL;
    opts->script = NULL;
    opts->snaplen = 1500;
    opts->buf_size = 512 * 1024 * 1024;
    opts->print_usage = 0;
    opts->print_version = 0;
    opts->protocol = ProtocolRaw;
    opts->threshold = 0;
    opts->stats_port = 33333;
}

void free_options(struct options *opts) {
    if (opts->dev) free(opts->dev);
    if (opts->filter) free(opts->filter);
    if (opts->offline_file) free(opts->offline_file);
    if (opts->script) free(opts->script);
    free(opts);
}

struct options *parse_options(int argc, char **argv) {
    int i, lastarg;
    struct options *opts;

    opts = malloc(sizeof(*opts));
    init_options(opts);
    for (i = 1; i < argc; i++) {
        lastarg = (i == (argc-1));
        if (!strcmp(argv[i],"-v")) {
            opts->print_version = 1; 
        } else if (!strcmp(argv[i],"-h")) {
            opts->print_usage = 1;
        } else if (!strcmp(argv[i],"-A")) {
            opts->ascii = 1;
        } else if (!strcmp(argv[i],"-i")) {
            if (lastarg) goto invalid;
            if (opts->dev) free(opts->dev);
            opts->dev = strdup(argv[++i]);
        } else if (!strcmp(argv[i],"-B")) {
            if (lastarg) goto invalid;
            opts->buf_size = atoi(argv[++i]) * 1024;
        } else if (!strcmp(argv[i],"-t")) {
            if (lastarg) goto invalid;
            opts->threshold = atoi(argv[++i]);
        } else if (!strcmp(argv[i],"-p")) {
            if (lastarg) goto invalid;
            if (!strcmp(argv[i+1],"raw")) {
                opts->protocol = ProtocolRaw;
            } else if (!strcmp(argv[i+1],"redis")) {
                opts->protocol = ProtocolRedis;
            } else if (!strcmp(argv[i+1],"memcached")) {
                opts->protocol = ProtocolMemcached;
            } else if (!strcmp(argv[i+1],"http")) {
                opts->protocol = ProtocolHTTP;
            } else {
                goto invalid;
            }
            i++;
        } else if (!strcmp(argv[i],"-P")) {
            if (lastarg) goto invalid;
            opts->stats_port = atoi(argv[++i]);
        } else if (!strcmp(argv[i],"-s")) {
            if (lastarg) goto invalid;
            opts->snaplen = atoi(argv[++i]);
        } else if (!strcmp(argv[i],"-S")) {
            if (lastarg) goto invalid;
            if (opts->script) free(opts->script);
            opts->script = strdup(argv[++i]);
        } else if (!strcmp(argv[i],"-r")) {
            if (lastarg) goto invalid;
            if (opts->offline_file) free(opts->offline_file);
            opts->offline_file = strdup(argv[++i]);
        } else if (!strcmp(argv[i],"-w")) {
            if (lastarg) goto invalid;
            if (opts->save_file) free(opts->save_file);
            opts->save_file = strdup(argv[++i]);
        } else {
            if (argv[i][0] == '-') goto invalid;
            // treat other options as filter
            if (!opts->filter) {
                opts->filter = strdup(argv[i]);
            } else {
                char *tmp;
                int new_size, old_size = strlen(opts->filter);
                // add 2 for white space and terminal char 
                new_size = old_size+strlen(argv[i])+2;
                tmp = realloc(opts->filter, new_size);
                if (tmp) {
                    opts->filter = tmp;
                    opts->filter[old_size++] = ' ';
                    memcpy(opts->filter+old_size, argv[i], strlen(argv[i]));
                    opts->filter[new_size-1] = '\0'; 
                }
            }
        }
    }
    if (!opts->filter) opts->filter = strdup("tcp");
    return opts;

invalid:
    log_message(FATAL, "Invalid option \"%s\" or option argument missing",argv[i]);
    free_options(opts);
    return NULL;
}

int main(int argc, char **argv) {
    char err[MAX_ERR_BUFF_SIZE];
    struct options *opts; 

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    print_redirect(stdout);
    opts = parse_options(argc, argv);
    if (!opts) log_message(FATAL, "Failed to parse options, %s", err);

    if (opts->print_usage) {
        free_options(opts);
        usage();
    }
    if (getuid() != 0 && !opts->offline_file) {
        free_options(opts);
        log_message(FATAL, "You don't have permission to capture on the network card interface");
    }
    if (opts->snaplen < 64) opts->snaplen = 64;
    if (opts->protocol != ProtocolRaw && opts->snaplen > 256) {
        opts->snaplen = 256;
    }
    srv = server_create(opts, err);
    if (!srv) {
        free_options(opts);
        log_message(FATAL, "Failed to create the sniffer server, %s", err);
    }
    if (server_run(srv, err) == -1) {
        server_destroy(srv);
        free_options(opts);
        log_message(FATAL, "Failed to run the server, %s", err);
    }
    server_destroy(srv);
    free_options(opts);
    return 0;
}
