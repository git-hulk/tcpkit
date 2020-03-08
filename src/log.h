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

void raw_printf(char *fmt, ...); 
void color_printf(const char *color, char *fmt, ...); 
void print_redirect(FILE *fp);
void log_message(enum LEVEL loglevel, char *fmt, ...); 

#endif
