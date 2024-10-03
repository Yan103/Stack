#ifndef STACKMETHODS_H
#define STACKMETHODS_H

#include <assert.h>
#include <stdio.h>

#include "Stack.h"
#include "ReturnCodes.h"

const int POISON_NUM = -666;
const int START_SIZE = 2;

#define STACK_INIT(st) { StackInit(&(st), __FILE__, __func__, __LINE__, #st); }

//* ========== IMPORTANT ==========
//* if you run the program in DEBUG mode (with the flag of the same name), additional information will be checked
//* else all these checks will be disabled
#ifdef DEBUG
    #define STACK_DUMP(st) { StackDump(st, __FILE__, __func__, __LINE__); }
    #define ASSERT(condition, text_error) my_assert(condition, text_error, __FILE__, __func__, __LINE__)
    #define STACK_ASSERT(st) {                                    \
    if (!StackOk(st)) {                                           \
        STACK_DUMP(st);                                           \
        assert(0);                                                \
    }                                                             \
}
    #define DEBUG_INFO(st) {                                      \
    (st)->debug_info = (DebugInfo*) calloc(1, sizeof(DebugInfo)); \
    ASSERT((st)->debug_info != NULL, ERR_MSG_NULL_PTR);           \
    (st)->debug_info->filename = file;                            \
    (st)->debug_info->func = func;                                \
    (st)->debug_info->line = line;                                \
    (st)->debug_info->var_name = var_name;                        \
}
#else
    #define STACK_DUMP(st) {}
    #define STACK_ASSERT(st) {}
    #define DEBUG_INFO(st) {}
    #define ASSERT(condition, text_error) {}
#endif

void StackKanaryCheck(Stack* st);

enum FuncReturn StackInit(Stack** st, const char *file, const char* func, int line, const char* var_name);

enum FuncReturn StackCtor(Stack* st, size_t capacity);

enum FuncReturn StackDump(Stack* st, const char *file, const char* func, int line);

int StackOk(Stack* st);

const char* StackStrErr(FuncReturn error);

enum FuncReturn StackPush(Stack* st, StackElem_t value);

enum FuncReturn StackPop(Stack* st, StackElem_t* value);

enum FuncReturn StackDtor(Stack* st);

enum FuncReturn DumpPrint(FILE* filename, Stack* st, const char *file, const char *func, int line);

enum FuncReturn StackResize(Stack* st, size_t new_size);

enum FuncReturn ReCalloc(Stack* st, size_t new_size);

size_t StackFind(Stack* st, StackElem_t find_value);

enum FuncReturn StackClean(Stack* st);

enum FuncReturn DumpErrorPrint(FILE* filename, Stack* st, const char *file, const char *func, int line);

size_t HashFunc1(Stack* st);

size_t HashFunc2(Stack* st);

void StackHashCheck(Stack* st);

enum FuncReturn StackUpdateHash(Stack* st);

#endif // STACKMETHODS_H