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

#ifndef TCPKIT_STATS_H
#define TCPKIT_STATS_H

#include <arpa/inet.h>
#include "cJSON.h"

#define LATENCY_BUCKETS 18

struct query_stats {
    uint64_t request_bytes;
    uint64_t response_bytes;
    uint64_t requests;
    uint64_t responses;
    uint64_t buckets[LATENCY_BUCKETS];

    struct in_addr ip;
    uint16_t port;
};

cJSON *create_stats_object(struct query_stats *stats);
void stats_incr(struct query_stats *stats, int is_request, int bytes); 
void stats_observer_latency(struct query_stats *stats, int64_t latency); 

#endif
