/*
 * This file is taken from one of my other projects: https://github.com/Kiyoshika/C-Linked-List
 */

#ifndef HASHMAP_LINKED_LIST_H
#define HASHMAP_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    void* value;
    struct node* next;
} linked_list;

void list_init(linked_list** list, void* value, size_t size);

void list_free(linked_list** list);

void list_add_node(linked_list* list, void* value, size_t size);

void list_pop(linked_list** list, void* value, size_t size);

#endif //HASHMAP_LINKED_LIST_H
