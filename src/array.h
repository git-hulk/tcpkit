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

#ifndef TCPKIT_ARRAY_H
#define TCPKIT_ARRAY_H
struct array {
    int size;
    int used;
    int entry_size;
    char entries[0];
};

struct array *array_alloc(int entry_size, int size);
int array_used(struct array *arr);
char *array_push(struct array *arr);
char *array_pos(struct array *arr, int pos);
void array_dealloc(struct array *arr);

#endif //TCPKIT_ARRAY_H
