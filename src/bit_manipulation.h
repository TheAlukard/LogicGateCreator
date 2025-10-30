#pragma once

#define BIT_AT(val, i) (((val) >> (i)) & 1)
#define BIT_SET(val, i, x)                     \
    do {                                       \
        (val) |= (BIT_AT(val, i) | (x)) << i;  \
    } while (0)
