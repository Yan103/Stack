#ifndef STACKSTRUCT
#define STACKSTRUCT

#include <stdio.h>

typedef int StackElem_t;

struct StackStruct {
    const char* filename;
    int             line;
    const char* var_name;
    StackElem_t*    data;
    size_t          size;
    size_t      capacity;
};

#endif // STACKSTRUCT