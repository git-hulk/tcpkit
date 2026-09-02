#include <stdlib.h>
#include "tk_test.h"
#include "stats.h"

static void test_incr_separates_requests_and_responses(void) {
    struct query_stats stats;

    memset(&stats, 0, sizeof(stats));
    stats_incr(&stats, 1, 100);
    stats_incr(&stats, 1, 40);
    stats_incr(&stats, 0, 900);

    TK_EQ_INT(stats.requests, 2);
    TK_EQ_INT(stats.request_bytes, 140);
    TK_EQ_INT(stats.responses, 1);
    TK_EQ_INT(stats.response_bytes, 900);
}

static uint64_t bucket_of(int64_t latency_us) {
    struct query_stats stats;
    int i;

    memset(&stats, 0, sizeof(stats));
    stats_observer_latency(&stats, latency_us);
    for (i = 0; i < LATANCY_BUCKETS; i++) {
        if (stats.buckets[i]) return (uint64_t)i;
    }
    return (uint64_t)-1;
}

static void test_latency_lands_in_the_matching_bucket(void) {
    TK_EQ_INT(bucket_of(50), 0);        /* <0.1ms   */
    TK_EQ_INT(bucket_of(100), 0);       /* boundary */
    TK_EQ_INT(bucket_of(150), 1);       /* 0.1~0.2  */
    TK_EQ_INT(bucket_of(1500), 4);      /* 1ms~5ms  */
    TK_EQ_INT(bucket_of(615), 3);       /* 0.5ms~1ms */
    TK_EQ_INT(bucket_of(2000000), 12);  /* 1s~2s    */
}

static void test_negative_latency_is_clamped(void) {
    TK_EQ_INT(bucket_of(-1), 0);
}

static void test_stats_object_reports_the_counters(void) {
    struct query_stats stats;
    cJSON *object;
    char *json;

    memset(&stats, 0, sizeof(stats));
    stats_incr(&stats, 1, 184100);
    stats_incr(&stats, 0, 1413764);
    stats_observer_latency(&stats, 615);

    object = create_stats_object(&stats);
    json = cJSON_Print(object);

    TK_CHECK(strstr(json, "\"requests\"") != NULL, "requests must be reported");
    TK_CHECK(strstr(json, "\"response_bytes\"") != NULL, "response_bytes must be reported");
    TK_CHECK(strstr(json, "0.5ms~1ms") != NULL, "the observed bucket must be named");

    free(json);
    cJSON_Delete(object);
}

int main(void) {
    TK_RUN(test_incr_separates_requests_and_responses);
    TK_RUN(test_latency_lands_in_the_matching_bucket);
    TK_RUN(test_negative_latency_is_clamped);
    TK_RUN(test_stats_object_reports_the_counters);
    return tk_report("stats");
}
