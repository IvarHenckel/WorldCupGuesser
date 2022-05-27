#include <stdbool.h>

#ifndef LIST
#define LIST

typedef struct list_t list_t;

struct list_t {
    void** data;
    int size;
    int internal_size;
};

void list_add(list_t*, void*);

list_t* list_create();

void list_destroy();

void* list_get(list_t*, int);
#endif