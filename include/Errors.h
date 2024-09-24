#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

enum ErrorsCode {
    SUCCESS     =  0,
    ERROR       = -1,
    BAD_PTR     = -2,
    BAD_SIZE    = -3,
    BAD_FILE    = -4,
    EMPTY_STACK = -5,
    CLEAN_STACK = -6,
};

#endif // ERRORS_H