#include "stack.h"
#include "hash.h"
#include "StackDB.h"



int main()
{

    #ifndef NDEBUG //will make filename an argument in stack init later
    FILE* clean = fopen("log.txt", "w+b");
    fclose(clean);
    #endif

    Stack_t stk = {};
    stk.elSize = 4; // BETTER USE WITH INTS ONLY CAUSE DUMP CURRENTLY WORKS WITH INTS ONLY


// code example
    int x = 777;
    int* xptr = &x;
    STACK_INIT(&stk, 8);
    VoidIntDump(&stk);
    StackPush(&stk, xptr);
    VoidIntDump(&stk);
    StackPush(&stk, xptr);
    StackPop(&stk, xptr);
    VoidIntDump(&stk);
    StackDtor(&stk);
// code example
    return 0;
}

