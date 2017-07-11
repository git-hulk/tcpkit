/**
 *   tcprstat -- Extract stats about TCP response times
 *   Copyright (C) 2010  Ignacio Nin
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
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
**/

#include <pcap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util.h"
#include "array.h"


static struct array*
get_addresses_from_device(void) {
    char *elem;
    struct array *addrs;
    struct in_addr in_addr;
    struct pcap_addr *addr;
    pcap_if_t *devlist, *dev;
    struct sockaddr *real_addr;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&devlist, errbuf) || !devlist) {
        return NULL; // failed to get device list
    }
    addrs = array_alloc(sizeof(struct in_addr), 10);
    for (dev = devlist; dev; dev = dev->next) {
        //if (dev->flags & PCAP_IF_LOOPBACK) continue;
        for (addr = dev->addresses; addr; addr = addr->next) {
            if (!addr->addr && !addr->dstaddr) continue;
            real_addr = addr->addr ? addr->addr : addr->dstaddr;
            if (real_addr->sa_family == AF_INET || real_addr->sa_family == AF_INET6) {
                in_addr = ((struct sockaddr_in *) real_addr)->sin_addr;
                elem = array_push(addrs);
                memcpy(elem, &in_addr, sizeof(struct in_addr));
            }
        }
    }
    pcap_freealldevs(devlist);
    return addrs;
}

static struct array*
get_addresses_from_string(char *addrs_str) {
    int n, len;
    struct array *addrs;
    struct in_addr in_addr;
    char *pos, *address, *start, *elem;
    
    if (!addrs_str || strlen(addrs_str) <= 0) return NULL;

    len = strlen(addrs_str);
    start = addrs_str;
    addrs = array_alloc(sizeof(struct in_addr), 10);
    while((pos = strchr(start, ',')) != NULL || start < addrs_str + len) {
        if (!pos) { // last part in ip list string
            pos = addrs_str + len;
        }
        n = pos - start;
        address = malloc(n + 1); // reversed 1 byte for '\0'
        // FIXME: out of memory
        strncpy(address, start, n);
        address[n] = '\0';
        if (!inet_aton(address, &in_addr)) { // string to inet adress
            free(address);
            continue;
        }
        elem = array_push(addrs);
        memcpy(elem, &in_addr, sizeof(struct in_addr));
        start = pos + 1;
    }
    return addrs;
}

void
dump_local_addresses(struct array *addrs) {
    int i;
    char *elem, *addr_str;

    if (!addrs) return;
    printf("local addresses list: ");
    for (i = 0; i < array_used(addrs); i++) {
        elem = array_pos(addrs, i);
        if ((addr_str = inet_ntoa(*(struct in_addr*)elem))) {
            printf("%s\t", addr_str);
        }
    }
    printf("\n");
}

struct array*
get_local_addresses(char *addrs_str) {
    struct array *addrs;

    addrs = get_addresses_from_string(addrs_str);
    if (array_used(addrs) > 0) {
        return addrs;
    }
    return get_addresses_from_device();
}
