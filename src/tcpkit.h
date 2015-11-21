#ifndef _TCPKIT_H_
#define _TCPKIT_H_

#define LUA_COMPAT_MODULE

#include "packet_capture.h"
#include "script.h"
#include "util.h"

#define DEFAULT_CALLBACK "process_packet"
//extern pcap_wrapper *pw;
extern  lua_State *L; 
#endif
