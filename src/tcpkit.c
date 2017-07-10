#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tcpkit.h"
#include "packet.h"
#include "bandwidth.h"
#include "local_addresses.h"


struct server {
    char *filter;
    lua_State *vm;
    struct options *opts;
    struct bandwidth *bw;
    struct array *local_addrs;
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

struct bandwidth *
get_bandwidth()
{
    return srv.bw;
}

int
is_client_mode ()
{
    return srv.opts->server == NULL ? 0 : 1;
}

static void
usage(char *prog)
{
    fprintf(stderr, "%s is a tool to capature tcp packets, and you can analyze packets with lua\n", prog);
    fprintf(stderr, "\t-s server ip\n");
    fprintf(stderr, "\t-p port\n");
    fprintf(stderr, "\t-i device\n");
    fprintf(stderr, "\t-r offline file\n");
    fprintf(stderr, "\t-S lua script path, default is ../scripts/example.lua\n");
    fprintf(stderr, "\t-l local address\n");
    fprintf(stderr, "\t-C calculate bandwidth mode\n");
    fprintf(stderr, "\t-d duration, take effect when -C is set\n");
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
    while((ch = getopt(argc, argv, "s:p:i:S:Cd:l:r:uthv")) != -1) {
        switch(ch) {
            case 's': opts->server = strdup(optarg); break;
            case 'r': opts->offline_file = strdup(optarg); break;
            case 'p': opts->port= atoi(optarg); break;
            case 'i': opts->device = strdup(optarg); break;
            case 'S': opts->script = strdup(optarg); break;
            case 'C': opts->is_calc_mode = 1; break;
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
    return opts;
}

static lua_State *
create_lua_vm(const char *filename) {
    int ret;
    lua_State *vm;

    vm = script_init_vm();
    if(filename && access(filename, R_OK) == 0) {
        ret = luaL_dofile(vm, filename);
    } else {
        char *chunk = "\
        function process_packet(item) \
            if item.len > 0 then \
                local time_str = os.date('%Y-%m-%d %H:%M:%S', item.tv_sec)..'.'..item.tv_usec\
                local network_str = item.src .. ':' .. item.sport .. '=>' .. item.dst .. ':' .. item.dport \
                print(time_str, network_str, item.len, item.payload) \
            end \
        end \
        ";
        ret = luaL_dostring(vm, chunk);
    }
    if (!ret) { // load script file or chunk succ
        if (!script_check_func_exists(vm, DEFAULT_CALLBACK)) {
            logger(ERROR,"%s\n", "function process_packet was not found");
            return NULL;
        }
        return vm;
    }
    logger(ERROR,"%s", lua_tostring(vm, -1));
    return NULL;
}

static char *
create_filter(struct options *opts) {
    char *protocol = "", *filter;
    int n;

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
        n = snprintf(filter, n, "");
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

int
main(int argc, char **argv)
{
    lua_State *vm;
    struct options *opts;
    struct array *local_addrs;
    pcap_wrapper *wrapper;

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
    vm = create_lua_vm(opts->script);
    if (!vm && !opts->is_calc_mode) {
        // TODO: log error message
    }
    local_addrs = get_local_addresses(opts->local_addresses);
    if(array_used(local_addrs) <= 0) {
        logger(ERROR, "You must run with sudo, or use -l option to set local address\n");
        exit(0);
    }
    if (opts->offline_file) {
        wrapper = pw_create_offline(opts->offline_file);
    } else {
        wrapper = pw_create(opts->device);
    }
    if (!wrapper) {
        logger(ERROR, "You must assign device use -i and swith to root\n");
        exit(0);
    }

    srv.vm = vm;
    srv.opts = opts;
    srv.local_addrs = local_addrs;
    srv.filter = create_filter(opts);
    if(core_loop(wrapper, srv.filter, process_packet)  == -1) {
        logger(ERROR, "Start core loop failed, err %s\n", pcap_geterr(wrapper->pcap));
    }

    // FIXME: free options
    pw_release(wrapper);
    script_release(srv.vm);
    return 0;
}
