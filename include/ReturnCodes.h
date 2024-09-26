#ifndef RETURN_CODES_H
#define RETURN_CODES_H

#include <stdio.h>
//todo enum Debug Status and ErrorCode

enum ReturnCode {
    SUCCESS      =  0,
    PUSH_START   =  1, PUSH_END   = 2,
    POP_START    =  3, POP_END    = 4,
    RESIZE_START =  5, RESIZE_END = 6,
    CTOR_START   =  7, CTOR_END   = 8,
    DTOR_START   =  9, DTOR_END  = 10,
    ERROR        = -1,
    BAD_PTR      = -2,
    BAD_SIZE     = -3,
    BAD_FILE     = -4,
    EMPTY_STACK  = -5,
};

#endif // RETURN_CODES_H