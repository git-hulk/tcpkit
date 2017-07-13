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
    if (filename) log_file = filename;
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

int speed_human(uint64_t speed, char *buf, int size)
{
    int n;

    if (!buf || size < 16) return -1;
    if (speed > GB) {
        n = snprintf(buf, size, "%.2f GB/s", (speed * 1.0)/GB);
    } else if (speed > MB) {
        n = snprintf(buf, size, "%.2f MB/s", (speed * 1.0)/MB);
    } else if (speed > KB) {
        n = snprintf(buf, size, "%.2f KB/s", (speed * 1.0)/KB);
    } else {
        n = snprintf(buf, size, "%llu B/s", (unsigned long long)speed);
    }
    buf[n] = '\0';
    return 0;
}

#ifdef TEST
int main () {

    char buf[16];

    speed_human((uint64_t)512*1024*1024*1024, buf, 16);
    printf("%s\n", buf);
    speed_human((uint64_t)512*1023*1024, buf, 16);
    printf("%s\n", buf);
    speed_human((uint64_t)3033, buf, 16);
    printf("%s\n", buf);
    speed_human((uint64_t)123, buf, 16);
    printf("%s\n", buf);
    return 0;
}
#endif
