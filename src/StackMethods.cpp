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

enum FuncReturn StackInit(Stack** st, const char *file, const char *func, int line, const char* var_name) {
    ASSERT(st   != NULL, ERR_MSG_NULL_PTR);

    (*st) = (Stack*) calloc(1, sizeof(Stack));
    ASSERT(st   != NULL, ERR_MSG_NULL_PTR);

    DEBUG_INFO(*st);

    return SUCCESS;
}
// todo 1 func
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
        fprintf(log_filename, "STACK WAS DESTROYED!!!\n\n");
    }
    fclose(log_filename);

    return SUCCESS;
}

static tm GetTime() {
    time_t time_now = time(NULL);

    return *localtime(&time_now);
}

enum FuncReturn DumpErrorPrint(FILE* filename, Stack* st, const char *file, const char *func, int line) {
    ASSERT(filename != NULL, ERR_MSG_NULL_PTR);
    ASSERT(st       != NULL, ERR_MSG_NULL_PTR);
    ASSERT(file     != NULL, ERR_MSG_NULL_PTR);
    ASSERT(func     != NULL, ERR_MSG_NULL_PTR);

    struct tm tm = GetTime();

    fprintf(filename, "%-30s ERROR %d-%02d-%02d %02d:%02d:%02d %s\n",
            "=", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, "=");
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

int StackOk(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    return st->debug_info->err_bits == 0;
}

const char* StackStrErr(FuncReturn error) {
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

    STACK_DUMP(st);

    st->data[st->size] = value;
    st->size++;

    StackUpdateHash(st);
    StackKanaryCheck(st);
    STACK_ASSERT(st);
    STACK_DUMP(st);

    return SUCCESS;
}

enum FuncReturn StackPop(Stack* st, StackElem_t* value) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);
    StackHashCheck(st);
    StackKanaryCheck(st);
    STACK_ASSERT(st);
    ASSERT(value != NULL, ERR_MSG_NULL_PTR);
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

enum FuncReturn StackDtor(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);
    StackHashCheck(st);
    StackKanaryCheck(st);
    STACK_ASSERT(st);

    FREE(st);

    STACK_DUMP(st);

    return SUCCESS;
}

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


void StackKanaryCheck(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    if (st->RightKanary != st->LeftKanary) {
        st->debug_info->err_bits |= KANARY_DAMAGED;
    }
}

size_t HashFunc1(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);
    StackKanaryCheck(st);
    STACK_ASSERT(st);

    size_t hash = 0;
    for (size_t i = 0; i < st->size; i++) {
        hash = (size_t)st->data[i] + (hash << 16) + (hash >> 7) - hash;
    }

    return hash;
}
// todo func with for (void*)
size_t HashFunc2(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);
    StackKanaryCheck(st);
    STACK_ASSERT(st);

    size_t hash = 1;
    for (size_t i = 0; i < st->size; i++) {
        hash = (size_t)st->data[i] * (hash << 11) + (hash >> 4) - hash;
    }

    return hash;
}

void StackHashCheck(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    if (st->hash1 != HashFunc1(st) || st->hash2 != HashFunc2(st)) {
        st->debug_info->err_bits |= HASH_ERROR;
    }
}

enum FuncReturn StackUpdateHash(Stack* st) {
    ASSERT(st != NULL, ERR_MSG_NULL_PTR);

    st->hash1 = HashFunc1(st);
    st->hash2 = HashFunc2(st);

    return SUCCESS;
}
