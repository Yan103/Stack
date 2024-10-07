/*!
    \file
    File with the main function
*/

#include <stdio.h>

#include "Default.h"
#include "Stack.h"
#include "StackMethods.h"

int main() {
    Stack* st = STACK_INIT();

    //* todo update DumpPrint function (cringe)
    //* todo do 2 hash func and hash_update
    //* todo fix warnings
    //* todo make st a pointer
    //* todo struct?
    //* todo POISON
    //* todo logs: 0xaddress
    //* todo logs: handle corrupted debug data
    //* todo make poisons explicit: -666 (POISON)
    //* todo make logs and checks disableable
    //* todo fix sanitizer PRIKOLY
    //* todo add err_bits (bit vector)

    StackPush(st, 10);
    StackPush(st, 20);
    StackPush(st, 30);

    ////st->debug_info->err_bits |= BAD_FILE;
    ////st->debug_info->err_bits |= STACK_OVERFLOW;;

    StackClean(st);

    int y = 30;
    printf("%lu\n", StackFind(st, y)); y = 20;
    StackPush(st, 20);
    printf("%lu\n", StackFind(st, y));

    StackDtor(st);

    return SUCCESS;
}