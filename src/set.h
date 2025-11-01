#pragma once

#ifndef _SET_H
#define _SET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "types.h"

#define SET_KEY u64

#define CMP(key, other) ((key) == (other)) 

typedef struct {
    SET_KEY key;
    bool valid;
} SetNode;

typedef struct {
    SetNode *items;
    size_t count;
    size_t capacity;
} Set;

#define DEFAULT_SET_CAP 300 

Set set_new(void);
void set_delete(Set *set);
bool set_push(Set *set, SET_KEY key);
SET_KEY* set_array(Set *set);

#endif // _SET_H
