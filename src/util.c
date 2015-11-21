/*
 * ***************************************************************
 * author by @git-hulk at 2015-07-18 
 * Copyright (C) 2015 Inc.
 * *************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "util.h"

static char *log_file = NULL;
static enum LEVEL log_level = INFO;

void
set_log_level(enum LEVEL level) {
    log_level  = level;
}

void
set_log_file(char *filename)
{
    log_file = filename;
}

void
logger(enum LEVEL loglevel,char *fmt, ...)
{
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
        case INFO:  msg = "INFO";  color = C_YELLOW ; break;
        case WARN:  msg = "WARN";  color = C_PURPLE; break;
        case ERROR: msg = "ERROR"; color = C_RED; break;
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

    if(loglevel >= ERROR) {
        exit(1);
    }
}
