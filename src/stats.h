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

#include <stdint.h>
#include <circllhist.h>

#define N_BUCKET 18

typedef struct {
    int64_t total_reqs;
    int64_t total_costs;
    int64_t slow_counts;
    int64_t buckets[N_BUCKET];
    histogram_t *lathist;
}latency_stat;

typedef struct {
    int64_t req_bytes;
    int64_t rsp_bytes;
    int64_t req_packets;
    int64_t rsp_packets;
    latency_stat *latencies;
    int n_latency;
}stats;

extern int64_t latency_buckets[N_BUCKET];
extern const char *latency_buckets_name[N_BUCKET];

stats *stats_create(int n);
void stats_destroy(stats *st);
void stats_update_bytes(stats *st, int req, uint64_t bytes);
void stats_incr_slow_count(stats *st, int ind);
void stats_update_latency(stats *st, int ind, int64_t latency_us);
#endif //TCPKIT_STATS_H
