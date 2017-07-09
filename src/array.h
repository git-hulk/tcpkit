#ifndef _ARRAY_H_
#define _ARRAY_H_
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
#endif
