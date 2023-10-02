#include <stdio.h>

#include "Stack.h"

int main()
{
    Stack stk = {};
    STACK_CTOR(&stk, 6);

    STACK_DUMP(&stk);

    STACK_DTOR(&stk);
    return 0;
}