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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "logger.h"

static char *log_file = NULL;
static enum LEVEL log_level = INFO;

void set_log_level(enum LEVEL level) {
    log_level  = level;
}

void set_log_file(char *filename) {
    if (filename) log_file = filename;
}

void rlog(char *fmt, ...) {
    FILE *fp;
    va_list ap;
    char buf[4096];

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    fp = (log_file == NULL) ? stdout : fopen(log_file,"a");
    fprintf(fp, "%s\n", buf);
    if(log_file) {
        fclose(fp);
    }
}

void alog(enum LEVEL loglevel, char *fmt, ...) {
    FILE *fp;
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
    fp = (log_file == NULL) ? stdout : fopen(log_file,"a");
    if(log_file) {
        fprintf(fp, "[%s] [%s] %s\n", t_buf, msg, buf);
        fclose(fp);
    } else {
        fprintf(fp, "%s[%s] [%s] %s"C_NONE"\n", color, t_buf, msg, buf);
    }
    if(loglevel > ERROR) {
        exit(1);
    }
}