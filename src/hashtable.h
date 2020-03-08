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

#ifndef TCPKIT_HASHTABLE_H
#define TCPKIT_HASHTABLE_H

typedef struct entry {
    char *key;
    void *value;
    struct entry *next;
} entry;

typedef struct hashtable {
    int nbucket;
    entry **buckets;
    void (*free)(void *);
} hashtable;

hashtable *hashtable_create(int nbucket);
void hashtable_destroy(hashtable *ht);
void *hashtable_get(hashtable *ht, char *key);
void *hashtable_add(hashtable *ht, char *key, void *value);
int hashtable_del(hashtable *ht, char *key);
void **hashtable_values(hashtable *ht, int *cnt);
#endif
