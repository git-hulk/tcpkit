#ifndef _TCPKIT_H_
#define _TCPKIT_H_

#define LUA_COMPAT_MODULE

#include <stdint.h>
#include "pcap_wrapper.h"
#include "script.h"
#include "util.h"
#include "array.h"

#define VERSION "0.1.0"
#define DEFAULT_CALLBACK "process_packet"

struct tk_options {
    char *server;
    char *device;
    char *script;
    char *log_file;
    char *offline_file;
    int port;
    int specified_addresses; 
    int is_calc_mode;
    int duration;
    struct array *local_addresses;
};

struct bandwidth {
    uint64_t in_bytes;
    uint64_t in_packets;
    uint64_t out_bytes;
    uint64_t out_packets;
    uint64_t last_calc_time;
};

lua_State *get_lua_vm(); 
struct tk_options *get_global_options();
struct bandwidth *get_global_bandwidth();
void need_report_bandwidth();
int is_client_mode();
#endif
