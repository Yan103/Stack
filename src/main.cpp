#include <stdio.h>

#include "Default.h"
#include "Stack.h"
#include "StackMethods.h"

int main() {
    Stack* st = NULL;
    STACK_INIT(st);

    //? todo fix warnings
    //* todo make st a pointer
    //* todo struct?
    //* todo POISON
    //! todo logs: 0xaddress
    // todo logs: handle corrupted debug data
    //* todo make poisons explicit: -666 (POISON)
    // todo make logs and checks disableable
    //? todo fix sanitizer PRIKOLY
    //? todo add err_bits (bit vector)

    StackCtor(st, START_SIZE);

    StackPush(st, 10);
    StackPush(st, 20);
    StackPush(st, 30);

    int x = -1;
    StackPop(st, &x);

    printf("%d\n", x);
    StackPop(st, &x);

    int y = 30;
    printf("%lld\n", StackFind(st, &y)); y = 20;
    StackPush(st, 20);
    printf("%lld\n", StackFind(st, &y));

    StackDtor(st);

    return SUCCESS;
}