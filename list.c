#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void list_add(list_t* list, void* data)
{
    node_t* node = calloc(1, sizeof(node_t));
    node->data = data;
    node->next = list->head;
    list->head = node;
    list->size++;
}

list_t* list_create() {
    return calloc(1, sizeof(list_t));
}

bool list_empty(list_t* list) {
    return list->head == NULL;
}

// For testing:
// int main()
// {
//     int for_memory[20];
//     list_t* l = list_create();
//     for (int i = 0; i < 20; i++) {
//         for_memory[i] = i;
//         list_add(l, &for_memory[i]);
//     }
    
//     node_t* current = l->head;
//     while (current != NULL) {
//         printf("%d\n", *((int*) current->data));
//         current = current->next;
//     }

//     return 0;
// }
