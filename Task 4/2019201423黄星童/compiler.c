#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define REGISTER_NUMBER 12
#define BUFFER_SIZE 20
#define MEMORY_NUMBER 1024
#define VARIABLE_NUMBER 60
#define EXPRESSION_NUMBER 60

typedef struct variable {
    int x;
    enum {
        VARIABLE,
        PARAMETER,
        CONSTANT
    }type;
}variable;

struct expression {
    variable x[3];
    char op;
}expr[EXPRESSION_NUMBER];

int nowVar, usefulVar[VARIABLE_NUMBER + 3];
char varNum[VARIABLE_NUMBER][20];

variable GetID(char x[]) {
    int len = strlen(x), number = 1;
    if (x[len - 1] == ';') {
        x[len - 1] = '\0';
        len--;
    }

    for (int i = 0; i < len; i++) {
        if (i == 0 && x[i] == '-')
            continue;
        if (!isdigit(x[i])) {
            number = 0;
            break;
        }
    }

    if (number) {                                   // It is a constant.
        int neg = 0, num = 0;
        for (int i = 0; i < len; i++) {
            if (x[i] == '-') {
                neg = 1;
                continue;
            }
            num = num * 10 + x[i] - '0';
        }
        if (neg) num = -num;
        return (variable) {num, CONSTANT};
    }

    if (x[0] == 'a')                                // It is a parameter.
        return (variable) {VARIABLE_NUMBER + x[1] - '1', PARAMETER};

    for (int i = 0; i < nowVar; i++)                // It is a variable.
        if (!strcmp(varNum[i], x))
            return (variable) {i, VARIABLE};
    strcpy(varNum[nowVar], x);
    nowVar++;
    return (variable) {nowVar - 1, VARIABLE};
}

void Read() {
    char x[3][20], op[5];
    int num = 0;
    while (~(scanf("%s = %s %s %s\n", x[0], x[1], op, x[2]))) {
        for (int i = 0; i < 3; i++) {
            expr[num].x[i] = GetID(x[i]);
            if (expr[num].x[i].type == VARIABLE || expr[num].x[i].type == PARAMETER)
                usefulVar[expr[num].x[i].x]++;
        }
        expr[num].op = op[0];
        num++;
    }
}

int reg[REGISTER_NUMBER], freeReg = REGISTER_NUMBER - 4;
int mem[MEMORY_NUMBER], freeMem = MEMORY_NUMBER;
int stored[VARIABLE_NUMBER + 3];

const char *REGISTER_NAME[] = {"%rbx", "%rcx", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%rdi", "%rsi", "%r14", "%r15"};

void Print(variable x, char *buffer) {
    if (x.type == CONSTANT)
        sprintf(buffer, "$%d", x.x);
    else {
        if (stored[x.x] < REGISTER_NUMBER)
            sprintf(buffer, "%s", REGISTER_NAME[stored[x.x]]);
        else
            sprintf(buffer, "%d(%%rbp)", -8 * (stored[x.x] - REGISTER_NUMBER));
    }
}

void PrintOperator(char op, char *buffer) {
    switch (op) {
        case '+':
            sprintf(buffer, "addq");
            break;
        case '-':
            sprintf(buffer, "subq");
            break;
        case '*':
            sprintf(buffer, "imulq");
    }
}

void PrintMove(variable x1, variable x2) {                                                           // movq x2, x1.
    char src[BUFFER_SIZE], dest[BUFFER_SIZE];
    Print(x2, src);
    Print(x1, dest);
    printf("movq %s, %s\n", src, dest);
}

void PrintExpression(variable x1, variable x2, char op) {                                            // ???q x2, x1.
    char opt[BUFFER_SIZE], src[BUFFER_SIZE], dest[BUFFER_SIZE];
    PrintOperator(op, opt);
    Print(x2, src);
    Print(x1, dest);
    printf("%s %s, %s\n", opt, src, dest);
}

void DoXor(variable x) {
    char src[BUFFER_SIZE];
    Print(x, src);
    printf("xorq %s, %%rax\n", src);
}

void AllocateSpace(int x) {
    if (stored[x] != -1)
        return;

    if (freeReg) {                                          // Put x into register.
        stored[x] = reg[freeReg - 1];
        freeReg--;
        return;
    }

    stored[x] = REGISTER_NUMBER + mem[freeMem - 1];         // Put x into memory.
    freeMem--;
    return;
}

void FreeUpSpace(variable x) {
    if (x.type == CONSTANT) return;

    usefulVar[x.x]--;
    if (usefulVar[x.x]) return;

    if (x.type == VARIABLE)
        DoXor(x);

    if (stored[x.x] < REGISTER_NUMBER) {
        reg[freeReg] = stored[x.x];
        freeReg++;
        stored[x.x] = -1;
        return;
    }

    mem[freeMem] = stored[x.x] - REGISTER_NUMBER;
    freeMem++;
    stored[x.x] = -1;
    return;
}

void CalleeSaved() {
    puts("pushq %rbp");
    puts("pushq %rsp");
    puts("pushq %rbx");
    puts("pushq %r12");
    puts("pushq %r13");
    puts("pushq %r14");
    puts("pushq %r15");
    puts("movq %rsp, %rbp");
}

void RecoverCalleeSaved() {
    puts("popq %r15");
    puts("popq %r14");
    puts("popq %r13");
    puts("popq %r12");
    puts("popq %rbx");
    puts("popq %rsp");
    puts("popq %rbp");
}

void Divide(variable x0, variable x1, variable x2) {
    char buffer[BUFFER_SIZE];
    AllocateSpace(x0.x);
    printf("movq %%rax, %d(%%rbp)\n", -8 * mem[freeMem - 1]);                                     // Move %rax into memory.
    printf("movq %%rdx, %d(%%rbp)\n", -8 * mem[freeMem - 2]);                                     // Move %rdx into memory.

    Print(x1, buffer);
    printf("movq %s, %%rax\n", buffer);
    printf("cqto\n");
    if (x2.type == CONSTANT) {
        printf("movq $%d, %d(%%rbp)\n", x2.x, -8 * mem[freeMem - 3]);
        printf("idivq %d(%%rbp)\n", -8 * mem[freeMem - 3]);
    }
    else {
        Print(x2, buffer);
        printf("idivq %s\n", buffer);
    }
    Print(x0, buffer);
    printf("movq %%rax, %s\n", buffer);

    printf("movq %d(%%rbp), %%rax\n", -8 * mem[freeMem - 1]);
    printf("movq %d(%%rbp), %%rdx\n", -8 * mem[freeMem - 2]);

    FreeUpSpace(x0);
    FreeUpSpace(x1);
    FreeUpSpace(x2);
}

void Convert() {
    CalleeSaved();

    memset(stored, -1, sizeof(stored));

    for (int i = 0; i < REGISTER_NUMBER; i++)
        reg[i] = i;
    for (int i = 0; i < MEMORY_NUMBER; i++)
        mem[i] = MEMORY_NUMBER - i;

    stored[VARIABLE_NUMBER] = REGISTER_NUMBER - 4;
    stored[VARIABLE_NUMBER + 1] = REGISTER_NUMBER - 4 + 1;
    stored[VARIABLE_NUMBER + 2] = REGISTER_NUMBER - 4 + 2;

    puts("movq %rdx, %r14");
    puts("movq $0, %rax");

    for (int i = 0; i < EXPRESSION_NUMBER; i++) {
        variable x[3];
        x[0] = expr[i].x[0], x[1] = expr[i].x[1], x[2] = expr[i].x[2];

        if (expr[i].op == '/') {
            Divide(x[0], x[1], x[2]);
            continue;
        }

        int pre = -1;
        AllocateSpace(x[0].x);
        if (stored[x[0].x] > REGISTER_NUMBER) {
            pre = stored[x[0].x];
            stored[x[0].x] = REGISTER_NUMBER - 1;
        }

        PrintMove(x[0], x[1]);
        PrintExpression(x[0], x[2], expr[i].op);

        if (pre != -1) {
            printf("movq %%r15, %d(%%rbp)\n", -8 * (pre - REGISTER_NUMBER));
            stored[x[0].x] = pre;
        }

        FreeUpSpace(x[0]);
        FreeUpSpace(x[1]);
        FreeUpSpace(x[2]);
        continue;
    }

    RecoverCalleeSaved();
    puts("ret");
}

int main() {
    Read();
    Convert();
    return 0;
}