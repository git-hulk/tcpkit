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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util.h"
#include "array.h"

struct array *split_string(char *input, char delim)
{
    char *start, *p, *end, *token, *elem;
    struct array *tokens;

    if (!input) return NULL;
    tokens = array_alloc(sizeof(char *),  5);
    if (!tokens) return NULL;
    start = input;
    end = input + strlen(input)-1;
    while((p = strchr(start, delim)) != NULL) {
        if (p == start) {
            start = p + 1;
            continue;
        }
        *p = '\0';
        token = malloc(p-start+2);
        memcpy(token, start, p-start+1);
        token[p-start+1] = '\0';
        elem = array_push(tokens);
        memcpy(elem, &token, sizeof(char *));
        start = p+1;
    }
    if (start < end) {
        token = malloc(end-start+2);
        memcpy(token, start, end-start+1);
        token[end-start+1] = '\0';
        elem = array_push(tokens);
        memcpy(elem, &token, sizeof(char *));
    }
    return tokens;
}

void free_split_string(struct array *arr) {
    int i;
    char *token;

    for (i = 0; i < array_used(arr); i++) {
        token = *(char **)array_pos(arr, i);
        free(token);
    }
    array_dealloc(arr);
}

struct array *get_addresses_from_device() {
    struct ifaddrs *if_addrs, *ifa;
    struct in_addr in_addr;
    struct array *addrs;
    char *elem;

    if (getifaddrs(&if_addrs) == -1) {
        return NULL;
    }
    addrs = array_alloc(sizeof(struct in_addr), 5);
    for (ifa = if_addrs; ifa; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) continue;
        if (ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6) {
            in_addr = ((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
            if (!in_addr.s_addr) continue;
            elem = array_push(addrs);
            memcpy(elem, &in_addr, sizeof(struct in_addr));
        }
    }
    freeifaddrs(if_addrs);
    return addrs;
}
