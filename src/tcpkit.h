#ifndef _TCPKIT_H_
#define _TCPKIT_H_
#define LUA_COMPAT_MODULE
#include <stdint.h>
#include <netinet/in.h>

#include "script.h"
#include "util.h"
#include "array.h"
#include "pcap_wrapper.h"

#define VERSION "0.1.0"
#define DEFAULT_CALLBACK "process_packet"

struct options {
    int tcp;
    int udp;
    int port;
    int duration;
    int is_usage;
    int show_version;
    int is_calc_mode;
    int is_client_mode;
    char *server;
    char *device;
    char *script;
    char *log_file;
    char *offline_file;
    char *local_addresses;
    char *save_file;
};

struct stats *get_stats();
struct options *get_options();
struct lua_State * get_lua_vm();
int is_local_address(struct in_addr addr);
#endif
