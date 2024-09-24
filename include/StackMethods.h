#ifndef STACKMETHODS_H
#define STACKMETHODS_H

#include <stdio.h>
#include <assert.h>

#include "StackStruct.h"
#include "Errors.h"

const int POISON_NUM = -666;
const int START_SIZE = 2;

#define INIT(st) st.filename = __FILE__, st.line = __LINE__, st.var_name = #st

#define STACK_DUMP(st, status) { StackDump(st, __FILE__, __func__, __LINE__, status); }

#define STACK_ASSERT(st, status) {       \
    if (!StackOk(st)) {                  \
        STACK_DUMP(st, status);          \
        assert(0);                       \
        return ERROR;                    \
    }                                    \
    }                                    \

enum ErrorsCode StackCtor(StackStruct* st, size_t capacity);

enum ErrorsCode StackDump(StackStruct* st, const char *file, const char *func, int line, enum ErrorsCode status);

int StackOk(StackStruct* st);

const char* StackStrErr(enum ErrorsCode error);

enum ErrorsCode StackPush(StackStruct* st, StackElem_t value);

enum ErrorsCode StackPop(StackStruct* st, StackElem_t* value);

enum ErrorsCode StackDtor(StackStruct* st);

#endif // STACKMETHODS_H