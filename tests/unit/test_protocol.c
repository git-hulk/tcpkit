#include <stdlib.h>
#include "tk_test.h"
#include "protocol.h"

#define MAX_FORMATTED 127

static void test_redis_array_becomes_a_command_line(void) {
    const char *get = "*2\r\n$3\r\nGET\r\n$1\r\na\r\n";
    const char *set = "*3\r\n$3\r\nSET\r\n$1\r\nk\r\n$1\r\nv\r\n";
    char *out;

    out = format_redis(get, strlen(get));
    TK_EQ_STR(out, "GET a");
    free(out);

    out = format_redis(set, strlen(set));
    TK_EQ_STR(out, "SET k v");
    free(out);
}

static void test_redis_keeps_the_argument_order(void) {
    const char *cmd = "*4\r\n$4\r\nHSET\r\n$1\r\nh\r\n$1\r\nf\r\n$1\r\nv\r\n";
    char *out = format_redis(cmd, strlen(cmd));

    TK_EQ_STR(out, "HSET h f v");
    free(out);
}

static void test_raw_copies_the_payload(void) {
    char *out = format_raw("hello", 5);

    TK_EQ_STR(out, "hello");
    free(out);
}

static void test_raw_truncates_to_the_buffer_limit(void) {
    char payload[512];
    char *out;

    memset(payload, 'x', sizeof(payload));
    out = format_raw(payload, sizeof(payload));

    TK_EQ_INT(strlen(out), MAX_FORMATTED);
    free(out);
}

/* Copies into an exactly sized allocation: the payload a formatter gets from
 * libpcap has no NUL terminator, and here a scan past `size` hits a redzone. */
static char *unterminated(const char *literal, int size) {
    char *p = malloc(size);

    memcpy(p, literal, size);
    return p;
}

static void test_memcached_stops_at_the_payload_end(void) {
    char *payload = unterminated("GETNOCRLFATALL", 14);
    char *out = format_memcached(payload, 14);

    TK_EQ_STR(out, "GETNOCRLFATALL");
    free(out);
    free(payload);
}

static void test_redis_stops_at_the_payload_end(void) {
    /* A RESP array that ends mid-token, with no terminating CRLF. */
    char *payload = unterminated("*2\r\n$3\r\nGET", 11);
    char *out = format_redis(payload, 11);

    /* No complete argument to report, so the payload is summarised as is. */
    TK_EQ_STR(out, "*2..$3..GET");
    free(out);
    free(payload);
}

static void test_redis_handles_a_bare_array_marker(void) {
    char *payload = unterminated("*", 1);
    char *out = format_redis(payload, 1);

    TK_EQ_STR(out, "*");
    free(out);
    free(payload);
}

static void test_memcached_joins_lines_with_a_single_space(void) {
    const char *two = "a\r\nb\r\n";
    const char *one = "GET a\r\n";
    char *out;

    /* The separator used to be written one byte late, leaving the gap
     * uninitialised and the joined line carrying a stray byte. */
    out = format_memcached(two, strlen(two));
    TK_EQ_STR(out, "a b");
    free(out);

    out = format_memcached(one, strlen(one));
    TK_EQ_STR(out, "GET a");
    free(out);
}

static void test_memcached_keeps_the_last_character(void) {
    const char *cmd = "set foo 0 0 3\r\n";
    char *out = format_memcached(cmd, strlen(cmd));

    TK_EQ_STR(out, "set foo 0 0 3");
    free(out);
}

static void test_empty_payload_yields_an_empty_string(void) {
    char *out;

    /* buf[n - 1] with n == 0 used to write one byte before the allocation. */
    out = format_raw("", 0);
    TK_EQ_STR(out, "");
    free(out);

    out = format_memcached("", 0);
    TK_EQ_STR(out, "");
    free(out);

    out = format_redis("", 0);
    TK_EQ_STR(out, "");
    free(out);
}

static void test_redis_truncates_a_long_command(void) {
    char payload[512];
    char *out;
    int n;

    n = snprintf(payload, sizeof(payload), "*2\r\n$3\r\nGET\r\n$200\r\n");
    memset(payload + n, 'k', 200);
    n += 200;
    memcpy(payload + n, "\r\n", 2);
    n += 2;

    out = format_redis(payload, n);
    TK_CHECK(strlen(out) <= MAX_FORMATTED, "the formatted command must stay bounded");
    TK_CHECK(strncmp(out, "GET k", 5) == 0, "expected \"GET k...\", got \"%s\"", out);
    free(out);
}

static void test_raw_replaces_unprintable_bytes(void) {
    const char binary[] = { 0x00, 0x01, 'o', 'k', (char)0x80, (char)0xff };
    char *out = format_raw(binary, sizeof(binary));

    /* A NUL used to end the summary at the first byte. */
    TK_EQ_STR(out, "..ok..");
    free(out);
}

int main(void) {
    TK_RUN(test_redis_array_becomes_a_command_line);
    TK_RUN(test_redis_keeps_the_argument_order);
    TK_RUN(test_raw_copies_the_payload);
    TK_RUN(test_raw_truncates_to_the_buffer_limit);
    TK_RUN(test_raw_replaces_unprintable_bytes);
    TK_RUN(test_memcached_stops_at_the_payload_end);
    TK_RUN(test_redis_stops_at_the_payload_end);
    TK_RUN(test_redis_handles_a_bare_array_marker);
    TK_RUN(test_memcached_joins_lines_with_a_single_space);
    TK_RUN(test_memcached_keeps_the_last_character);
    TK_RUN(test_empty_payload_yields_an_empty_string);
    TK_RUN(test_redis_truncates_a_long_command);
    return tk_report("protocol");
}
