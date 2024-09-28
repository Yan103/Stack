#ifndef RETURN_CODES_H
#define RETURN_CODES_H

#include <stdio.h>
//todo enum Debug Status and ErrorCode

enum FuncReturn {
    SUCCESS      =  0,
    PUSH_START   =  1, PUSH_END   = 3,
    POP_START    =  5, POP_END    = 7,
    RESIZE_START =  9, RESIZE_END = 11,
    CTOR_START   =  13, CTOR_END   = 15,
    DTOR_START   =  17, DTOR_END  = 19,
    CLEAN_START  = 21, CLEAN_END  = 23,
    ERROR      = -1,
    BAD_PTR      = -2,
    BAD_SIZE     = -3,
    BAD_FILE     = -4,
    EMPTY_STACK  = -5,
};

#endif // RETURN_CODES_H