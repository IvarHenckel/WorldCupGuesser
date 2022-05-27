#include <stdbool.h>

#ifndef LIST
#define LIST

typedef struct node_t node_t;

struct node_t {
    node_t* next;
    void* data;
};

typedef struct list_t list_t;

struct list_t {
    node_t* head;
    int size;
};

void list_add(list_t*, void*);

list_t* list_create();

bool list_empty(list_t*);
#endif