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

#include <stdlib.h>
#include <string.h>

#include "redis.h"

request *parse_redis_request(char *payload, int size) {
    int n = 0, copy_size, buf_size, fied_index = 0;
    char *pos, *start, *end = payload+size;

    request *req = malloc(sizeof(request));
    buf_size = sizeof(req->buf)-1;
    if (payload[0] == '*') {
        start = payload;
        while(start+2 < end && (pos = strstr(start, "\r\n")) != NULL) {
            // skip the multi bulk len and bulk len
            if ((start[0] != '$' || fied_index % 2 != 1) && (start[0] != '*' || n != 0)) {
                if (n != 0) req->buf[n++] = ' ';
                copy_size = (pos-start) < (buf_size-n) ? pos-start : (buf_size-n);
                memcpy(req->buf + n, start, copy_size);
                n += (copy_size);
            }
            if (n == buf_size) break;
            start = pos+2;
            fied_index++;
        }
        req->buf[n] = '\0';
        if (n > 0) return req;
        free(req);
        return NULL;
    } else {
        pos = strstr(payload, "\r\n");
        if (pos) {
            copy_size = (pos-payload) < buf_size ? (pos-payload) : buf_size;
            memcpy(req->buf, payload, copy_size);
            req->buf[copy_size] = '\0';
            return req;
        }
    }
    free(req);
    return NULL;
}