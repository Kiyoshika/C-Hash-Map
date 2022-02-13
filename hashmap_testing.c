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
    hashmap_put(map, "watermelon", &value, sizeof(int));

    printf("Before Removing:\n");
    for (size_t i = 0; i < map->n_used_keys; ++i)
        printf("Key: %s -- Value: %d\n", map->used_keys[i], *(int*)map->used_values[i]);

    hashmap_remove(&map, "apples");
    hashmap_remove(&map, "grapes");
    hashmap_remove(&map, "thiskeydoesntexist"); // keys not in hashmap are ignored

    printf("\nAfter Removing:\n");
    for (size_t i = 0; i < map->n_used_keys; ++i)
        printf("Key: %s -- Value: %d\n", map->used_keys[i], *(int*)map->used_values[i]);

    hashmap_free(&map);

    return 0;
}