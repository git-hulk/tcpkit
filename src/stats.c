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

#include "stats.h"

int64_t latency_buckets[N_BUCKET] = {
    100, 200, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000, 3000000, 5000000,
    10000000, 20000000
};

const char *latency_buckets_name[N_BUCKET] = {
   "0.1ms", "0.2ms", "0.5ms", "1ms", "5ms", "10ms", "20ms", "50ms", "100ms", "200ms","500ms", "1s", "2s", "3s", "5s",
   "10s", "20s"
};

stats *stats_create(int n) {
    int i, j;

    stats *st = malloc(sizeof(*st));
    if (!st) return NULL;

    st->n_latency = n;
    st->req_packets = st->rsp_packets = 0;
    st->req_bytes = st->rsp_bytes = 0;
    st->latencies = malloc(n*sizeof(latency_stat));
    if (!st->latencies) return NULL;
    for (i = 0; i < n; i++) {
        st->latencies[i].total_reqs = 0;
        st->latencies[i].total_costs = 0;
        st->latencies[i].slow_counts = 0;
        for(j = 0; j < N_BUCKET; j++) {
           st->latencies[i].buckets[j] = 0;
        }
    }
    return st;
}

void stats_destroy(stats *st) {
   free(st->latencies);
   free(st);
}

void stats_update_bytes(stats *st, int req, uint64_t bytes) {
    if (req) {
        st->req_bytes += bytes;
        st->req_packets += 1;
    } else {
        st->rsp_bytes += bytes;
        st->rsp_packets += 1;
    }
}

void stats_incr_slow_count(stats *st, int ind) {
    st->latencies[ind].slow_counts++;
}

void stats_update_latency(stats *st, int ind, int64_t latency_us) {
    int i, n;
   st->latencies[ind].total_reqs++;
   st->latencies[ind].total_costs += latency_us;

   n = sizeof(latency_buckets)/ sizeof(latency_buckets[0]);
   for (i = 0; i < n-1; i++) {
        if (latency_buckets[i] >= latency_us) {
            st->latencies[ind].buckets[i]++;
            return;
        }
   }
    st->latencies[ind].buckets[n-1]++;
}