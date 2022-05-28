#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void list_add(list_t* list, void* data)
{
    list->data[list->size] = data;
    list->size++;
    if (list->size == list->internal_size) {
        list->internal_size *= 2;
        list->data = realloc(list->data, list->internal_size * sizeof(void*));
    }
}

list_t* list_create() {
    list_t* list = calloc(1, sizeof(list_t));
    list->internal_size = 10;
    list->data = calloc(list->internal_size, sizeof(void*));
}

void list_destroy(list_t* list)
{
    free(list->data);
    free(list);
}

void* list_get(list_t* list, int pos)
{
    return list->data[pos];
}