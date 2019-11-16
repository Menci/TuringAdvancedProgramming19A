#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "compiler.h"

#define EXPRESSION_NUM  (60)
#define VARIABLE_NUM    (100)
#define ARGUMENT_NUM    (3)

enum exprType {VARIABLE, ARGUMENT, CONST};
typedef enum exprType exprType_t;

static const operator_t _[] = {
    ['+'] = ADD, ['-'] = SUB, ['*'] = MUL, ['/'] = DIV
};

typedef struct {
    exprType_t type;
    union {
        int id;
        long val;
    } number;
} number_t;

typedef struct {
    number_t val[3];
    char oper;
} expression_t;

expression_t lines[EXPRESSION_NUM];

int lastVisit[VARIABLE_NUM + ARGUMENT_NUM];
int isWho[REGISTER_NUM], visitCnt[REGISTER_NUM];
int whereVariable[VARIABLE_NUM], whereArgument[ARGUMENT_NUM];
int freeMem[VARIABLE_NUM + ARGUMENT_NUM], *top = freeMem;

#define BUFF_SIZE       (100)
void readLine(expression_t *target, int index) {
    static char buff[BUFF_SIZE], name[3][10], op;
    fgets(buff, BUFF_SIZE, stdin);
    sscanf(buff, "%s %*c %s %c %s", name[0], name[1], &op, name[2]);
    for(int i = 0; i < 3; i++) {
        if(isdigit(name[i][0]) || name[i][0] == '-' || name[i][0] == '+') {
            target -> val[i].type = CONST;
            sscanf(name[i], "%ld", &(target -> val[i].number.val));
        } else if(name[i][0] == 'a') {
            target -> val[i].type = ARGUMENT;
            sscanf(name[i] + 1, "%d", &(target -> val[i].number.id));
            target -> val[i].number.id -= 1;
            lastVisit[target -> val[i].number.id + VARIABLE_NUM] = index;
        } else {
            (*target).val[i].type = VARIABLE;
            sscanf(name[i] + 1, "%d", &(target -> val[i].number.id));
            target -> val[i].number.id -= 1;
            lastVisit[target -> val[i].number.id] = index;
        }
    }
    target -> oper = op;
}
#undef BUFF_SIZE

int getRegister() {
    int mnID = 0;
    for(int i = 1; i < REGISTER_NUM; i++) {
        if(visitCnt[i] < visitCnt[mnID]) {
            mnID = i;
        }
    }
    return mnID;
}

int getMemory() {
    static int memCnt = REGISTER_NUM;
    if(top != freeMem) return *--top;
    return memCnt++;
}

void moveToMemory(int ID, int regID) {
    int pos = getMemory();
    if(ID >= VARIABLE_NUM) {
        whereArgument[ID - VARIABLE_NUM] = pos;
    } else {
        whereVariable[ID] = pos;
    }
    asmOperator(regID, pos, MOV);
}

int main() {
    int exprNum = 60;
    compileInitialize();
    for(int i = 0; i < exprNum; i++) {
        readLine(lines + i, i);
    }
    memset(whereVariable, -1, sizeof(whereVariable));
    memset(isWho, -1, sizeof(isWho));
    for(int i = 0; i < ARGUMENT_NUM; i++) {
        int pos = i;
        whereArgument[i] = pos;
        isWho[pos] = VARIABLE_NUM + i;
        visitCnt[pos] = exprNum * 3;
    }
    for(int i = 0; i < exprNum; i++) {
        int target;
        if(lines[i].val[0].type == ARGUMENT) {
            int id = lines[i].val[0].number.id;
            if(whereArgument[id] == -1 || whereArgument[id] >= REGISTER_NUM) {
                int pos = getRegister();
                if(isWho[pos] != -1) {
                    moveToMemory(isWho[pos], pos);
                    visitCnt[pos] = 0;
                }
                if(whereArgument[id] != -1) {
                    *top++ = whereArgument[id];
                }
                whereArgument[id] = pos;
                isWho[pos] = VARIABLE_NUM + id;
            }
            target = whereArgument[id];
        } else {
            int id = lines[i].val[0].number.id;
            if(whereVariable[id] == -1 || whereVariable[id] >= REGISTER_NUM) {
                int pos = getRegister();
                if(isWho[pos] != -1) {
                    moveToMemory(isWho[pos], pos);
                }
                if(whereVariable[id] != -1) {
                    *top++ = whereVariable[id];
                }
                whereVariable[id] = pos;
                isWho[pos] = id;
            }
            target = whereVariable[id];
        }
        visitCnt[target] += 1;
        switch(lines[i].val[1].type) {
            case CONST:
                asmOperatori(lines[i].val[1].number.val, target, MOV);
                break;
            case ARGUMENT:
                asmOperator(whereArgument[lines[i].val[1].number.id], target, MOV);
                break;
            case VARIABLE:
                asmOperator(whereVariable[lines[i].val[1].number.id], target, MOV);
                break;
        }
        switch(lines[i].val[2].type) {
            case CONST:
                asmOperatori(lines[i].val[2].number.val, target, _[lines[i].oper]);
                break;
            case ARGUMENT:
                asmOperator(whereArgument[lines[i].val[2].number.id], target, _[lines[i].oper]);
                break;
            case VARIABLE:
                asmOperator(whereVariable[lines[i].val[2].number.id], target, _[lines[i].oper]);
                break;
        }

        submitAnswer(target);

        for(int id = 0; id < 3; id++) {
            if(lines[i].val[id].type == ARGUMENT) {
                int index = lines[i].val[id].number.id;
                if(i == lastVisit[index + VARIABLE_NUM]) {
                    if(whereArgument[index] < REGISTER_NUM) {
                        isWho[whereArgument[index]] = -1;
                    } else {
                        *top++ = whereArgument[index];
                    }
                }
            } else if(lines[i].val[id].type == VARIABLE) {
                int index = lines[i].val[id].number.id;
                if(i == lastVisit[index]) {
                    if(whereVariable[index] < REGISTER_NUM) {
                        isWho[whereVariable[index]] = -1;
                        visitCnt[whereVariable[index]] = 0;
                    } else {
                        *top++ = whereVariable[index];
                    }
                }
            }
        }
    }
    ending();
    return 0;
}
