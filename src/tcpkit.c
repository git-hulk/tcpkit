#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "tcpkit.h"
#include "packet.h"
#include "stats.h"
#include "local_addresses.h"

struct server {
    char *filter;
    lua_State *vm;
    struct options *opts;
    struct stats *stats;
    struct array *local_addrs;
    pcap_t *sniffer;
    int stop;
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
    if (opts->duration > 0) {// only print stats
        return stats_packet_handler;
    }
    return analyze_packet_handler;
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
    while((ch = getopt(argc, argv, "s:p:i:S:d:l:r:w:uthv")) != -1) {
        switch(ch) {
            case 's': opts->server = strdup(optarg); break;
            case 'r': opts->offline_file = strdup(optarg); break;
            case 'w': opts->save_file = strdup(optarg); break;
            case 'p': opts->port= atoi(optarg); break;
            case 'i': opts->device = strdup(optarg); break;
            case 'S': opts->script = strdup(optarg); break;
            case 'd': opts->duration = atoi(optarg); break;
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
        // FIXME: free options
        pcap_breakloop(srv.sniffer);
        script_release(srv.vm);
        srv.stop = 1;
    }
}

void
signal_handler(int sig) {
    struct pcap_stat ps;
    if (sig == SIGINT || sig == SIGTERM) {
        pcap_stats(srv.sniffer, &ps);
        fprintf(stderr, "%llu packets captured\n", srv.stats->packets);
        fprintf(stderr, "%u packets received by filter\n", ps.ps_recv);
        fprintf(stderr, "%u packets dropped by kernel\n", ps.ps_drop);
        fprintf(stderr, "%u packet dropped by interface\n", ps.ps_ifdrop);
        terminate();
    }
}

int
main(int argc, char **argv)
{
    int ret;
    lua_State *vm;
    pcap_t *sniffer;
    pthread_t stats_tid;
    struct options *opts;
    struct array *local_addrs;
    pcap_handler handler;
    char errbuf[PCAP_ERRBUF_SIZE];

    opts = parse_options(argc, argv);
    if(opts->is_usage) {
        usage(argv[0]);
        exit(0);
    }
    if(opts->show_version) {
        printf("%s version is %s\n", argv[0], VERSION);
        exit(0);
    }
    set_log_file(opts->log_file);
    srv.opts = opts;

    vm = script_create_vm(opts->script);
    if (!vm && !opts->is_calc_mode) {
        logger(ERROR, "Failed to create lua vm");
        exit(0);
    }
    srv.vm = vm;

    if (!script_is_func_exists(vm, DEFAULT_CALLBACK)) {
        logger(ERROR, "Function process_packet was not found");
        exit(0);
    }
    local_addrs = get_local_addresses(opts->local_addresses);
    if(array_used(local_addrs) <= 0) {
        logger(ERROR, "You must run with sudo, or use -l option to set local address\n");
        exit(0);
    }
    srv.local_addrs = local_addrs;
    
    // setup print stats thread
    srv.stats = create_stats();
    if (opts->duration > 0) {
        ret = pthread_create(&stats_tid, NULL, print_stats_routine, (void*)(long)opts->duration);
        if (ret != 0) {
            logger(WARN, "Fail to create print stats thread, err\n", strerror(errno));
        }
    }
    // register signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (opts->offline_file) {
        handler = analyze_packet_handler;
        sniffer = open_pcap_by_offline(opts->offline_file, errbuf);
    } else {
        handler = get_live_handler(opts);
        sniffer = open_pcap_by_device(opts->device, errbuf);
    }
    if (!sniffer) {
        logger(ERROR, "Failed to init tcpkit, err %s\n", errbuf);
        exit(0);
    }
    srv.sniffer = sniffer;
    srv.filter = create_filter(opts);
    logger(INFO, "Setup tcpkit on device %s with filter[%s ]\n", opts->device, srv.filter);
    if(core_loop(sniffer, srv.filter, handler) == -1) {
        logger(ERROR, "Failed to start tcpkit, err %s\n", pcap_geterr(sniffer));
    }
    terminate();
    pcap_close(sniffer);
    return 0;
}
