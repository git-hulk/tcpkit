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

#ifndef TCPKIT_PRTOCOL_H
#define TCPKIT_PRTOCOL_H

/* Each returns a freshly allocated, NUL terminated summary line of at most
 * MAX_BUF_SIZE printable characters, or NULL when out of memory. The payload
 * itself is not required to be NUL terminated. */
char *format_redis(const char *payload, int size);
char *format_memcached(const char *payload, int size);
char *format_http(const char *payload, int size);
char *format_raw(const char *payload, int size);

#endif
