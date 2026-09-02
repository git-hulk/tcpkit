#include "cJSON.h"
#include "stats.h"

/* Upper bound in microseconds, inclusive; the last bucket catches the rest. */
static const struct {
    int64_t upper_us;
    const char *name;
} latency_buckets[] = {
    {      100, "<0.1ms"      },
    {      200, "0.1ms~0.2ms" },
    {      500, "0.2~0.5ms"   },
    {     1000, "0.5ms~1ms"   },
    {     5000, "1ms~5ms"     },
    {    10000, "5ms~10ms"    },
    {    20000, "10ms~20ms"   },
    {    50000, "20ms~50ms"   },
    {   100000, "50ms~100ms"  },
    {   200000, "100ms~200ms" },
    {   500000, "200ms~500ms" },
    {  1000000, "0.5s~1s"     },
    {  2000000, "1s~2s"       },
    {  3000000, "2s~3s"       },
    {  5000000, "3s~5s"       },
    { 10000000, "5s~10s"      },
    { 20000000, "10s~20s"     },
    {        0, "+inf"        }
};

/* Fails to compile if the table and the counter array fall out of step. */
typedef char latency_bucket_table_check[
    (sizeof(latency_buckets) / sizeof(latency_buckets[0]) == LATENCY_BUCKETS) ? 1 : -1];

cJSON *create_stats_object(struct query_stats *stats) {
    int i;
    cJSON *stats_object, *bucket_object, *latency_object;

    stats_object = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats_object, "requests", (double)stats->requests);
    cJSON_AddNumberToObject(stats_object, "request_bytes", (double)stats->request_bytes);
    cJSON_AddNumberToObject(stats_object, "responses", (double)stats->responses);
    cJSON_AddNumberToObject(stats_object, "response_bytes", (double)stats->response_bytes);

    latency_object = cJSON_CreateArray();
    for (i = 0; i < LATENCY_BUCKETS; i++) {
        if (stats->buckets[i]) {
            bucket_object = cJSON_CreateObject();
            cJSON_AddNumberToObject(bucket_object, latency_buckets[i].name, stats->buckets[i]);
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
    int i;

    if (latency < 0) latency = 0;
    for (i = 0; i < LATENCY_BUCKETS - 1; i++) {
        if (latency_buckets[i].upper_us >= latency) {
            stats->buckets[i]++;
            return;
        }
    }
    stats->buckets[LATENCY_BUCKETS - 1]++;
}
