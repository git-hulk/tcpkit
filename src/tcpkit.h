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

#ifndef TCPKIT_TCPKIT_H
#define TCPKIT_TCPKIT_H

#define MAX_ERR_BUFF_SIZE 256
typedef enum {
    ProtocolRaw = 1,
    ProtocolRedis,
    ProtocolMemcached,
    ProtocolHTTP,
}ProtocolType;

struct options {
    char *dev;
    char *filter;
    char *script;
    char *offline_file;
    char *save_file;
    int snaplen;
    int buf_size;
    int threshold;
    int print_version;
    int print_usage;
    int stats_port;
    ProtocolType protocol;
};

#endif
