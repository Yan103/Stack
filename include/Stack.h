#ifndef STACK
#define STACK

#include <stdio.h>

typedef int StackElem_t;

struct DebugInfo {
    const char* filename;
    const char*     func;
    const char* var_name;
    int             line;
    int         err_bits;
};

struct Stack {
    long int    RightKanary;
    DebugInfo*   debug_info;
    size_t            hash1;
    StackElem_t*       data;
    size_t             size;
    size_t         capacity;
    size_t            hash2;
    long int     LeftKanary;
};

#endif // STACK