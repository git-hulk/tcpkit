#include <stdlib.h>
#include "tk_test.h"
#include "hashtable.h"

static int freed;

static void counting_free(void *v) {
    freed++;
    free(v);
}

static char *dup_value(const char *s) {
    char *v = malloc(strlen(s) + 1);
    strcpy(v, s);
    return v;
}

static void test_get_returns_added_value(void) {
    hashtable *ht = hashtable_create(16);

    hashtable_add(ht, "10.0.0.1:6379", dup_value("server"));
    TK_EQ_STR((char *)hashtable_get(ht, "10.0.0.1:6379"), "server");
    TK_CHECK(hashtable_get(ht, "10.0.0.2:6379") == NULL, "absent key must be NULL");

    hashtable_destroy(ht);
}

static void test_add_keeps_the_first_value(void) {
    hashtable *ht = hashtable_create(16);
    char *rejected;
    void *existing;

    hashtable_add(ht, "key", dup_value("first"));
    /* A rejected duplicate is not adopted by the table, so it is still ours. */
    rejected = dup_value("second");
    existing = hashtable_add(ht, "key", rejected);

    TK_EQ_STR((char *)existing, "first");
    TK_EQ_STR((char *)hashtable_get(ht, "key"), "first");
    free(rejected);

    hashtable_destroy(ht);
}

static void test_del_removes_the_entry(void) {
    hashtable *ht = hashtable_create(4);

    hashtable_add(ht, "a", dup_value("1"));
    hashtable_add(ht, "b", dup_value("2"));

    TK_EQ_INT(hashtable_del(ht, "a"), 1);
    TK_CHECK(hashtable_get(ht, "a") == NULL, "deleted key must be NULL");
    TK_EQ_STR((char *)hashtable_get(ht, "b"), "2");
    TK_EQ_INT(hashtable_del(ht, "a"), 0);

    hashtable_destroy(ht);
}

static void test_collisions_keep_every_entry(void) {
    hashtable *ht = hashtable_create(1);
    char key[16];
    int i;

    for (i = 0; i < 32; i++) {
        snprintf(key, sizeof(key), "key-%d", i);
        hashtable_add(ht, key, dup_value(key));
    }
    for (i = 0; i < 32; i++) {
        snprintf(key, sizeof(key), "key-%d", i);
        TK_EQ_STR((char *)hashtable_get(ht, key), key);
    }

    /* Deleting from the middle of a bucket chain must not orphan the tail. */
    TK_EQ_INT(hashtable_del(ht, "key-16"), 1);
    TK_EQ_STR((char *)hashtable_get(ht, "key-17"), "key-17");
    TK_EQ_STR((char *)hashtable_get(ht, "key-0"), "key-0");

    hashtable_destroy(ht);
}

static void test_values_lists_every_entry(void) {
    hashtable *ht = hashtable_create(8);
    void **values;
    int cnt = -1;

    values = hashtable_values(ht, &cnt);
    TK_EQ_INT(cnt, 0);
    free(values);

    hashtable_add(ht, "a", dup_value("1"));
    hashtable_add(ht, "b", dup_value("2"));
    hashtable_add(ht, "c", dup_value("3"));

    values = hashtable_values(ht, &cnt);
    TK_EQ_INT(cnt, 3);
    TK_CHECK(values != NULL, "values must be allocated");
    free(values);

    hashtable_destroy(ht);
}

static void test_destroy_uses_the_custom_free(void) {
    hashtable *ht = hashtable_create(8);

    freed = 0;
    ht->free = counting_free;
    hashtable_add(ht, "a", dup_value("1"));
    hashtable_add(ht, "b", dup_value("2"));
    hashtable_destroy(ht);

    TK_EQ_INT(freed, 2);
}

int main(void) {
    TK_RUN(test_get_returns_added_value);
    TK_RUN(test_add_keeps_the_first_value);
    TK_RUN(test_del_removes_the_entry);
    TK_RUN(test_collisions_keep_every_entry);
    TK_RUN(test_values_lists_every_entry);
    TK_RUN(test_destroy_uses_the_custom_free);
    return tk_report("hashtable");
}
