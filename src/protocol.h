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
