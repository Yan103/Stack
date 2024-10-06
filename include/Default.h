/*!
    \file
    File with default functions
*/

#ifndef DEFAULT_H
#define DEFAULT_H

#define END         "\033[0;0m"
#define BLACK_CLR   "\033[1;30m"
#define RED_CLR     "\033[1;31m"
#define GREEN_CLR   "\033[1;32m"
#define YELLOW_CLR  "\033[1;33m"
#define BLUE_CLR    "\033[1;34m"
#define MAGENTA_CLR "\033[1;35m"
#define CYAN_CLR    "\033[1;36m"
#define WHITE_CLR   "\033[1;37m"

#define BLACK(str)   BLACK_CLR   str END
#define RED(str)     RED_CLR     str END
#define GREEN(str)   GREEN_CLR   str END
#define YELLOW(str)  YELLOW_CLR  str END
#define BLUE(str)    BLUE_CLR    str END
#define MAGENTA(str) MAGENTA_CLR str END
#define CYAN(str)    CYAN_CLR    str END
#define WHITE(str)   WHITE_CLR   str END

/// Proper release of dynamic memory
#define FREE(object) { free(object); (object) = NULL; }

/*!
    @brief Function that, in case of an error, prints where it occurred and terminates the program
    \param [in]  condition - the condition that needs to be checked
    \param [in] text_error - error description text
    \param [in]       file - the name of the file where the error occurred
    \param [in]       func - the name of the function where the error occurred
    \param [in]       line - the number of the line where the error occurred
*/
void my_assert(bool condition, const char *text_error, const char *file, const char *func, int line);

#endif // DEFAULT_H
