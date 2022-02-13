#include "hashmap.h"

// this is an arbitrary hash function with no mathematical underpinning
// most likely can make some improvements to this
int64_t hash(const char* key)
{
    int64_t hash = 0;
    for (size_t i = 0; i < strlen(key); ++i)
        hash = 7 * hash + 31 * key[i];
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

    (*map)->used_keys = malloc(sizeof(char*) * (*map)->allocation_size);
    (*map)->used_values = malloc(sizeof(void*) * (*map)->allocation_size);
    (*map)->n_used_keys = 0;
    (*map)->key_allocation_size = 16;

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

    // clear used keys and values
    for (size_t i = 0; i < (*map)->n_used_keys; ++i)
    {
        free((*map)->used_keys[i]);
        free((*map)->used_values[i]);
    }
    free((*map)->used_keys);
    free((*map)->used_values);
    (*map)->n_used_keys = 0;

    free(*map);
}

void hashmap_put(hashmap* map, const char* key, void* value, size_t size)
{
    int64_t hash_value = hash(key);
    size_t hash_index = hash_value % map->allocation_size;

    // check if linked list is null
    if (map->linked_list_array[hash_index] == NULL)
    {
        // set head value
        list_init(&map->linked_list_array[hash_index], value, size);
        list_set_key(map->linked_list_array[hash_index], key);
        map->logical_size++;
        map->used_keys[map->n_used_keys] = strdup(key);
        map->used_values[map->n_used_keys] = malloc(sizeof(void*));
        memcpy(map->used_values[map->n_used_keys], value, sizeof(void*));
        map->n_used_keys++;
        if (map->n_used_keys >= map->key_allocation_size)
        {
            map->key_allocation_size *= 2;
            map->used_keys = realloc(map->used_keys, sizeof(char*) * map->key_allocation_size);
            map->used_values = realloc(map->used_values, sizeof(void*) * map->key_allocation_size);
        }

        // resize hashmap and rehash all previously used keys to get their new indices
        if (map->logical_size >= map->load_factor_size)
        {

            // free linked lists
            for (size_t i = 0; i < map->allocation_size; ++i)
                if ((map->linked_list_array)[i] != NULL)
                    list_free(&(map->linked_list_array)[i]);
            free(map->linked_list_array);
            map->linked_list_array = NULL;

            // reallocate memory for the linked lists
            map->linked_list_array = malloc(sizeof(linked_list) * map->allocation_size);

            // reset logical and update allocation sizes / load factor
            map->logical_size = 0;
            map->allocation_size = map->allocation_size << 1; // increase by power of two
            map->load_factor_size = (size_t)((float)map->allocation_size * 0.75f);

            // reset all lists back to NULL
            for (size_t i = 0; i < map->allocation_size; ++i)
                (map->linked_list_array)[i] = NULL;

            // go over used keys and recompute hashes
            for (size_t i = 0; i < map->n_used_keys; ++i)
            {
                hash_value = hash(map->used_keys[i]);
                hash_index = hash_value % map->allocation_size;
                // check if linked list is null
                if (map->linked_list_array[hash_index] == NULL)
                {
                    // set head value
                    list_init(&map->linked_list_array[hash_index], map->used_values[i], size);
                    list_set_key(map->linked_list_array[hash_index], map->used_keys[i]);
                    map->logical_size++;
                }
                else
                    list_add_node_with_key(map->linked_list_array[hash_index], map->used_keys[i], map->used_values[i], size);
            }
        }
    }
    else
    {
        list_add_node_with_key(map->linked_list_array[hash_index], key, value, size);
        map->used_keys[map->n_used_keys] = strdup(key);
        map->used_values[map->n_used_keys] = malloc(sizeof(void*));
        memcpy(map->used_values[map->n_used_keys], value, sizeof(void*));
        map->n_used_keys++;
        if (map->n_used_keys >= map->key_allocation_size)
        {
            map->key_allocation_size *= 2;
            map->used_keys = realloc(map->used_keys, sizeof(char*) * map->key_allocation_size);
            map->used_values = realloc(map->used_values, sizeof(void*) * map->key_allocation_size);
        }
    }
}

void* hashmap_get(hashmap* map, const char* key)
{
    int64_t hash_value = hash(key);
    size_t hash_index = hash_value % map->allocation_size;

    // iterate linked list until key is found
    linked_list* list_copy = map->linked_list_array[hash_index];

    while (list_copy != NULL)
    {
        // if key is found, set that value
        if (strncmp(list_copy->key, key, strlen(list_copy->key)) == 0)
            return list_copy->value;
        else
            list_copy = list_copy->next;
    }

    return NULL;

}

int64_t find_key_index(hashmap* map, const char* key)
{
    for (int64_t i = 0; i < map->n_used_keys; ++i)
        if (strncmp(map->used_keys[i], key, strlen(map->used_keys[i])) == 0) return i;
    return -1;
}

void hashmap_remove(hashmap** map, const char* key)
{
    int64_t hash_value = hash(key);
    size_t hash_index = hash_value % (*map)->allocation_size;

    int64_t key_index = find_key_index(*map, key);

    if (key_index != -1)
    {
        list_remove_by_key(&(*map)->linked_list_array[hash_index], key);
        // remove key/value from meta-arrays
        size_t i = key_index;
        free((*map)->used_keys[i]);
        free((*map)->used_values[i]);
        // after freeing memory allocated by previous key, point to new addresses
        for (i = key_index; i < (*map)->n_used_keys - 1; ++i)
        {
            (*map)->used_keys[i] = (*map)->used_keys[i + 1];
            (*map)->used_values[i] = (*map)->used_values[i + 1];

        }

        (*map)->n_used_keys -= 1;
        (*map)->used_keys = realloc((*map)->used_keys, (*map)->n_used_keys * sizeof(char*));
        (*map)->used_values = realloc((*map)->used_values, (*map)->n_used_keys * sizeof(void*));

    }
}