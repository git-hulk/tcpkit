/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   SPDX-License-Identifier: MIT
 *
 *   Use of this source code is governed by the MIT license that can be found
 *   in the LICENSE file at the root of this repository.
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
