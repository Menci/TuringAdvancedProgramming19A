#ifndef COMPILE_H_

#define COMPILE_H_

#define REGISTER_NUM    (9)

typedef enum {ADD, SUB, MUL, DIV, MOV} operator_t;

void asmOperator(int src, int tar, operator_t operType);
void asmOperatori(long val, int tar, operator_t operType);

void compileInitialize();
void submitAnswer(int);
void ending();

#endif