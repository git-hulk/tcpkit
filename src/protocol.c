#include <stdlib.h>
#include <string.h>
#include "protocol.h"

const int MAX_BUF_SIZE = 127;

char *format_redis(const char *payload, int size) {
    int n = 0, copy_size, buf_size, fields = 0;
    const char *start;
    char *buf, *pos;
    
    if (payload[0] == '*') {
        buf_size = size > MAX_BUF_SIZE ? MAX_BUF_SIZE : size;
        buf = malloc(buf_size+1);
        start = payload;
        while((pos = strstr(start, "\r\n")) != NULL) {
            if (fields != 0 && fields % 2 == 0) {
                copy_size = pos-start < buf_size - n ? pos-start : buf_size-n;
                memcpy(buf+n, start, copy_size);
                n += copy_size;
                if (n >= buf_size) break;
                buf[n++] = ' ';
            }
            if (pos-payload+2 >= size) break;
            start = pos+2;
            fields++;
        }
        if (n == 0) {
            memcpy(buf, payload, buf_size);
            n = buf_size;
        }
        buf[n-1] = '\0';
        return buf;
    }
    return format_memcached(payload, size);
}

char *format_memcached(const char *payload, int size) {
    int n = 0, copy_size, buf_size;
    char *pos, *buf;
    const char *start;

    buf_size = size > MAX_BUF_SIZE ? MAX_BUF_SIZE : size;
    buf = malloc(buf_size+1);
    start = payload;
    while ((pos = strstr(start, "\r\n")) != NULL) {
        copy_size = pos-start < buf_size-n ? pos-start : buf_size-n;
        memcpy(buf+n, start, copy_size);
        n += copy_size;
        if (n >= buf_size) break;
        buf[++n] = ' ';
        if (pos-payload+2 >= size) break;
        start = pos+2;
    }
    if (n == 0) {
        memcpy(buf, payload, buf_size);
        n = buf_size;
    }
    buf[n-1] = '\0';
    return buf;
}

char *format_http(const char *payload, int size) {
    return format_memcached(payload, size);
}

char *format_raw(const char *payload, int size) {
    char*buf;
    int buf_size;
    buf_size = size > MAX_BUF_SIZE ? MAX_BUF_SIZE : size;

    buf = malloc(buf_size+1);
    memcpy(buf, payload, buf_size);
    buf[buf_size] = '\0';
    return buf;
}
