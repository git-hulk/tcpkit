#ifndef _TCPKIT_H_
#define _TCPKIT_H_

#define LUA_COMPAT_MODULE

#include "packet_capture.h"
#include "script.h"
#include "util.h"

#define DEFAULT_CALLBACK "process_packet"

struct tk_options {
    char *server;
    char *device;
    char *script;
    char *log_file;
    int port;
    int specified_addresses; 
};

lua_State *get_lua_vm(); 
struct tk_options *get_global_options();
int is_client_mode();
#endif
