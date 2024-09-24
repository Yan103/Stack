#include <stdio.h>

#include "StackStruct.h"
#include "StackMethods.h"
#include "default.h"

int main() {
    StackStruct st = {INIT(st)};

    StackCtor(&st, START_SIZE);

    StackPush(&st, 10);
    StackPush(&st, 20);
    StackPush(&st, 30);

    int x = -1;
    StackPop(&st, &x);

    printf("%d\n", x);
    StackPop(&st, &x);

    StackDtor(&st);

    return 0;
}