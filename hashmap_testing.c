#include "include/hashmap.h"

int main()
{
    hashmap* map;
    hashmap_init(&map);

    int value = 10;
    hashmap_put(map, "lets", &value, sizeof(int));

    value = 20;
    hashmap_put(map, "get", &value, sizeof(int));

    value = 30;
    hashmap_put(map, "that", &value, sizeof(int));

    value = 40;
    hashmap_put(map, "bread", &value, sizeof(int));

    value = 50;
    hashmap_put(map, "bruh", &value, sizeof(int));

    printf("%d\n", *(int*)map->linked_list_array[8]->value);
    printf("%d\n", *(int*)map->linked_list_array[8]->next->value);

    hashmap_free(&map);
    return 0;
}