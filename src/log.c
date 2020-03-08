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

#include "log.h"

static FILE *log_fp = NULL;
static enum LEVEL log_level = INFO;

void print_redirect(FILE *fp) {
    log_fp = fp;
}

void raw_printf(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    color_printf(NULL, fmt, args);
    va_end(args);
}

void color_printf(const char *color, char *fmt, ...) {
    va_list ap;
    char buf[4096];

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if(log_fp != stdout || color == NULL) {
        fprintf(log_fp, "%s",  buf);
        fflush(log_fp);
    } else {
        fprintf(log_fp, "%s%s"NONE"", color, buf);
    }
}

void log_message(enum LEVEL loglevel, char *fmt, ...) {
    va_list ap;
    time_t now;
    char buf[4096];
    char t_buf[64];
    char *msg = NULL;
    const char *color = "";

    if(loglevel < log_level) return; 

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    switch(loglevel) {
        case DEBUG: msg = "DEBUG"; break;
        case INFO:  msg = "INFO";  color = GREEN; break;
        case WARN:  msg = "WARN";  color = YELLOW; break;
        case ERROR: msg = "ERROR"; color = RED; break;
        case FATAL: msg = "FATAL"; color = RED; break;
    }
    now = time(NULL);
    strftime(t_buf,64,"%Y-%m-%d %H:%M:%S",localtime(&now));
    if(log_fp != stdout) {
        fprintf(log_fp, "[%s] [%s] %s\n", t_buf, msg, buf);
        fflush(log_fp);
    } else {
        fprintf(log_fp, "%s[%s] [%s] %s"NONE"\n", color, t_buf, msg, buf);
    }
    if(loglevel > ERROR) {
        exit(1);
    }
}
