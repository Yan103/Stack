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
    }                                    \
}                                        \

enum FuncReturn StackInit(Stack** st, const char *file, const char *func, int line, const char* var_name);

enum FuncReturn StackCtor(Stack* st, size_t capacity);

enum FuncReturn StackDump(Stack* st, const char *file, const char *func, int line, FuncReturn status);

int StackOk(Stack* st);

const char* StackStrErr(FuncReturn error);

enum FuncReturn StackPush(Stack* st, StackElem_t value);

enum FuncReturn StackPop(Stack* st, StackElem_t* value);

enum FuncReturn StackDtor(Stack* st);

enum FuncReturn DumpPrint(FILE* filename, Stack* st,
                          const char *file, const char *func, int line, FuncReturn status);

enum FuncReturn StackResize(Stack* st, size_t new_size);

enum FuncReturn ReCalloc(Stack* st, size_t new_size);

size_t StackFind(Stack* st, StackElem_t* find_value);

#endif // STACKMETHODS_H