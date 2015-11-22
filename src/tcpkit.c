#include "tcpkit.h"
#include "packet.h"
#include "local_addresses.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct tk_options opts;

// global lua state
static lua_State *L = NULL; 

lua_State *
get_lua_vm() {
    return L;
}

struct tk_options *
get_global_options()
{
    return &opts;
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
    fprintf(stderr, "\t-S lua script path, detail in example.lua.\n");
    fprintf(stderr, "\t-l local address.\n");
    fprintf(stderr, "\t-f log file.\n");
    fprintf(stderr, "\t-h help.\n");
}

int
main(int argc, char **argv)
{
    char ch, filter[128], is_usage = 0;

    while((ch = getopt(argc, argv, "s:p:i:S:l:h")) != -1) {
        switch(ch) {
            case 's': opts.server = strdup(optarg); break;                       
            case 'p': opts.port= atoi(optarg); break;                       
            case 'i': opts.device = strdup(optarg); break;                       
            case 'S': opts.script = strdup(optarg); break;                       
            case 'l':
                opts.specified_addresses = 1;
                if (parse_addresses(optarg)) {
                    logger(ERROR, "parsing local addresses\n");
                    return EXIT_FAILURE;

                } 
                break;                       

            case 'f': opts.log_file = strdup(optarg); break;                       
            case 'h': is_usage = 1; break;                       
        }
    }

    if( is_usage ) {
        usage(argv[0]);
        exit(0);
    }

    if(!opts.script) {
        opts.script = "example.lua";
    }
    if(access(opts.script, R_OK) == -1) {
        logger(ERROR, "load lua script failed, as %s\n", strerror(errno));
        exit(0);
    }

    L = script_init(opts.script);
    // check callback function is exist.
    if(! script_check_func_exists(L, DEFAULT_CALLBACK)) {
        logger(ERROR, "function %s is required in lua script.\n", DEFAULT_CALLBACK);
        exit(0);
    }

    if(!opts.specified_addresses && get_addresses() != 0) {
        exit(0);
    }

    if(!opts.port) {
        logger(ERROR, "port is required.\n");
        exit(0);
    }
    if (! opts.device) {
        logger(ERROR, "device is required.\n");
        exit(0);
    }
    if(opts.log_file) {
       set_log_file(opts.log_file); 
    }

    pcap_wrapper *pw;
    if (!(pw = pw_create(opts.device))) {
        logger(ERROR, "start captrue packet failed.\n");
        exit(0);
    }

    int ret;
    if(opts.server) {
        snprintf(filter, sizeof(filter), "host %s and tcp port %d", opts.server, opts.port);
    } else {
        snprintf(filter, sizeof(filter), "tcp port %d", opts.port);
    }

    ret = core_loop(pw, filter, process_packet);
    if(ret == -1) {
        logger(ERROR, "start core loop failed.\n");
    }

    pw_release(pw);
    script_release(L);

    return 0;
}
