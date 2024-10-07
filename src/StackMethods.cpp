/*!
    \file
    File with the declaration of the my stack functions
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Default.h"
#include "ReturnCodes.h"
#include "StackMethods.h"
#include "Stack.h"

/// @brief Constant with log filename
static const char* LOG_FILE         = "log.txt";

/// @brief Constant with message about NULL pointer
static const char* ERR_MSG_NULL_PTR = "Null pointer was passed";

/// @brief Constant with 30 equals ('=') symbols
static const char* THIRTYEQUALS     = "=============================="; //// CRINGE???

/*!
    @brief Function that do stack initialization
    \param [in] st       - the pointer on pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
    \param [in] var_name - the name of variable with stack
*/
Stack* StackInit(const char *file, const char *func, int line, const char* var_name) {
    Stack* st = (Stack*) calloc(1, sizeof(Stack));

    DEBUG_INFO(st);

    StackCtor(st, START_SIZE);

    return st;
}

/*!
    @brief Function that creates the stack
    \param [in] st       - the pointer on Stack structer
    \param [in] capacity - the max size of stack
*/
enum FuncReturn StackCtor(Stack* st, size_t capacity) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);
    StackKanaryCheck(st);
    STACK_DUMP(st);

    st->data = 0;
    st->capacity = capacity;
    st->LeftKanary = st->RightKanary = 0;

    st->data = (StackElem_t*) calloc(capacity, sizeof(StackElem_t));
    if (st->data == NULL) st->debug_info->err_bits |= BAD_PTR;

    StackKanaryCheck(st);
    STACK_ASSERT(st);
    for (size_t i = 0; i < capacity; i++) st->data[i] = POISON_NUM;

    StackUpdateHash(st);
    STACK_DUMP(st);

    return SUCCESS;
}

/*!
    @brief Function that dumping all information about stack
    \param [in] st - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
FuncReturn StackDump(Stack* st, const char* file, const char* func, int line) {
    FILE* log_filename = fopen(LOG_FILE, "a");

    if (!log_filename) {
        printf(RED("Error occured while opening file\n"));
        st->debug_info->err_bits |= BAD_FILE;

        STACK_ASSERT(st);
    }

    if (st != NULL) {
        if (st->debug_info->err_bits == 0) DumpPrint(log_filename, st, file, func, line);
        else DumpErrorPrint(log_filename, st, file, func, line);
    } else {
        fprintf(log_filename, "%s", THIRTYEQUALS);
        fprintf(log_filename, " STACK WAS DESTROYED!!! ");
        fprintf(log_filename, "%s", THIRTYEQUALS);
        fprintf(log_filename, "\n\n");
    }
    fclose(log_filename);

    return SUCCESS;
}

/*!
    @brief Function that returns time in what function was launched
*/
static tm GetTime() {
    time_t time_now = time(NULL);

    return *localtime(&time_now);
}

/*!
    @brief Function that shows information about stack errors
    \param [in] filename - the pointer on FILE
    \param [in] st       - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
enum FuncReturn DumpErrorPrint(FILE* filename, Stack* st, const char *file, const char *func, int line) {
    ASSERT(filename != NULL, ERR_MSG_NULL_PTR);
    ASSERT(st       != NULL, ERR_MSG_NULL_PTR);
    ASSERT(file     != NULL, ERR_MSG_NULL_PTR);
    ASSERT(func     != NULL, ERR_MSG_NULL_PTR);

    struct tm tm = GetTime();

    fprintf(filename, "%s ERROR %d-%02d-%02d %02d:%02d:%02d %s\n",
            THIRTYEQUALS, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, THIRTYEQUALS);
    fprintf(filename, "Stack [%p] {%s} %s: %d, from %s: %d (%s)\n",
           (void*)st, st->debug_info->var_name, st->debug_info->filename, st->debug_info->line, file, line, func);

    size_t mask = 0b1;
    for (size_t i = 0; i < 32; i++) {
        if (((size_t)st->debug_info->err_bits & mask) == mask ) {
            fprintf(filename, ">>> %s\n", StackStrErr((FuncReturn)((size_t)st->debug_info->err_bits & mask)));
        }
        mask = mask << 1;
    }
    fputc('\n', filename);

    return SUCCESS;
}

/*!
    @brief Function that shows information about stack
    \param [in] filename - the pointer on FILE
    \param [in] st       - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
enum FuncReturn DumpPrint(FILE* filename, Stack* st, const char *file, const char *func, int line) {
    ASSERT(filename != NULL, ERR_MSG_NULL_PTR);
    ASSERT(st       != NULL, ERR_MSG_NULL_PTR);
    ASSERT(file     != NULL, ERR_MSG_NULL_PTR);
    ASSERT(func     != NULL, ERR_MSG_NULL_PTR);

    struct tm tm = GetTime();

    fprintf(filename, "DUMPED %d-%02d-%02d %02d:%02d:%02d\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(filename, "Stack [%p] {%s} %s: %d, from %s: %d (%s -> SUCCESS) {\n",
           (void*)st, st->debug_info->var_name, st->debug_info->filename, st->debug_info->line, file, line, func);
    fprintf(filename, "\tsize = %lu,\n\tcapacity = %lu,\n\thash1 = %lu,\n\thash2 = %lu,\n\tdata [%p] {\n",
                        st->size,    st->capacity,     st->hash1,      st->hash2, (void*)st->data);

    for (size_t i = 0; i < st->capacity; i++) {
        if (i < st->size && st->data[i] != POISON_NUM) {
            fprintf(filename, "\t\t* [%lu] %d\n", i, st->data[i]);
        } else {
            fprintf(filename, "\t\t  [%lu] %d (POISON)\n", i, st->data[i]);
        }
    }

    fprintf(filename, "\t}\n}\n\n");

    return SUCCESS;
}

/*!
    @brief Function that checks stack state
    \param [in] st - the pointer on Stack structer
*/
int StackOk(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    return st->debug_info->err_bits == 0;
}

/*!
    @brief Function that get string name of error
    \param [in] error - the enum value of FuncReturn
*/
const char* StackStrErr(FuncReturn error) {
    /// @brief Macros for case-return short form
    #define DESCR_(error) { case error: return #error; }
    switch (error) {
        DESCR_(SUCCESS);
        DESCR_(BAD_FILE);
        DESCR_(BAD_PTR);
        DESCR_(BAD_SIZE);
        DESCR_(EMPTY_STACK);
        DESCR_(STACK_OVERFLOW);
        DESCR_(KANARY_DAMAGED);
        DESCR_(HASH_ERROR);

        default: return "UNKNOWN STATUS";
    }
    #undef DESCR_
}

/*!
    @brief Function that pushs new value in end of stack
    \param [in] st    - the pointer on Stack structer
    \param [in] value - the value which will push in last place in stack
*/
enum FuncReturn StackPush(Stack* st, StackElem_t value) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    StackHashCheck(st);
    StackKanaryCheck(st);
    STACK_ASSERT(st);
    STACK_DUMP(st);

    if (st->size >= st->capacity) StackResize(st, st->capacity * 2);

    if (st->size >= st->capacity) {
        st->debug_info->err_bits |= STACK_OVERFLOW;
        STACK_ASSERT(st);
    }

    st->data[st->size] = value;
    st->size++;

    StackUpdateHash(st);
    StackKanaryCheck(st);

    STACK_ASSERT(st);
    STACK_DUMP(st);

    return SUCCESS;
}

/*!
    @brief Function that gets last element in stack
    \param [in] st    - the pointer on Stack structer
    \param [in] value - the pointer on variable kuda will save the pop element
*/
enum FuncReturn StackPop(Stack* st, StackElem_t* value) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);
    ASSERT(value != NULL, ERR_MSG_NULL_PTR);

    StackHashCheck(st);
    StackKanaryCheck(st);

    STACK_ASSERT(st);
    STACK_DUMP(st);

    if (st->size > 0 && st->data[st->size - 1] == POISON_NUM) {
        st->debug_info->err_bits |= EMPTY_STACK;
        STACK_ASSERT(st);

        return EMPTY_STACK;
    } else if (st->size <= 0) {
        st->debug_info->err_bits |= BAD_SIZE;
        STACK_ASSERT(st);

        return BAD_FILE;
    }

    *value = st->data[st->size - 1];
    st->data[st->size - 1] = POISON_NUM;
    st->size--;
    StackUpdateHash(st);

    if (st->size <= st->capacity / 4) StackResize(st, st->capacity / 2);

    StackKanaryCheck(st);

    STACK_ASSERT(st);
    STACK_DUMP(st);

    return SUCCESS;
}

/*!
    @brief Function that destroys the stack
    \param [in] st - the pointer on Stack structer
*/
enum FuncReturn StackDtor(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    StackHashCheck(st);
    StackKanaryCheck(st);

    STACK_ASSERT(st);
    STACK_DUMP(st);

    FREE(st);

    return SUCCESS;
}

/*!
    @brief Function that resizes stack size
    \param [in] st       - the pointer on Stack structer
    \param [in] new_size - the new size of stack
*/
enum FuncReturn StackResize(Stack* st, size_t new_size) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    StackHashCheck(st);
    StackKanaryCheck(st);

    STACK_ASSERT(st);
    STACK_DUMP(st);

    ReCalloc(st, new_size);

    StackUpdateHash(st);
    StackKanaryCheck(st);

    STACK_DUMP(st);

    return SUCCESS;
}

/*!
    @brief Function that realloc memory and fills it POISON_NUM
    \param [in] st       - the pointer on Stack structer
    \param [in] new_size - the new size of stack
*/
enum FuncReturn ReCalloc(Stack* st, size_t new_size) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    st->data = (StackElem_t*) realloc(st->data, sizeof(StackElem_t) * new_size);
    ASSERT(st->data != NULL, ERR_MSG_NULL_PTR);

    if (new_size > st->capacity) for (size_t i = st->size; i < new_size; i++) st->data[i] = POISON_NUM;
    st->capacity = new_size;

    StackKanaryCheck(st);

    STACK_ASSERT(st);

    return SUCCESS;
}

/*!
    @brief Function that try find element in stack (return element's index if he in stack, else capacity)
    \param [in] st         - the pointer on Stack structer
    \param [in] find_value - the find value
*/
size_t StackFind(Stack* st, StackElem_t find_value) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    StackHashCheck(st);
    StackKanaryCheck(st);

    STACK_ASSERT(st);

    for (size_t i = 0; i < st->size; i++)
        if (st->data[i] == find_value)
            return i;

    return st->capacity;
}

/*!
    @brief Function that cleans the stack
    \param [in] st - the pointer on Stack structer
*/
enum FuncReturn StackClean(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    StackHashCheck(st);
    StackKanaryCheck(st);

    STACK_ASSERT(st);
    STACK_DUMP(st);

    for (size_t i = 0; i < st->size; i++) st->data[i] = POISON_NUM;
    st->size = 0;

    StackUpdateHash(st);
    StackKanaryCheck(st);

    STACK_ASSERT(st);
    STACK_DUMP(st);

    return SUCCESS;
}

/*!
    @brief Function that checks the right and left kanaryees
    \param [in] st - the pointer on Stack structer
*/
void StackKanaryCheck(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    if (st->RightKanary != st->LeftKanary) {
        st->debug_info->err_bits |= KANARY_DAMAGED;
    }
}

/*!
    @brief Function that calculates hash for void* type
    \param [in] arr      - the pointer on array
    \param [in] ElemSize - the size of elements
    \param [in] ElemNum  - the number of elements
*/
static size_t voidHashFunc1(void* arr, size_t ElemSize, size_t ElemNum) {
    ASSERT(arr != NULL, ERR_MSG_NULL_PTR);

    size_t hash = 0;
    size_t data = (size_t)arr;
    for (size_t i = 0; i < ElemNum; i++) {
        hash = data + (hash << 16) + (hash >> 7) - hash;
        data += ElemSize;
    }

    return hash;
}

/*!
    @brief Function that calculates hash for void* type
    \param [in] arr      - the pointer on array
    \param [in] ElemSize - the size of elements
    \param [in] ElemNum  - the number of elements
*/
static size_t voidHashFunc2(void* arr, size_t ElemSize, size_t ElemNum) {
    ASSERT(arr != NULL, ERR_MSG_NULL_PTR);

    size_t hash = 1;
    size_t data = (size_t)arr;
    for (size_t i = 0; i < ElemNum; i++) {
        hash = data + (hash << 11) + (hash >> 4) - hash;
        data += ElemSize;
    }

    return hash;
}

/*!
    @brief Function that calculates first hash for stack
    \param [in] st - the pointer on Stack structer
*/
size_t StackHashFunc1(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    StackKanaryCheck(st);

    STACK_ASSERT(st);

    size_t hash = voidHashFunc1(st->data, sizeof(StackElem_t), st->size);

    return hash;
}

/*!
    @brief Function that calculates second hash for stack
    \param [in] st - the pointer on Stack structer
*/
size_t StackHashFunc2(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    StackKanaryCheck(st);

    STACK_ASSERT(st);

    size_t hash = voidHashFunc2(st->data, sizeof(StackElem_t), st->size);

    return hash;
}

/*!
    @brief Function that checks the stack hashes
    \param [in] st - the pointer on Stack structer
*/
void StackHashCheck(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    if (st->hash1 != StackHashFunc1(st) || st->hash2 != StackHashFunc2(st)) {
        st->debug_info->err_bits |= HASH_ERROR;
    }
}

/*!
    @brief Function that updates stack hashes
    \param [in] st - the pointer on Stack structer
*/
enum FuncReturn StackUpdateHash(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    st->hash1 = StackHashFunc1(st);
    st->hash2 = StackHashFunc2(st);

    return SUCCESS;
}
