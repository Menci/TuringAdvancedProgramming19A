#include <assert.h>
#include <stdio.h>

#include "stack.h"

void stackInitialize(rpn_stack *stk) {
    stk -> top = -1;
}

void stackPush(rpn_stack *stk, char _val) {
    assert(stk -> top < STACK_MAX_SIZE);
    stk -> top += 1;
    stk -> val[stk -> top] = _val;
}

void stackPop(rpn_stack *stk) {
    assert(stk -> top >= 0);
    stk -> top -= 1;
}

bool stackIsEmpty(rpn_stack *stk) {
    return stk -> top == -1;
}

int stackTop(rpn_stack *stk) {
    return stk -> val[stk -> top];
}

#ifdef _LOCAL_DEBUG_

void stackPrint(rpn_stack *stk) {
    printf("Stack info:\n");
    for(int i = stk -> top; i >= 0; i--) {
        printf("%d\n", stk -> val[i]);
    }
}

#endif
