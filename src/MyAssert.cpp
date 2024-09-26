/*!
    \file
    File with the declaration of the my_assert function
*/

#include <stdio.h>
#include <stdlib.h>

#include "Default.h"

/*!
    Function that, in case of an error, prints where it occurred and terminates the program
    \param [in]  condition - the condition that needs to be checked
    \param [in] text_error - error description text
    \param [in]       file - the name of the file where the error occurred
    \param [in]       func - the name of the function where the error occurred
    \param [in]       line - the number of the line where the error occurred
*/
void my_assert(bool condition, const char *text_error, const char *file, const char *func, int line) {
    if (!(condition)) {
        printf(RED("%s in: %s -> %s -> %d\n"), text_error, file, func, line);
        abort();
    }
}
