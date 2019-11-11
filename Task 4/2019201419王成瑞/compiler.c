#include <assert.h>
#include <stdio.h>

#include "compiler.h"

const static char command[][7] = {
    [ADD] = "addq", [SUB] = "subq", [MUL] = "imulq", [DIV] = "idivq", [MOV] = "movq"
};

const static char registers[][10] = {
    "%rdi", "%rsi", "%rcx", "%rbx", "%r8", 
    "%r9", "%r12", "%r13", "%rbp"
};
const static char returnVal[] = "%r15";
const static char stackPointer[] = "%rsp";
const static char speial[] = "%r14";

void compileInitialize() {
    puts("push\t%rbx");
    puts("push\t%rbp");
    puts("push\t%r12");
    puts("push\t%r13");
    puts("push\t%r14");
    puts("push\t%r15");
    printf("movq\t$0,\t%s\n", returnVal);
    puts("movq\t%rdx,\t%rcx");
}

void asmOperator(int srcID, int tarID, operator_t operType) {
    assert(srcID >= 0 && tarID >= 0);
    if(operType == DIV) {
        assert(tarID < REGISTER_NUM);
        printf("movq\t%s,\t%%rax\n", registers[tarID]);
        puts("cqto");
        printf("%s\t", command[operType]);
        if(srcID < REGISTER_NUM) {
            printf("%s\n", registers[srcID]);
        } else {
            printf("-%d(%s)\n", 8 * (srcID - REGISTER_NUM + 1), stackPointer);
        }
        printf("movq\t%%rax,\t%s\n", registers[tarID]);
    } else {
        printf("%s\t", command[operType]);
        if(srcID < REGISTER_NUM) {
            printf("%s,\t", registers[srcID]);
        } else {
            printf("-%d(%s),\t", 8 * (srcID - REGISTER_NUM + 1), stackPointer);
        }
        if(tarID < REGISTER_NUM) {
            printf("%s\n", registers[tarID]);
        } else {
            printf("-%d(%s)\n", 8 * (tarID - REGISTER_NUM + 1), stackPointer);
        }
    }
}

void asmOperatori(long val, int tarID, operator_t operType) {
    assert(tarID >= 0);
    if(operType == DIV) {
        assert(tarID < REGISTER_NUM);
        printf("movq\t%s,\t%%rax\n", registers[tarID]);
        printf("movq\t$%ld,\t%s\n", val, speial);
        puts("cqto");
        printf("%s\t%s\n", command[operType], speial);
        printf("movq\t%%rax,\t%s\n", registers[tarID]);
    } else {
        printf("%s\t$%ld,\t", command[operType], val);
        if(tarID < REGISTER_NUM) {
            printf("%s\n", registers[tarID]);
        } else {
            printf("-%d(%s)\n", 8 * (tarID - REGISTER_NUM + 1), stackPointer);
        }
    }
}

void submitAnswer(int target) {
    assert(target >= 0 && target < REGISTER_NUM);
    printf("xorq\t%s,\t%s\n", registers[target], returnVal);
}

void ending() {
    printf("movq\t%s,\t%%rax\n", returnVal);
    puts("pop\t%r15");
    puts("pop\t%r14");
    puts("pop\t%r13");
    puts("pop\t%r12");
    puts("pop\t%rbp");
    puts("pop\t%rbx");
    puts("ret");
}
