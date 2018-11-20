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

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "logger.h"

static enum LEVEL log_level = INFO;
static FILE *log_fp = NULL;

void set_log_fp(FILE *fp) {
    log_fp = fp;
}

void rlog(char *fmt, ...) {
    va_list ap;
    char buf[4096];

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    fprintf(log_fp, "%s\n", buf);
    fflush(log_fp);
}

void alog(enum LEVEL loglevel, char *fmt, ...) {
    va_list ap;
    time_t now;
    char buf[4096];
    char t_buf[64];
    char *msg = NULL;
    const char *color = "";

    if(loglevel < log_level) {
        return;
    }

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    switch(loglevel) {
        case DEBUG: msg = "DEBUG"; break;
        case INFO:  msg = "INFO";  color = C_GREEN; break;
        case WARN:  msg = "WARN";  color = C_YELLOW; break;
        case ERROR: msg = "ERROR"; color = C_RED; break;
        case FATAL: msg = "FATAL"; color = C_RED; break;
    }

    now = time(NULL);
    strftime(t_buf,64,"%Y-%m-%d %H:%M:%S",localtime(&now));
    if(log_fp != stdout) {
        fprintf(log_fp, "[%s] [%s] %s\n", t_buf, msg, buf);
    } else {
        fprintf(log_fp, "%s[%s] [%s] %s"C_NONE"\n", color, t_buf, msg, buf);
    }
    fflush(log_fp);
    if(loglevel > ERROR) {
        exit(1);
    }
}