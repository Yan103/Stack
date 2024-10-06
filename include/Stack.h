/*!
    \file
    File with the stack structure
*/

#ifndef STACK
#define STACK

#include <stdio.h>

/// @brief Type of elements in stack
typedef int StackElem_t;

/// @brief Structure with debug info (use in DEBUG mode)
struct DebugInfo {
    const char* filename;
    const char*     func;
    const char* var_name;
    int             line;
    int         err_bits;
};

/// @brief Structure stack with all information about it
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