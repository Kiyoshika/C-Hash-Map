#ifndef CHASHMAP_HASHMAP_H
#define CHASHMAP_HASHMAP_H

#include "../ext/linked_list.h"
#include <stdbool.h>

// Under the hood, this hashmap uses an array of linked lists to implement chaining upon index collision.
typedef struct
{
    linked_list** linked_list_array;
    size_t logical_size;
    size_t allocation_size;
    size_t load_factor_size;
} hashmap;

void hashmap_init(hashmap** map);

void hashmap_put(hashmap* map, const char* key, void* value, size_t size);

void hashmap_get(hashmap* map, const char* key, void** value, size_t size);

void hashmap_free(hashmap** map);

#endif //CHASHMAP_HASHMAP_H
