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

    int* find_value;

    // search "bread" key
    find_value = hashmap_get(map, "bread");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    // search "get" key
    find_value = hashmap_get(map, "get");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    // note that if you change the dereferenced value, the changes will be applied in the map
    *find_value = 33; // this changes "get"'s value to 33 from 20.

    // search "get" key again to see changes
    find_value = hashmap_get(map, "get");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    // search "apples" key
    find_value = hashmap_get(map, "apples");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    hashmap_free(&map);

    return 0;
}