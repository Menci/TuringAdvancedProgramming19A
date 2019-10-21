#ifndef STACK_H_
#define STACK_H_

#include <stdbool.h>

#define STACK_MAX_SIZE (1000000 * 5)

typedef struct {
    char val[STACK_MAX_SIZE];
    int top;
} rpn_stack;

void stackInitialize(rpn_stack*);

void stackPush(rpn_stack*, char);
void stackPop(rpn_stack*);

bool stackIsEmpty(rpn_stack*);

int stackTop(rpn_stack*);

#ifdef _LOCAL_DEBUG_
void stackPrint(rpn_stack*);
#endif

#endif