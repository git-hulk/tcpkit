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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "array.h"

struct array *split_string(char *input, char delim)
{
    char *start, *p, *end, *token, *elem;
    struct array *tokens;

    if (!input) return NULL;
    tokens = array_alloc(sizeof(char *),  5);
    if (!tokens) return NULL;
    start = input;
    end = input + strlen(input)-1;
    while((p = strchr(start, delim)) != NULL) {
        if (p == start) {
            start = p + 1;
            continue;
        }
        *p = '\0';
        token = malloc(p-start+2);
        memcpy(token, start, p-start+1);
        token[p-start+1] = '\0';
        elem = array_push(tokens);
        memcpy(elem, &token, sizeof(char *));
        start = p+1;
    }
    if (start < end) {
        token = malloc(end-start+2);
        memcpy(token, start, end-start+1);
        token[end-start+1] = '\0';
        elem = array_push(tokens);
        memcpy(elem, &token, sizeof(char *));
    }
    return tokens;
}

void free_split_string(struct array *arr) {
    int i;
    char *token;

    for (i = 0; i < array_used(arr); i++) {
        token = *(char **)array_pos(arr, i);
        free(token);
    }
    array_dealloc(arr);
}