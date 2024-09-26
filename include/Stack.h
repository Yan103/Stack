#ifndef STACK
#define STACK

#include <stdio.h>

typedef int StackElem_t;

struct DebugInfo {
    const char* filename;
    const char*     func;
    int             line;
    const char* var_name;
    char        err_bits;
};

struct Stack {
    DebugInfo* debug_info;
    StackElem_t*     data;
    size_t           size;
    size_t       capacity;
};

#endif // STACK