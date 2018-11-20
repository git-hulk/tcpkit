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

#ifndef TCPKIT_LOGGER_H
#define TCPKIT_LOGGER_H

#include <stdio.h>
#include <stdlib.h>

#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_PURPLE "\033[35m"
#define C_NONE "\033[0m"
#define GB (1024*1024*1024)

enum LEVEL {
    DEBUG = 1,
    INFO,
    WARN,
    ERROR,
    FATAL
};

void rlog(char *fmt, ...);
void alog(enum LEVEL loglevel, char *fmt, ...);
void set_log_fp(FILE *fp);

#endif //TCPKIT_LOGGER_H
