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

static void test_raw_keeps_binary_payloads_within_bounds(void) {
    const char binary[] = { 0x00, 0x01, 0x02, 0x7f, (char)0x80, (char)0xff };
    char *out = format_raw(binary, sizeof(binary));

    /* A NUL in the payload ends the C string, but the copy must stay in bounds. */
    TK_CHECK(out != NULL, "format_raw must allocate");
    TK_EQ_INT(strlen(out), 0);
    free(out);
}

int main(void) {
    TK_RUN(test_redis_array_becomes_a_command_line);
    TK_RUN(test_redis_keeps_the_argument_order);
    TK_RUN(test_raw_copies_the_payload);
    TK_RUN(test_raw_truncates_to_the_buffer_limit);
    TK_RUN(test_raw_keeps_binary_payloads_within_bounds);
    return tk_report("protocol");
}
