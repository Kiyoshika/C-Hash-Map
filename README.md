# C-Hash-Map
An experimental implementation of a custom hashmap in C.

# Implementation Details
The core of the hashmap is implemented as an array of linked lists (for chaining). On the side, two additional arrays are used to record the used keys and values; these are used when resizing the hashmap as every linked list is cleared and all previously used keys are rehashed in the newly-sized map.

There is a loading factor of 75% of the allocated size. The initial allocation size is 16 so the initial load factor is 12. Whenever the load factor is reached, the allocated size raises by the next power of two: 16 -> 32 -> 64 -> ... etc.

# Time Complexity
Time complexities will vary based off of how long the chains at each index are. Some indices could contain only a head node (constant time) or some could be several nodes deep (linear).
* hashmap_get: O(1) (on average)
* hashmap_put:
    * without resizing: O(1)
    * with resizing: O(n) (on average) -- when map dynamically resizes from load factor
* hashmap_remove: O(1) (on average)
* hashmap_get_pairs: O(n) (on average)
# Example #1
This example shows entering a bunch of keys and observing the hashmap dynamically grows as the load factor is reached.

Then we search for a few existing keys, overwrite a value within the map and searching a key that doesn't exist.

```c
#include "hashmap.h"

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

    // search "hello" key
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
```
Output:
```text
Logical Size: 0
Allocation Size: 16
Load Factor: 12

Logical Size: 17
Allocation Size: 32
Load Factor: 24

Value: 10
Value: 200
Value: 33
Value: not found!
```

# Example #2
This example shows removing a couple keys and printing out all key-value pairs to the console.
```c
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
```
Output:
```text
Before Removing:
Key: apples -- Value: 10
Key: bananas -- Value: 20
Key: grapes -- Value: 30
Key: oranges -- Value: 40
Key: watermelon -- Value: 50

After Removing:
Key: bananas -- Value: 20
Key: oranges -- Value: 40
Key: watermelon -- Value: 50
```
# Example #3
This example shows how to extract all the current key/values within a map and prints them to the console.
```c
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
```
Output:
```text
Key: oranges -- Value: 40
Key: grapes -- Value: 30
Key: apples -- Value: 10
Key: cats -- Value: 50
Key: bananas -- Value: 20
```