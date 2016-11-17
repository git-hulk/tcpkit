#include "tcpkit.h"
#include "packet.h"
#include "local_addresses.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// global lua state
static lua_State *L = NULL; 
struct tk_options opts;
struct bandwidth bw;

lua_State *
get_lua_vm() {
    return L;
}

struct tk_options *
get_global_options()
{
    return &opts;
}

struct bandwidth* get_global_bandwidth()
{
    return &bw;
}

void need_report_bandwidth()
{
    uint64_t now_ts, delta;
    struct timeval now;
    char in_speed_buf[16], out_speed_buf[16];

    gettimeofday(&now, NULL);
    now_ts = now.tv_sec * 1000000 + now.tv_usec;
    delta = now_ts - bw.last_calc_time;
    if (delta >= opts.duration * 1000000) {
        // trans to second
        delta /= 1000000;
        speed_human(bw.in_bytes / delta, in_speed_buf, 16);
        speed_human(bw.out_bytes / delta, out_speed_buf, 16);
        if (bw.last_calc_time) {
            printf("Incoming %s, %llu packets/s, Outgoing %s, %llu packets/s\n", 
                    in_speed_buf,
                    (unsigned long long)bw.in_packets / delta,
                    out_speed_buf,
                    (unsigned long long)bw.out_packets / delta
                  );
        }
        // reset bandwith
        bw.last_calc_time = now_ts;
        bw.in_bytes = bw.out_bytes = 0;
        bw.in_packets = bw.out_packets = 0;
    }
}

int
is_client_mode ()
{
    return opts.server == NULL ? 0 : 1;
}

void
usage(char *prog)
{
    fprintf(stderr, "%s is a tool to capature tcp packets, and you can analyze packets with lua.\n", prog);
    fprintf(stderr, "\t-s server ip.\n");
    fprintf(stderr, "\t-p port.\n");
    fprintf(stderr, "\t-i device.\n");
    fprintf(stderr, "\t-S lua script path, default is ../scripts/example.lua.\n");
    fprintf(stderr, "\t-l local address.\n");
    fprintf(stderr, "\t-C calculate bandwidth mode.\n");
    fprintf(stderr, "\t-d duration, take effect when -C is set.\n");
    fprintf(stderr, "\t-f log file.\n");
    fprintf(stderr, "\t-t only tcp.\n");
    fprintf(stderr, "\t-t only udp.\n");
    fprintf(stderr, "\t-v version.\n");
    fprintf(stderr, "\t-h help.\n");
}

void check_lua_script() {
    // calculate don't need script handler
    if (opts.is_calc_mode) return;

    if(!opts.script) {
        opts.script = "../scripts/example.lua";
    }
    if(access(opts.script, R_OK) == -1) {
        logger(ERROR, "load lua script %s failed, as %s\n", opts.script, strerror(errno));
        exit(0);
    }
    L = script_init(opts.script);
    // check callback function is exist.
    if(!script_check_func_exists(L, DEFAULT_CALLBACK)) {
        logger(ERROR, "function %s is required in lua script.\n", DEFAULT_CALLBACK);
        exit(0);
    }
}

int
main(int argc, char **argv)
{
    int ret, only_tcp = 0, only_udp = 0;
    pcap_wrapper *pw;
    char ch, filter[128], *protocol, is_usage = 0, show_version = 0;

    while((ch = getopt(argc, argv, "s:p:i:S:Cd:l:r:uthv")) != -1) {
        switch(ch) {
            case 's': opts.server = strdup(optarg); break;
            case 'r': opts.offline_file = strdup(optarg); break;
            case 'p': opts.port= atoi(optarg); break;
            case 'i': opts.device = strdup(optarg); break;
            case 'S': opts.script = strdup(optarg); break;
            case 'C': opts.is_calc_mode = 1; break;
            case 'd': opts.duration = atoi(optarg); break;
            case 't': only_tcp = 1; break;
            case 'u': only_udp = 1; break;
            case 'l':
                opts.specified_addresses = 1;
                if (parse_addresses(optarg)) {
                    logger(ERROR, "parsing local addresses\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'f': opts.log_file = strdup(optarg); break;
            case 'h': is_usage = 1; break;
            case 'v': show_version= 1; break;
        }
    }

    if( is_usage ) {
        usage(argv[0]);
        exit(0);
    }
    if(show_version) {
        printf("%s version is %s\n", argv[0], VERSION);
        exit(0);
    }
    if(!opts.specified_addresses && get_addresses() != 0) {
        exit(0);
    }
    if (!opts.port && !opts.offline_file) logger(ERROR, "port is required.\n");

    if (!opts.device) {
        opts.device = strdup("any");
    }

    if (opts.log_file) set_log_file(opts.log_file); 

    if (opts.offline_file) {
        pw = pw_create_offline(opts.offline_file);
    } else {
        pw = pw_create(opts.device);
    }
    if (!pw) {
        logger(ERROR, "may be you should assign device use -i and swith to root.\n");
    }

    check_lua_script();

    protocol = " ";
    if (only_udp) protocol = "udp";
    if (only_tcp) protocol = "tcp";
    if(opts.server && opts.port) {
        snprintf(filter, sizeof(filter), "host %s and %s port %d", opts.server, protocol, opts.port);
    } else if (opts.port) {
        snprintf(filter, sizeof(filter), "%s port %d", protocol, opts.port);
    } else { // without filter
        snprintf(filter, sizeof(filter), "");
    }
    ret = core_loop(pw, filter, process_packet);
    if(ret == -1) logger(ERROR, "start core loop failed, as %s.\n", pcap_geterr(pw->pcap));

    pw_release(pw);
    script_release(L);
    return 0;
}
