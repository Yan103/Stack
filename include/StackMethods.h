/*!
    \file
    File with definition of stack functions
*/

#ifndef STACKMETHODS_H
#define STACKMETHODS_H

#include <assert.h>
#include <stdio.h>

#include "ReturnCodes.h"
#include "Stack.h"

/// @brief Constant for poison elements in stack
static const int POISON_NUM = -666;

/// @brief Constant for default stack size
static const int START_SIZE = 2;

/// @brief Macros for stack initialization
#define STACK_INIT(st) StackInit(__FILE__, __func__, __LINE__, #st)

//* ========== IMPORTANT ==========
//* if you run the program in DEBUG mode (with the flag of the same name), additional information will be checked
//* else all these checks will be disabled
#ifdef DEBUG
    /// @brief Macros for Dumping information about stack
    #define STACK_DUMP(st) { StackDump(st, __FILE__, __func__, __LINE__); }
    /// @brief Macros for my assert function
    #define ASSERT(condition, text_error) my_assert(condition, text_error, __FILE__, __func__, __LINE__)
    /// @brief Macros for my stack assert function
    #define STACK_ASSERT(st) {                                    \
    if (!StackOk(st)) {                                           \
        STACK_DUMP(st);                                           \
        assert(0);                                                \
    }                                                             \
}
    /// @brief Macros for creating debug information about stack
    #define DEBUG_INFO(st) {                                      \
    (st)->debug_info = (DebugInfo*) calloc(1, sizeof(DebugInfo)); \
    ASSERT((st)->debug_info != NULL, ERR_MSG_NULL_PTR);           \
    (st)->debug_info->filename = file;                            \
    (st)->debug_info->func = func;                                \
    (st)->debug_info->line = line;                                \
    (st)->debug_info->var_name = var_name;                        \
}
#else
    /// @brief Macros for Dumping information about stack (empty in N_DEBUG mode)
    #define STACK_DUMP(st) {}
    /// @brief Macros for my stack assert function (empty in N_DEBUG mode)
    #define STACK_ASSERT(st) {}
    /// @brief Macros for creating debug information about stack (empty in N_DEBUG mode)
    #define DEBUG_INFO(st) {}
    /// @brief Macros for my assert function (empty in N_DEBUG mode)
    #define ASSERT(condition, text_error) {}
#endif

/*!
    @brief Function that checks the right and left kanaryees
    \param [in] st - the pointer on Stack structer
*/
void StackKanaryCheck(Stack* st);

/*!
    @brief Function that do stack initialization
    \param [in] st       - the pointer on pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
    \param [in] var_name - the name of variable with stack
*/
Stack* StackInit(const char *file, const char* func, int line, const char* var_name);

/*!
    @brief Function that creates the stack
    \param [in] st       - the pointer on Stack structer
    \param [in] capacity - the max size of stack
*/
enum FuncReturn StackCtor(Stack* st, size_t capacity);

/*!
    @brief Function that dumping all information about stack
    \param [in] st - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
enum FuncReturn StackDump(Stack* st, const char *file, const char* func, int line);

/*!
    @brief Function that checks stack state
    \param [in] st - the pointer on Stack structer
*/
int StackOk(Stack* st);

/*!
    @brief Function that get string name of error
    \param [in] error - the enum value of FuncReturn
*/
const char* StackStrErr(FuncReturn error);

/*!
    @brief Function that pushs new value in end of stack
    \param [in] st    - the pointer on Stack structer
    \param [in] value - the value which will push in last place in stack
*/
enum FuncReturn StackPush(Stack* st, StackElem_t value);

/*!
    @brief Function that gets last element in stack
    \param [in] st    - the pointer on Stack structer
    \param [in] value - the pointer on variable kuda will save the pop element
*/
enum FuncReturn StackPop(Stack* st, StackElem_t* value);

/*!
    @brief Function that destroys the stack
    \param [in] st - the pointer on Stack structer
*/
enum FuncReturn StackDtor(Stack* st);

/*!
    @brief Function that shows information about stack
    \param [in] filename - the pointer on FILE
    \param [in] st       - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
enum FuncReturn DumpPrint(FILE* filename, Stack* st, const char *file, const char *func, int line);

/*!
    @brief Function that resizes stack size
    \param [in] st       - the pointer on Stack structer
    \param [in] new_size - the new size of stack
*/
enum FuncReturn StackResize(Stack* st, size_t new_size);

/*!
    @brief Function that realloc memory and fills it POISON_NUM
    \param [in] st       - the pointer on Stack structer
    \param [in] new_size - the new size of stack
*/
enum FuncReturn ReCalloc(Stack* st, size_t new_size);

/*!
    @brief Function that try find element in stack (return element's index if he in stack, else capacity)
    \param [in] st         - the pointer on Stack structer
    \param [in] find_value - the find value
*/
size_t StackFind(Stack* st, StackElem_t find_value);

/*!
    @brief Function that cleans the stack
    \param [in] st - the pointer on Stack structer
*/
enum FuncReturn StackClean(Stack* st);

/*!
    @brief Function that shows information about stack errors
    \param [in] filename - the pointer on FILE
    \param [in] st       - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
enum FuncReturn DumpErrorPrint(FILE* filename, Stack* st, const char *file, const char *func, int line);

/*!
    @brief Function that calculates first hash for stack
    \param [in] st - the pointer on Stack structer
*/
size_t StackHashFunc1(Stack* st);

/*!
    @brief Function that calculates second hash for stack
    \param [in] st - the pointer on Stack structer
*/
size_t StackHashFunc2(Stack* st);

/*!
    @brief Function that checks the stack hashes
    \param [in] st - the pointer on Stack structer
*/
void StackHashCheck(Stack* st);

/*!
    @brief Function that updates stack hashes
    \param [in] st - the pointer on Stack structer
*/
enum FuncReturn StackUpdateHash(Stack* st);

#endif // STACKMETHODS_H