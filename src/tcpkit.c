#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "tcpkit.h"
#include "packet.h"
#include "stats.h"
#include "local_addresses.h"

struct server {
    int stop;
    char *filter;
    lua_State *vm;
    pcap_t *sniffer;
    void *private;
    struct stats *stats;
    pcap_handler handler;
    struct options *opts;
    struct array *local_addrs;
    pthread_t stats_tid;
};

struct server srv;

struct lua_State *
get_lua_vm() {
    return srv.vm;
}

struct options *
get_options() {
    return srv.opts;
}

struct stats*
get_stats() {
    return srv.stats;
}

pcap_handler
get_live_handler(struct options *opts) {
    if (opts->duration > 0 && !opts->save_file) {// only print stats
        return stats_packet_handler;
    } else if(opts->save_file) {
        return dump_packet_handler;
    } else {
        return analyze_packet_handler;
    }
}

void* print_stats_routine(void *arg) {
    struct stats *stats= get_stats();
    while(1) {
        need_print_stats(stats, (long)arg);
        usleep(100000); // wake up every 100 millseconds
    };
    return NULL;
}

static void
usage(char *prog) {
    fprintf(stderr, "%s is a tool to capature the tcp packets, and analyze the packets with lua\n", prog);
    fprintf(stderr, "\t-s server ip\n");
    fprintf(stderr, "\t-p port\n");
    fprintf(stderr, "\t-i device\n");
    fprintf(stderr, "\t-r offline file\n");
    fprintf(stderr, "\t-w write the raw packets to file\n");
    fprintf(stderr, "\t-S lua script path, default is ../scripts/example.lua\n");
    fprintf(stderr, "\t-l local address\n");
    fprintf(stderr, "\t-d interval to print stats, unit is second \n");
    fprintf(stderr, "\t-B operating system capture buffer size, in units of KiB (1024 bytes). \n");
    fprintf(stderr, "\t-f log file\n");
    fprintf(stderr, "\t-t only tcp\n");
    fprintf(stderr, "\t-u only udp\n");
    fprintf(stderr, "\t-v version\n");
    fprintf(stderr, "\t-h help\n");
}

static struct options*
parse_options(int argc, char **argv) {
    char ch;
    struct options *opts;

    opts = calloc(1, sizeof(*opts));
    while((ch = getopt(argc, argv, "s:p:i:S:d:B:l:r:w:uthv")) != -1) {
        switch(ch) {
            case 's': opts->server = strdup(optarg); break;
            case 'r': opts->offline_file = strdup(optarg); break;
            case 'w': opts->save_file = strdup(optarg); break;
            case 'p': opts->port= atoi(optarg); break;
            case 'i': opts->device = strdup(optarg); break;
            case 'S': opts->script = strdup(optarg); break;
            case 'd': opts->duration = atoi(optarg); break;
            case 'B': opts->buffer_size = atoi(optarg); break;
            case 't': opts->tcp = 1; break;
            case 'u': opts->udp = 1; break;
            case 'l': opts->local_addresses = strdup(optarg); break;
            case 'f': opts->log_file = strdup(optarg); break;
            case 'h': opts->is_usage = 1; break;
            case 'v': opts->show_version= 1; break;
        }
    }
    opts->device = opts->device ? opts->device : strdup("any");
    opts->is_client_mode = opts->server != NULL;
    return opts;
}

static char *
create_filter(struct options *opts) {
    int n;
    char *protocol = "", *filter;

    n = 128; // it seems like 128 is enough for filter
    filter = malloc(n);
    if (opts->udp) protocol = "udp";
    if (opts->tcp) protocol = "tcp";
    if(opts->server && opts->port) {
        n = snprintf(filter, n, "host %s and %s port %d",
                opts->server, protocol, opts->port);
    } else if (opts->port) {
        n = snprintf(filter, n, "%s port %d", protocol, opts->port);
    } else { // without filter
        n = snprintf(filter, n, " ");
    }
    filter[n] = '\0';
    return filter;
}

int
is_local_address(struct in_addr addr) {
    int i;
    char *elem;
    struct array *addrs;

    addrs = srv.local_addrs;
    if (!addrs) return 0;
    for (i = 0; i < array_used(addrs); i++) {
        elem = array_pos(addrs, i);
        if (((struct in_addr*)elem)->s_addr == addr.s_addr) {
            return 1;
        }
    }
    return 0;
}

void
terminate() {
    if (!srv.stop) {
        pcap_breakloop(srv.sniffer);
        srv.stop = 1;
    }
}

void
signal_handler(int sig) {
    struct pcap_stat ps;
    if (sig == SIGINT || sig == SIGTERM) {
        // Dump stat only online
        if (!srv.opts->offline_file) {
            pcap_stats(srv.sniffer, &ps);
            fprintf(stderr, "%llu packets captured\n", srv.stats->packets);
            fprintf(stderr, "%u packets received by filter\n", ps.ps_recv);
            fprintf(stderr, "%u packets dropped by kernel\n", ps.ps_drop);
            fprintf(stderr, "%u packet dropped by interface\n", ps.ps_ifdrop);
        }
        terminate();
    }
}

char *
init_server(struct options *opts) {
    char *errbuf;
    struct dump_wrapper *dw = NULL;

    errbuf = malloc(PCAP_ERRBUF_SIZE + 1);
    srv.opts = opts;
    set_log_file(opts->log_file);
    srv.vm = script_create_vm(opts->script);
    if (!srv.vm) {
        sprintf(errbuf, "failed to create lua vm");
        return errbuf;
    }
    if (!script_is_func_exists(srv.vm, DEFAULT_CALLBACK)) {
        sprintf(errbuf, "process_packet function was not found");
        return errbuf;
    }
    srv.local_addrs = get_local_addresses(opts->local_addresses);
    if(array_used(srv.local_addrs) <= 0) {
        sprintf(errbuf, "local address list is empty");
        return errbuf;
    }
    srv.stats = create_stats();
    if (opts->duration > 0) {
        if(pthread_create(&srv.stats_tid, NULL, print_stats_routine, (void*)(long)opts->duration)) {
            sprintf(errbuf, "failed to create stats thread");
            return errbuf;
        }
    }
    srv.filter = create_filter(opts);
    if (opts->offline_file) {
        srv.handler = analyze_packet_handler;
        srv.sniffer = open_pcap_by_offline(opts->offline_file, errbuf);
    } else {
        srv.handler = get_live_handler(opts);
        srv.sniffer = open_pcap_by_device(opts->device, errbuf);
        /* Packets that arrive for a capture are stored in a buffer,
         * so that they do not have to be read by the application as
         * soon as they arrive. if too many packets are being captured
         * and the napshot length doesn't limit the amount of data that's buffered,
         * packets could be dropped if the buffer fills up before the application
         * can read packets from it
         */
        if (opts->buffer_size > 0) {
            pcap_set_buffer_size(srv.sniffer, opts->buffer_size * 1024);
        }
    }
    if (!srv.sniffer) {
        return errbuf;
    }
    srv.private = srv.sniffer;
    if (srv.handler == dump_packet_handler) {
        dw = malloc(sizeof(*dw));
        dw->dumper = pcap_dump_open(srv.sniffer, opts->save_file);
        dw->pcap = srv.sniffer;
        srv.private = dw;
    }
    if (dw && !dw->dumper) {
        sprintf(errbuf, "failed to open save file, %s", pcap_geterr(srv.sniffer));
        free(dw);
        return errbuf;
    }
    return NULL;
}

void deinit_server() {
    struct options *opts;

    pcap_close(srv.sniffer);
    script_release(srv.vm);
    if (srv.stats) free(srv.stats);
    if (srv.filter) free(srv.filter);
    if (srv.local_addrs) array_dealloc(srv.local_addrs);
    if (srv.private && srv.private != srv.sniffer) free(srv.private);

    if ((opts = srv.opts)) { // free options
        if (opts->server) free(opts->server);
        if (opts->offline_file) free(opts->offline_file);
        if (opts->save_file) free(opts->save_file);
        if (opts->device) free(opts->device);
        if (opts->script) free(opts->script);
        if (opts->local_addresses) free(opts->local_addresses);
        if (opts->log_file) free(opts->log_file);
    }
}

int
main(int argc, char **argv) {
    char *err;
    struct options *opts;

    opts = parse_options(argc, argv);
    if(opts->is_usage) {
        usage(argv[0]);
        return 0;
    }
    if(opts->show_version) {
        printf("%s version is %s\n", argv[0], VERSION);
        return 0;
    }
    err = init_server(opts);
    if (err) {
        logger(ERROR, "Failed to init server, err: %s", err);
        free(err);
        return 1;
    }
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    logger(INFO, "Setup tcpkit on device %s with filter[%s ]\n", opts->device, srv.filter);
    if(core_loop(srv.sniffer, srv.filter, srv.handler, srv.private) == -1) {
        logger(ERROR, "Failed to start tcpkit, err: %s\n", pcap_geterr(srv.sniffer));
    }

    if (srv.opts->duration > 0) {
        pthread_join(srv.stats_tid, NULL);
    }
    deinit_server();
    return 0;
}
