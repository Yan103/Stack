#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "StackMethods.h"
#include "StackStruct.h"
#include "Errors.h"
#include "default.h"

enum ErrorsCode StackCtor(StackStruct* st, size_t capacity) {
    assert(st != NULL);

    st->data = 0;
    st->capacity = capacity;

    st->data = (StackElem_t*) calloc(capacity, sizeof(StackElem_t));
    STACK_ASSERT(st, BAD_PTR);

    STACK_DUMP(st, SUCCESS);

    return SUCCESS;
}

enum ErrorsCode StackDump(StackStruct* st, const char *file, const char *func, int line, enum ErrorsCode status) {
    STACK_ASSERT(st, BAD_PTR);

    FILE* log_filename = fopen("log.txt", "a");

    if (!log_filename) {
        printf(RED("Error occured while opening file\n"));
        STACK_DUMP(st, BAD_FILE);

        return BAD_FILE;
    }

    time_t time_now = time(NULL); struct tm tm = *localtime(&time_now);

    fprintf(log_filename, "DUMPED %d-%02d-%02d %02d:%02d:%02d\nStackStruct [%p] {%s} %s: %d, from %s: %d (%s -> %s) {\n\
            size = %lld,\n\
            capacity = %lld,\n\
            data [%p] {\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
            st, st->var_name, st->filename, st->line, file, line, func,
            StackStrErr(status), st->size, st->capacity, st->data);

    for (size_t i = 0; i < st->capacity; i++) {
        if (i < st->size) {
            fprintf(log_filename, "%14c * [%lld] %d\n", ' ', i, st->data[i]);
        } else {
            fprintf(log_filename, "%16c [%lld] %d\n", ' ', i, POISON_NUM);
        }
    }

    fprintf(log_filename, "%12c}\n}\n\n", ' '); fclose(log_filename);

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

    if (st->size >= st->capacity) { //todo new func and recalloc
        st->data = (StackElem_t*) realloc(st->data, sizeof(StackElem_t) * 2 * st->capacity);
        st->capacity *= 2;
    }

    st->data[st->size] = value;
    st->size++;

    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    return SUCCESS;
}

enum ErrorsCode StackPop(StackStruct* st, StackElem_t* value) {
    STACK_ASSERT(st, BAD_PTR);
    STACK_DUMP(st, SUCCESS);

    if (st->size > 0) {
        *value = st->data[st->size - 1];
        st->size--;
    } else {
        STACK_DUMP(st, EMPTY_STACK);
        return EMPTY_STACK;
    }

    if (st->size < st->capacity / 2) { //todo new func and recalloc
        st->data = (StackElem_t*) realloc(st->data, sizeof(StackElem_t) * st->capacity / 2);
        st->capacity /= 2;
    }

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
