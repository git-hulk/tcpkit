#include "cJSON.h"
#include "stats.h"

int64_t latency_buckets[] = {
    100, 200, 500, 1000, 5000,
    10000, 20000, 50000, 100000,
    200000, 500000, 1000000, 2000000,
    3000000, 5000000, 10000000, 20000000
};

const char *latency_buckets_name[] = {
   "<0.1ms", "0.1ms~0.2ms", "0.2~0.5ms",
   "0.5ms~1ms", "1ms~5ms", "5ms~10ms", "10ms~20ms",
   "20ms~50ms", "50ms~100ms", "100ms~200ms","200ms~500ms",
   "0.5s~1s", "1s~2s", "2s~3s", "3s~5s",
   "5s~10s", "10s~20s", "+inf"
};

cJSON *create_stats_object(struct query_stats *stats) {
    int i, n;
    cJSON *stats_object, *bucket_object, *latency_object;

    stats_object = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats_object, "requests", (double)stats->requests);
    cJSON_AddNumberToObject(stats_object, "request_bytes", (double)stats->request_bytes);
    cJSON_AddNumberToObject(stats_object, "responses", (double)stats->responses);
    cJSON_AddNumberToObject(stats_object, "response_bytes", (double)stats->response_bytes);

    latency_object = cJSON_CreateArray();
    n = sizeof(latency_buckets)/ sizeof(latency_buckets[0]);
    for (i = 0; i < n; i++) {
        if (stats->buckets[i]) {
            bucket_object = cJSON_CreateObject();
            cJSON_AddNumberToObject(bucket_object, latency_buckets_name[i], stats->buckets[i]);
            cJSON_AddItemToArray(latency_object, bucket_object);    
        }
    }
    cJSON_AddItemToObject(stats_object, "latency", latency_object);    
    return stats_object;
}

void stats_incr(struct query_stats *stats, int is_request, int bytes) {
    if (is_request) {
        stats->requests += 1;
        stats->request_bytes += bytes;
    } else {
        stats->responses += 1;
        stats->response_bytes += bytes;
    }
}

void stats_observer_latency(struct query_stats *stats, int64_t latency) {
    int i, n;

    if (latency < 0) latency = 0;
    n = sizeof(latency_buckets)/ sizeof(latency_buckets[0]);
    for (i = 0; i < n-1; i++) {
        if (latency_buckets[i] >= latency) {
            stats->buckets[i]++;
            return;
        }
    }
    stats->buckets[n-1]++;
}
