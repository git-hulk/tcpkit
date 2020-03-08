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

#ifndef TCPKIT_STATS_H
#define TCPKIT_STATS_H

#include <arpa/inet.h>
#include "cJSON.h"

#define LATANCY_BUCKETS 18 

struct query_stats {
    uint64_t request_bytes;
    uint64_t response_bytes;
    uint64_t requests;
    uint64_t responses;
    uint64_t buckets[LATANCY_BUCKETS]; 

    struct in_addr ip;
    uint16_t port;
};

cJSON *create_stats_object(struct query_stats *stats);
void stats_incr(struct query_stats *stats, int is_request, int bytes); 
void stats_observer_latency(struct query_stats *stats, int64_t latency); 

#endif
