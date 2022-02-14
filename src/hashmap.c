#include "hashmap.h"

// this is an arbitrary hash function with no mathematical underpinning
// most likely can make some improvements to this
int64_t hash(const char* key)
{
    int64_t hash = 0;
    for (size_t i = 0; i < strlen(key); ++i)
        hash = 31 * hash + key[i];
    return hash;
}

// iterate over all items in every linked list and store their
// key-value pairs into two arrays
void hashmap_get_pairs(hashmap* map, char*** keys, void** values, size_t size)
{
    // assuming keys and values have already been allocated
    // putting memory responsibility on client :)

    // now iterate over ALL items (complete linked list depth) and store them in the arrays
    size_t current_iter = 0;
    for (size_t i = 0; i < map->allocation_size; ++i)
    {
        if (map->linked_list_array[i] != NULL)
        {
            linked_list *tmp = map->linked_list_array[i];
            while (tmp != NULL)
            {
                (*keys)[current_iter] = strdup(tmp->key);
                memcpy((char*)(*values) + current_iter*size, tmp->value, size);
                tmp = tmp->next;
                current_iter++;
            }
        }
    }
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
    (*map)->total_size = 0;

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

    // check if linked list is null
    if (map->linked_list_array[hash_index] == NULL)
    {
        // set head value
        list_init(&map->linked_list_array[hash_index], value, size);
        list_set_key(map->linked_list_array[hash_index], key);
        map->logical_size++;
        map->total_size++;

        // resize hashmap and rehash all previously used keys to get their new indices
        if (map->logical_size >= map->load_factor_size)
        {

            // get all used keys/values to rehash in newly-sized map
            char** used_keys = malloc(sizeof(char*) * map->total_size);
            void* used_values = malloc(size * map->total_size);
            hashmap_get_pairs(map, &used_keys, &used_values, size);

            // free linked lists
            for (size_t i = 0; i < map->allocation_size; ++i)
                if ((map->linked_list_array)[i] != NULL)
                    list_free(&(map->linked_list_array)[i]);
            free(map->linked_list_array);
            map->linked_list_array = NULL;

            // reset logical and update allocation sizes / load factor
            map->logical_size = 0;
            map->allocation_size = map->allocation_size << 1; // increase by power of two
            map->load_factor_size = (size_t)((float)map->allocation_size * 0.75f);

            // reallocate memory for the linked lists
            map->linked_list_array = malloc(sizeof(linked_list) * map->allocation_size);

            // reset all lists back to NULL
            for (size_t i = 0; i < map->allocation_size; ++i)
                (map->linked_list_array)[i] = NULL;

            // go over used keys and recompute hashes
            for (size_t i = 0; i < map->total_size; ++i)
            {
                hash_value = hash(used_keys[i]);
                hash_index = hash_value % map->allocation_size;
                // check if linked list is null
                if (map->linked_list_array[hash_index] == NULL)
                {
                    // set head value
                    list_init(&map->linked_list_array[hash_index], (char*)used_values + i*size, size);
                    list_set_key(map->linked_list_array[hash_index], used_keys[i]);
                    map->logical_size++;
                }
                else
                    list_add_node_with_key(map->linked_list_array[hash_index], used_keys[i], (char*)used_values + i*size, size);
            }

            for (size_t i = 0; i < map->total_size; ++i)
                free(used_keys[i]);
            free(used_keys);
            free(used_values);
        }
    }
    else
    {
        list_add_node_with_key(map->linked_list_array[hash_index], key, value, size);
        map->total_size++;
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

void hashmap_remove(hashmap** map, const char* key)
{
    int64_t hash_value = hash(key);
    size_t hash_index = hash_value % (*map)->allocation_size;

    // check if value exists
    void* find_value = hashmap_get(*map, key);
    if (find_value != NULL)
    {
        list_remove_by_key(&(*map)->linked_list_array[hash_index], key);
        (*map)->total_size--;
    }

}