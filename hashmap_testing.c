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

    // TODO: redesign this. Currently imbalance between memory allocation between client and API
    // ideally just want to have one pointer and return null or return copy of pointer
    int* find_value = malloc(sizeof(int));

    // search "bread" key
    hashmap_get(map, "bread", (void**)&find_value, sizeof(int));
    if (find_value != NULL)
        printf("Found Value: %d\n", *find_value);
    else
        printf("Value not found!\n");
/*
    // search "get" key
    find_value = hashmap_get(map, "get", (void **) &find_value, sizeof(int));
    if (find_value != NULL)
        printf("Found Value: %d\n", *find_value);
    else
        printf("Value not found!\n");

    *find_value = 33;

    // search "apples" key
    //find_value = NULL;
    find_value = hashmap_get(map, "get", (void **) &find_value, sizeof(int));
    if (find_value != NULL)
        printf("Found Value: %d\n", *find_value);
    else
        printf("Value not found!\n");
*/
    hashmap_free(&map);

    return 0;
}