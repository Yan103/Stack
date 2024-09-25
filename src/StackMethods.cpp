#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "StackMethods.h"
#include "StackStruct.h"
#include "Errors.h"
#include "default.h"

const char* log_file = "log.txt";
const char* ERR_MSG_NULL_PTR = "Null pointer was passed";

enum ErrorsCode StackCtor(StackStruct* st, size_t capacity) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    st->data = 0;
    st->capacity = capacity;

    st->data = (StackElem_t*) calloc(capacity, sizeof(StackElem_t));
    STACK_ASSERT(st, BAD_PTR);

    STACK_DUMP(st, SUCCESS);

    return SUCCESS;
}

enum ErrorsCode StackDump(StackStruct* st, const char *file, const char *func, int line, enum ErrorsCode status) {
    STACK_ASSERT(st, BAD_PTR);
    ASSERT(file != NULL, ERR_MSG_NULL_PTR);
    ASSERT(func != NULL, ERR_MSG_NULL_PTR);

    FILE* log_filename = fopen(log_file, "a");

    if (!log_filename) {
        printf(RED("Error occured while opening file\n"));
        STACK_DUMP(st, BAD_FILE);

        return BAD_FILE;
    }

    DumpPrint(log_filename, st, file, func, line, status);
    fclose(log_filename);

    return SUCCESS;
}

enum ErrorsCode DumpPrint(FILE* filename, StackStruct* st,
                          const char *file, const char *func, int line, enum ErrorsCode status) {
    STACK_ASSERT(st, BAD_PTR);
    ASSERT(filename != NULL, ERR_MSG_NULL_PTR);
    ASSERT(file     != NULL, ERR_MSG_NULL_PTR);
    ASSERT(func     != NULL, ERR_MSG_NULL_PTR);

    time_t time_now = time(NULL); struct tm tm = *localtime(&time_now);

    fprintf(filename, "DUMPED %d-%02d-%02d %02d:%02d:%02d\nStackStruct [%p] {%s} %s: %d, from %s: %d (%s -> %s) {\n\
            size = %lld,\n\
            capacity = %lld,\n\
            data [%p] {\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
            st, st->var_name, st->filename, st->line, file, line, func,
            StackStrErr(status), st->size, st->capacity, st->data);

    for (size_t i = 0; i < st->capacity; i++) {
        if (i < st->size) {
            fprintf(filename, "%14c * [%lld] %d\n", ' ', i, st->data[i]);
        } else {
            fprintf(filename, "%16c [%lld] %d\n", ' ', i, POISON_NUM);
        }
    }
    fprintf(filename, "%12c}\n}\n\n", ' ');

    return SUCCESS;
}

int StackOk(StackStruct* st) {
    return (st != NULL);        //*(st != NULL && st->data != NULL)
}

const char* StackStrErr(enum ErrorsCode error) {
    #define DESCR_(error) { case error: return #error; }
    switch (error) {
        DESCR_(BAD_FILE);
        DESCR_(BAD_PTR);
        DESCR_(BAD_SIZE);
        DESCR_(ERROR);
        DESCR_(SUCCESS);
        DESCR_(EMPTY_STACK);
        DESCR_(CLEAN_STACK);
    }
    #undef DESCR_

    return "ABOBA";
}

enum ErrorsCode StackPush(StackStruct* st, StackElem_t value) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    if (st->size >= st->capacity) StackResize(st, st->capacity * 2);

    st->data[st->size] = value;
    st->size++;

    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    return SUCCESS;
}

enum ErrorsCode StackPop(StackStruct* st, StackElem_t* value) {
    STACK_ASSERT(st, BAD_PTR);
    ASSERT(value != NULL, ERR_MSG_NULL_PTR);
    STACK_DUMP(st, SUCCESS);

    if (st->size > 0) {
        *value = st->data[st->size - 1];
        st->size--;
    } else {
        STACK_DUMP(st, EMPTY_STACK);
        return EMPTY_STACK;
    }

    if (st->size < st->capacity / 4) ReCalloc(st, st->capacity / 2);

    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    return SUCCESS;
}

enum ErrorsCode StackDtor(StackStruct* st) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    FREE(st->data);
    st->size = 0;
    st->capacity = 0;

    STACK_DUMP(st, CLEAN_STACK);

    return CLEAN_STACK;
}

enum ErrorsCode StackResize(StackStruct* st, size_t new_size) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    ReCalloc(st, new_size);

    STACK_DUMP(st, SUCCESS);

    return SUCCESS;
}

enum ErrorsCode ReCalloc(StackStruct* st, size_t new_size) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    st->data = (StackElem_t*) realloc(st->data, sizeof(StackElem_t) * new_size);
    ASSERT(st->data != NULL, ERR_MSG_NULL_PTR);

    memset(st->data+st->capacity, 0, st->capacity);
    st->capacity = new_size;

    return SUCCESS;
}