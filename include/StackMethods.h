#ifndef STACKMETHODS_H
#define STACKMETHODS_H

#include <assert.h>
#include <stdio.h>

#include "Stack.h"
#include "ReturnCodes.h"

const int POISON_NUM = -666;
const int START_SIZE = 2;

#define STACK_INIT(st) { StackInit(&st, __FILE__, __func__, __LINE__, #st); }

#define STACK_DUMP(st, status) { StackDump(st, __FILE__, __func__, __LINE__, status); }

#define STACK_ASSERT(st, status) {       \
    if (!StackOk(st)) {                  \
        STACK_DUMP(st, status);          \
        assert(0);                       \
        return ERROR;                    \
    }                                    \
    }                                    \

enum ReturnCode StackInit(Stack** st, const char *file, const char *func, int line, const char* var_name);

enum ReturnCode StackCtor(Stack* st, size_t capacity);

enum ReturnCode StackDump(Stack* st, const char *file, const char *func, int line, enum ReturnCode status);

int StackOk(Stack* st);

const char* StackStrErr(enum ReturnCode error);

enum ReturnCode StackPush(Stack* st, StackElem_t value);

enum ReturnCode StackPop(Stack* st, StackElem_t* value);

enum ReturnCode StackDtor(Stack* st);

enum ReturnCode DumpPrint(FILE* filename, Stack* st,
                          const char *file, const char *func, int line, enum ReturnCode status);

enum ReturnCode StackResize(Stack* st, size_t new_size);

enum ReturnCode ReCalloc(Stack* st, size_t new_size);

size_t StackCheck(Stack* st, StackElem_t* find_value);

#endif // STACKMETHODS_H