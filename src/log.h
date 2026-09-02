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

#ifndef TCPKIT_LOG_H
#define TCPKIT_LOG_H

#include <stdio.h>
#include <stdlib.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define PURPLE "\033[35m"
#define NONE "\033[0m"

enum LEVEL {
    DEBUG = 1,
    INFO,
    WARN,
    ERROR,
    FATAL
};

#if defined(__GNUC__)
#define TK_PRINTF_FMT(a, b) __attribute__((format(printf, a, b)))
#else
#define TK_PRINTF_FMT(a, b)
#endif

void color_printf(const char *color, const char *fmt, ...) TK_PRINTF_FMT(2, 3);
void print_redirect(FILE *fp);
void log_message(enum LEVEL loglevel, const char *fmt, ...) TK_PRINTF_FMT(2, 3);

#endif
