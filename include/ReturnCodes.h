#ifndef RETURN_CODES_H
#define RETURN_CODES_H

#include <stdio.h>

enum FuncReturn {
    SUCCESS        = 0b0,
    BAD_PTR        = 0b1,
    BAD_SIZE       = 0b10,
    BAD_FILE       = 0b100,
    EMPTY_STACK    = 0b1000,
    STACK_OVERFLOW = 0b10000,
    KANARY_DAMAGED = 0b100000,
    HASH_ERROR     = 0b1000000,
};

#endif // RETURN_CODES_H