#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Default.h"
#include "StackMethods.h"
#include "Stack.h"
#include "ReturnCodes.h"

const char* LOG_FILE         = "log.txt";
const char* ERR_MSG_NULL_PTR = "Null pointer was passed";

enum ReturnCode StackInit(Stack** st, const char *file, const char *func, int line, const char* var_name) {
    ASSERT(st   != NULL, "NULL POINTER!");
    ASSERT(file != NULL, ERR_MSG_NULL_PTR);
    ASSERT(file != NULL, ERR_MSG_NULL_PTR);

    (*st) = (Stack*) calloc(1, sizeof(Stack));
    STACK_ASSERT(*st, BAD_PTR);

    (*st)->debug_info = (DebugInfo*) calloc(1, sizeof(DebugInfo));
    STACK_ASSERT(*st, BAD_PTR);

    (*st)->debug_info->filename = file;
    (*st)->debug_info->func = func;
    (*st)->debug_info->line = line;
    (*st)->debug_info->var_name = var_name;

    return SUCCESS;
}

enum ReturnCode StackCtor(Stack* st, size_t capacity) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);
    STACK_DUMP(st, CTOR_START);

    st->data = 0;
    st->capacity = capacity;

    st->data = (StackElem_t*) calloc(capacity, sizeof(StackElem_t));
    STACK_ASSERT(st, BAD_PTR);
    for (size_t i = 0; i < capacity; i++) st->data[i] = POISON_NUM;

    STACK_DUMP(st, CTOR_END);

    return SUCCESS;
}

enum ReturnCode StackDump(Stack* st, const char* file, const char* func, int line, enum ReturnCode status) {
    STACK_ASSERT(st, BAD_PTR);
    ASSERT(file != NULL, ERR_MSG_NULL_PTR);
    ASSERT(func != NULL, ERR_MSG_NULL_PTR);

    FILE* log_filename = fopen(LOG_FILE, "a");

    if (!log_filename) {
        printf(RED("Error occured while opening file\n"));
        STACK_DUMP(st, BAD_FILE);

        return BAD_FILE;
    }

    DumpPrint(log_filename, st, file, func, line, status);
    fclose(log_filename);

    return SUCCESS;
}

static tm GetTime() {
    time_t time_now = time(NULL);

    return *localtime(&time_now);
}

enum ReturnCode DumpPrint(FILE* filename, Stack* st,
                          const char *file, const char *func, int line, enum ReturnCode status) {
    STACK_ASSERT(st, BAD_PTR);
    ASSERT(filename != NULL, ERR_MSG_NULL_PTR);
    ASSERT(file     != NULL, ERR_MSG_NULL_PTR);
    ASSERT(func     != NULL, ERR_MSG_NULL_PTR);

    struct tm tm = GetTime();

    fprintf(filename, "DUMPED %d-%02d-%02d %02d:%02d:%02d\nStack [%p] {%s} %s: %d, from %s: %d (%s -> %s) {\n\
            size = %lld,\n\
            capacity = %lld,\n\
            data [%p] {\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
            st, st->debug_info->var_name, st->debug_info->filename, st->debug_info->line, file, line, func,
            StackStrErr(status), st->size, st->capacity, st->data);

    for (size_t i = 0; i < st->capacity; i++) {
        if (i < st->size) {
            fprintf(filename, "%14c * [%lld] %d\n", ' ', i, st->data[i]);
        } else {
            fprintf(filename, "%16c [%lld] %d (POISON)\n", ' ', i, st->data[i]);
        }
    }
    fprintf(filename, "%12c}\n}\n\n", ' ');

    return SUCCESS;
}

int StackOk(Stack* st) {  // todo add more checks, size < capacity, poisons after size O(N)
    // todo invent 2 more checks
    return (st != NULL);        //*(st != NULL && st->data != NULL)
}

const char* StackStrErr(enum ReturnCode error) {
    #define DESCR_(error) { case error: return #error; }
    switch (error) {
        DESCR_(SUCCESS);
        DESCR_(BAD_FILE);
        DESCR_(BAD_PTR);
        DESCR_(BAD_SIZE);
        DESCR_(ERROR);
        DESCR_(EMPTY_STACK);
        DESCR_(DTOR_START);   DESCR_(DTOR_END);
        DESCR_(POP_START);    DESCR_(POP_END);
        DESCR_(PUSH_START);   DESCR_(PUSH_END);
        DESCR_(CTOR_START);   DESCR_(CTOR_END);
        DESCR_(RESIZE_START); DESCR_(RESIZE_END);
        default: return "UNKNOWN STATUS";
    }
    #undef DESCR_
}

enum ReturnCode StackPush(Stack* st, StackElem_t value) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, PUSH_START);

    if (st->size >= st->capacity) StackResize(st, st->capacity * 2);

    st->data[st->size] = value;
    st->size++;

    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, PUSH_END);

    return SUCCESS;
}

enum ReturnCode StackPop(Stack* st, StackElem_t* value) {
    STACK_ASSERT(st, BAD_PTR);
    ASSERT(value != NULL, ERR_MSG_NULL_PTR);
    STACK_DUMP(st, POP_START);

    if (st->size > 0) {
        *value = st->data[st->size - 1];
        st->data[st->size - 1] = POISON_NUM;
        st->size--;
    } else {
        STACK_DUMP(st, EMPTY_STACK);
        return EMPTY_STACK;
    }

    if (st->size <= st->capacity / 4) StackResize(st, st->capacity / 2);

    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, POP_END);

    return SUCCESS;
}

enum ReturnCode StackDtor(Stack* st) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, DTOR_START);

    FREE(st->data);
    st->size = 0;
    st->capacity = 0;

    STACK_DUMP(st, DTOR_END);

    return SUCCESS;
}

enum ReturnCode StackResize(Stack* st, size_t new_size) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, RESIZE_START);

    ReCalloc(st, new_size);

    STACK_DUMP(st, RESIZE_END);

    return SUCCESS;
}

enum ReturnCode ReCalloc(Stack* st, size_t new_size) {
    STACK_ASSERT(st, BAD_PTR);

    st->data = (StackElem_t*) realloc(st->data, sizeof(StackElem_t) * new_size);
    ASSERT(st->data != NULL, ERR_MSG_NULL_PTR);

    if (new_size > st->capacity) for (size_t i = st->size; i < new_size; i++) st->data[i] = POISON_NUM;
    st->capacity = new_size;

    return SUCCESS;
}

size_t StackCheck(Stack* st, StackElem_t* find_value) {
    STACK_ASSERT(st, BAD_PTR);
    ASSERT(find_value != NULL, ERR_MSG_NULL_PTR);

    for (size_t i = 0; i < st->size; i++)
        if (st->data[i] == *find_value)
            return i;

    return st->capacity;
}