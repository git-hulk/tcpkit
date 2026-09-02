/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   SPDX-License-Identifier: MIT
 *
 *   Use of this source code is governed by the MIT license that can be found
 *   in the LICENSE file at the root of this repository.
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
    int ascii;
    ProtocolType protocol;
};

#endif
