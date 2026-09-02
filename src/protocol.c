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

#include <stdlib.h>
#include <string.h>
#include "protocol.h"

const int MAX_BUF_SIZE = 127;

/* The payload points into the capture buffer and is not NUL terminated, so
 * every scan below is bounded by the size the caller reported. */

struct buf {
    char *data;
    int len;
    int cap;    /* excludes the terminating NUL */
};

static int buf_init(struct buf *b, int size) {
    b->cap = size > MAX_BUF_SIZE ? MAX_BUF_SIZE : size;
    if (b->cap < 0) b->cap = 0;
    b->len = 0;
    b->data = malloc(b->cap + 1);
    return b->data != NULL;
}

/* The result is a single summary line, so bytes that would break it -- a
 * newline from a truncated request, a NUL from a binary value -- become dots. */
static void buf_append(struct buf *b, const char *src, int n) {
    int i, room = b->cap - b->len;
    unsigned char c;

    if (n > room) n = room;
    for (i = 0; i < n; i++) {
        c = (unsigned char)src[i];
        b->data[b->len++] = (c >= 0x20 && c < 0x7f) ? (char)c : '.';
    }
}

static const char *find_crlf(const char *start, const char *end) {
    for (; start + 1 < end; start++) {
        if (start[0] == '\r' && start[1] == '\n') return start;
    }
    return NULL;
}

char *format_redis(const char *payload, int size) {
    struct buf b;
    const char *start, *end, *pos;
    int fields = 0;

    if (size <= 0) return format_raw(payload, size);
    if (payload[0] != '*') return format_memcached(payload, size);
    if (!buf_init(&b, size)) return NULL;

    /* A RESP array alternates a $length line with the argument itself. */
    start = payload;
    end = payload + size;
    while ((pos = find_crlf(start, end)) != NULL) {
        if (fields != 0 && fields % 2 == 0) {
            if (b.len > 0) buf_append(&b, " ", 1);
            buf_append(&b, start, (int)(pos - start));
            if (b.len >= b.cap) break;
        }
        start = pos + 2;
        fields++;
    }
    if (b.len == 0) buf_append(&b, payload, size);
    b.data[b.len] = '\0';
    return b.data;
}

char *format_memcached(const char *payload, int size) {
    struct buf b;
    const char *start, *end, *pos;

    if (!buf_init(&b, size)) return NULL;

    start = payload;
    end = payload + size;
    while ((pos = find_crlf(start, end)) != NULL) {
        if (b.len > 0) buf_append(&b, " ", 1);
        buf_append(&b, start, (int)(pos - start));
        if (b.len >= b.cap) break;
        start = pos + 2;
    }
    if (b.len == 0) buf_append(&b, payload, size);
    b.data[b.len] = '\0';
    return b.data;
}

char *format_http(const char *payload, int size) {
    return format_memcached(payload, size);
}

char *format_raw(const char *payload, int size) {
    struct buf b;

    if (!buf_init(&b, size)) return NULL;
    buf_append(&b, payload, size);
    b.data[b.len] = '\0';
    return b.data;
}
