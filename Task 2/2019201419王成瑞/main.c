#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "double.h"
#include "doubleio.h"
#include "stack.h"


#define MAX_NUMBER_CNT (100000)
#define INPUT_MAX_LENGTH (MAX_NUMBER_CNT * 200)


char input[INPUT_MAX_LENGTH], operator[MAX_NUMBER_CNT];
double_t number[MAX_NUMBER_CNT], stk[MAX_NUMBER_CNT];

int L[MAX_NUMBER_CNT * 2];
enum Ltype {OPERTAOR, NUMBER} type[MAX_NUMBER_CNT * 2];

bool readLine() {
    input[0] = '(';
    char *ptr = input + 1, c;
    while((c = getchar()) != '\n') {
        if(c == EOF) return false;
        if(c != ' ') *ptr++ = c;
    }
    *ptr++ = ')';
    *ptr++ = 0;
    return true;
}

bool isOperator(char s) {
    return s == '+' || s == '-' || s == '*' || s == '/';
}
bool isSignedBegin(char *const ptr) {
    return (*ptr == '+' || *ptr == '-') && (isOperator(*(ptr - 1)) || *(ptr - 1) == '(');
}

int getPriority(char op) {
    switch(op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return -1;
    }
}

int main() {
    while(readLine()) {
        rpn_stack *sop = (rpn_stack*)malloc(sizeof(rpn_stack));
        stackInitialize(sop);

        int cntL = 0, cntOp = 0, cntNum = 0;
        for(char *str = input; *str; str++) {
            if(isdigit(*str) || isSignedBegin(str)) {
                char *tmp = str + 1;
                while(isdigit(*tmp) || *tmp == '.') tmp++;
                type[cntL] = NUMBER, L[cntL] = cntNum;
                cntL++;
                number[cntNum++] = readDouble(str, tmp);
                str = tmp - 1;
            } else if(isOperator(*str)) {
                while(!stackIsEmpty(sop) && isOperator(stackTop(sop)) && getPriority(*str) <= getPriority(stackTop(sop))) {
                    type[cntL] = OPERTAOR, L[cntL] = cntOp;
                    cntL++;
                    operator[cntOp++] = stackTop(sop);
                    stackPop(sop);
                }
                stackPush(sop, *str);
            } else if(*str == '(') {
                stackPush(sop, '(');
            } else if(*str == ')') {
                while(!stackIsEmpty(sop) && stackTop(sop) != '(') {
                    type[cntL] = OPERTAOR, L[cntL] = cntOp;
                    cntL++;
                    operator[cntOp++] = stackTop(sop);
                    stackPop(sop);
                }
                assert(stackTop(sop) == '(');
                stackPop(sop);
            }
        }
        while(!stackIsEmpty(sop)) {
            type[cntL] = OPERTAOR, L[cntL] = cntOp;
            cntL++;
            operator[cntOp++] = stackTop(sop);
            stackPop(sop);
        }

        double_t *top = stk;
        for(int i = 0; i < cntL; i++) {
            if(type[i] == OPERTAOR) {
                char op = operator[L[i]];
                double_t y = *--top;
                double_t x = *--top;
                if(op == '+') *top++ = doubleAdd(x, y);
                else if(op == '-') *top++ = doubleMin(x, y);
                else if(op == '*') *top++ = doubleMul(x, y);
                else if(op == '/') *top++ = doubleDiv(x, y);
            } else {
                *top++ = number[L[i]];
            }
        }
        assert(top - stk == 1);

        printDouble(*--top);
        fflush(stdout);

        free(sop);
    }
    return 0;
}
