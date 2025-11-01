#include "set.h"
#include <stdlib.h>
#include <stdint.h>

Set set_new()
{
    Set set;
    set.capacity = DEFAULT_SET_CAP;
    set.count = 0;
    set.items = (SetNode*)calloc(set.capacity, sizeof(SetNode));

    return set;
}

void set_delete(Set *set)
{
    free(set->items);
    set->items = NULL;
    set->capacity = 0;
    set->count = 0;

    return;
}

uint32_t set_get_hash(Set *set, SET_KEY key)
{
    return key % set->capacity;
}

uint32_t set_get_i(Set *set, SET_KEY key)
{
    uint32_t hash_value = set_get_hash(set, key);

    for (uint32_t i = 0; i < set->capacity; i++) {
        uint32_t index = (hash_value + i) % set->capacity;

        if (set->items[index].valid && CMP(key, set->items[index].key)) {
            return index;
        }
    }

    return 0;
}

bool set_push(Set *set, SET_KEY key)
{
    uint32_t hash_value = set_get_hash(set, key);
    
    for (uint32_t i = 0; i < set->capacity; i++) {
        uint32_t index = (hash_value + i) % set->capacity;

        if (!set->items[index].valid) {
            set->items[index].key = key;
            set->items[index].valid = true;
            set->count++;

            return true;
        }
    }
    
    return false;
}

bool set_has(Set *set, SET_KEY key)
{
    uint32_t hash_value = set_get_hash(set, key);
    
    for (uint32_t i = 0; i < set->capacity; i++) {
        uint32_t index = (hash_value + i) % set->capacity;

        if (set->items[index].valid && CMP(key, set->items[index].key)) {
            return true;
        }
    }
    
    return false;
}

SET_KEY* set_array(Set *set)
{
    if (set->count <= 0 || set->count > set->capacity) return NULL;

    SET_KEY *array = malloc(sizeof(SET_KEY) * set->count);

    for (size_t i = 0, j = 0; i < set->capacity && j < set->count; i++) {
        if (set->items[i].valid) {
            array[j++] = set->items[i].key;
        }
    }

    return array;
}
