#include "../include/hashmap.h"

int64_t hash(const char* key)
{
    int64_t hash = 0;
    for (size_t i = 0; i < strlen(key); ++i)
        hash = 31 * hash + key[i];
    return hash;
}

void hashmap_init(hashmap** map)
{
    // start with allocation of 2^4 = 16
    // increment with powers of two using shifting: allocation_size << 1
    *map = malloc(sizeof(hashmap));
    (*map)->allocation_size = 16;
    // load factor controls when the map should be resized. Here we resize the map once it reaches 75% capacity
    (*map)->load_factor_size = (size_t)((float)(*map)->allocation_size * 0.75f);
    (*map)->logical_size = 0;

    (*map)->linked_list_array = malloc(sizeof(linked_list) * (*map)->allocation_size);

    // set 16 NULL linked lists
    for (size_t i = 0; i < (*map)->allocation_size; ++i)
        ((*map)->linked_list_array)[i] = NULL;
}

void hashmap_free(hashmap** map)
{
    // clear linked lists within map
    for (size_t i = 0; i < (*map)->allocation_size; ++i)
        if (((*map)->linked_list_array)[i] != NULL)
            list_free(&((*map)->linked_list_array)[i]);
    free((*map)->linked_list_array);
    free(*map);
}

void hashmap_put(hashmap* map, const char* key, void* value, size_t size)
{
    int64_t hash_value = hash(key);
    size_t hash_index = hash_value % map->allocation_size;

    // check linked list is null
    if (map->linked_list_array[hash_index] == NULL)
    {
        // set head value
        list_init(&map->linked_list_array[hash_index], value, size);
        map->logical_size++;
        if (map->logical_size >= map->load_factor_size)
            printf("TODO: Resize hashmap here...\n");
    }
        // if hash collision exists, add value to the linked list
    else
        list_add_node(map->linked_list_array[hash_index], value, size);
}