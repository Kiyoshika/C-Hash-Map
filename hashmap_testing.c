#include "include/hashmap.h"

int main()
{
    hashmap* map;
    hashmap_init(&map);

    printf("Logical Size: %zu\n", map->logical_size);
    printf("Allocation Size: %zu\n", map->allocation_size);
    printf("Load Factor: %zu\n\n", map->load_factor_size);

    int value = 10;
    hashmap_put(map, "hello", &value, sizeof(int));

    value = 20;
    hashmap_put(map, "this", &value, sizeof(int));

    value = 30;
    hashmap_put(map, "is", &value, sizeof(int));

    value = 40;
    hashmap_put(map, "my", &value, sizeof(int));

    value = 50;
    hashmap_put(map, "experimental", &value, sizeof(int));

    value = 60;
    hashmap_put(map, "hashmap", &value, sizeof(int));

    value = 70;
    hashmap_put(map, "implementation", &value, sizeof(int));

    value = 80;
    hashmap_put(map, "and", &value, sizeof(int));

    value = 90;
    hashmap_put(map, "I", &value, sizeof(int));

    value = 100;
    hashmap_put(map, "hope", &value, sizeof(int));

    value = 110;
    hashmap_put(map, "you", &value, sizeof(int));

    value = 120;
    hashmap_put(map, "enjoy", &value, sizeof(int));

    value = 130;
    hashmap_put(map, "it", &value, sizeof(int));

    value = 140;
    hashmap_put(map, "as", &value, sizeof(int));

    value = 150;
    hashmap_put(map, "much", &value, sizeof(int));

    value = 160;
    hashmap_put(map, "asi", &value, sizeof(int));

    value = 170;
    hashmap_put(map, "do", &value, sizeof(int));

    value = 180;
    hashmap_put(map, "yeehaw", &value, sizeof(int));

    value = 190;
    hashmap_put(map, "cowboy", &value, sizeof(int));

    value = 190;
    hashmap_put(map, "frozen", &value, sizeof(int));

    value = 200;
    hashmap_put(map, "burritos", &value, sizeof(int));

    value = 210;
    hashmap_put(map, "are", &value, sizeof(int));

    value = 220;
    hashmap_put(map, "good", &value, sizeof(int));

    printf("Logical Size: %zu\n", map->logical_size);
    printf("Allocation Size: %zu\n", map->allocation_size);
    printf("Load Factor: %zu\n\n", map->load_factor_size);

    int* find_value;

    // search "hellow" key
    find_value = hashmap_get(map, "hello");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    // search "burritos" key
    find_value = hashmap_get(map, "burritos");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    // note that if you change the dereferenced value, the changes will be applied in the map
    *find_value = 33; // this changes "get"'s value to 33 from 20.

    // search "burritos" key again to see changes
    find_value = hashmap_get(map, "burritos");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    // search "apples" key
    find_value = hashmap_get(map, "apples");
    if (find_value == NULL) printf("Value: not found!\n");
    else printf("Value: %d\n", *find_value);

    hashmap_free(&map);

    return 0;
}