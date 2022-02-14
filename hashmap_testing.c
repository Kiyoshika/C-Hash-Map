#include "hashmap.h"

int main()
{
    hashmap* map;
    hashmap_init(&map);

    int value = 10;
    hashmap_put(map, "apples", &value, sizeof(int));

    value = 20;
    hashmap_put(map, "bananas", &value, sizeof(int));

    value = 30;
    hashmap_put(map, "grapes", &value, sizeof(int));

    value = 40;
    hashmap_put(map, "oranges", &value, sizeof(int));

    value = 50;
    hashmap_put(map, "cats", &value, sizeof(int));

    hashmap_remove(&map, "grapes");
    hashmap_remove(&map, "apples");
    hashmap_remove(&map, "dogs"); // will be ignored since key doesn't exist

    // client manages the memory to store local keys/values
    char** keys = malloc(sizeof(char*) * map->total_size);
    int* values = malloc(sizeof(int) * map->total_size);

    hashmap_get_pairs(map, &keys, (void**)&values, sizeof(int));

    // print out all key value pairs in map
    // note the order will not necessarily be the same due to the hash function assigning
    // different positions
    for (size_t i = 0; i < map->total_size; ++i)
        printf("Key: %s -- Value: %d\n", keys[i], values[i]);

    // free up memory
    for (size_t i = 0; i < map->total_size; ++i)
        free(keys[i]);
    free(keys);
    free(values);

    hashmap_free(&map);

    return 0;
}