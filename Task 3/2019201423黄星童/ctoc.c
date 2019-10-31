#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define REGISTER_NUMBER 16
#define MEMORY_NUMBER 1024
#define VARIABLE_NUMBER 60
#define EXPRESSION_NUMBER 60

typedef struct variable {
    int x, type;
}variable;

struct expression {
    variable x[3];
    char op;
}expr[EXPRESSION_NUMBER];

int nowVar, usefulVar[VARIABLE_NUMBER];
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
        return (variable) {num, 2};
    }

    if (x[0] == 'a')                                // It is a parameter.
        return (variable) {VARIABLE_NUMBER + x[1] - '1', 1};

    for (int i = 0; i < nowVar; i++)                // It is a variable.
        if (!strcmp(varNum[i], x))
            return (variable) {i, 0};
    strcpy(varNum[nowVar], x);
    nowVar++;
    return (variable) {nowVar - 1, 0};
}

void Read() {
    char x[3][20], op[5];
    int num = 0;
    while (~(scanf("%s = %s %s %s\n", x[0], x[1], op, x[2]))) {
        for (int i = 0; i < 3; i++) {
            expr[num].x[i] = GetID(x[i]);
            if (!expr[num].x[i].type)
                usefulVar[expr[num].x[i].x]++;
        }
        expr[num].op = op[0];
        num++;
    }
}

int reg[REGISTER_NUMBER], freeReg = REGISTER_NUMBER - 4;
int mem[MEMORY_NUMBER], freeMem = MEMORY_NUMBER;
int stored[VARIABLE_NUMBER + 3];

void AllocateSpace(int x) {
    if (stored[x])
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

void FreeUpSpace(int x) {
    usefulVar[x]--;
    if (usefulVar[x]) return;

    if (stored[x] <= REGISTER_NUMBER) {
        printf("    printf(%cv%d = %c%c%c%c%c, reg[%d]);\n", '"', x + 1, '%', 'd', '\\', 'n', '"', stored[x]);
        reg[freeReg] = stored[x];
        freeReg++;
        stored[x] = 0;
        return;
    }

    printf("    printf(%cv%d = %c%c%c%c%c, mem[%d]);\n", '"', x + 1, '%', 'd', '\\', 'n', '"', stored[x] - REGISTER_NUMBER);
    mem[freeMem] = stored[x] - REGISTER_NUMBER;
    freeMem++;
    stored[x] = 0;
    return;
}

void Print(int x) {
    if (stored[x] <= REGISTER_NUMBER)
        printf("reg[%d]", stored[x]);
    else
        printf("mem[%d]", stored[x] - REGISTER_NUMBER);
}

void PrintMove(int x1, int x2) {                                                         // x1 = x2.
    Print(x1);
    printf(" = ");
    Print(x2);
    putchar(';');
    printf("\n");
}

void PrintOperator(int x1, int x2, char op) {                                            // x1 ?= x2.
    Print(x1);
    printf(" %c= ", op);
    Print(x2);
    putchar(';');
    printf("\n");
}

void Convert() {
    for (int i = 0; i < REGISTER_NUMBER; i++)
        reg[i] = i + 1;
    for (int i = 0; i < MEMORY_NUMBER; i++)
        mem[i] = MEMORY_NUMBER - i;

    stored[VARIABLE_NUMBER] = REGISTER_NUMBER - 4 + 1;
    stored[VARIABLE_NUMBER + 1] = REGISTER_NUMBER - 4 + 2;
    stored[VARIABLE_NUMBER + 2] = REGISTER_NUMBER - 4 + 3;

    for (int i = 0; i < EXPRESSION_NUMBER; i++) {
        variable x[3];
        x[0] = expr[i].x[0], x[1] = expr[i].x[1], x[2] = expr[i].x[2];
        if (x[1].type == 2 && x[2].type == 2) {                                                         // var = const op const.
            AllocateSpace(x[0].x);

            printf("    ");
            Print(x[0].x);
            printf(" = %d;\n", x[1].x);
            printf("    ");
            Print(x[0].x);
            printf(" %c= %d;\n", expr[i].op, x[2].x);

            FreeUpSpace(x[0].x);
            continue;
        }

        if (x[1].type == 1 && x[2].type == 1) {                                                         // var = param op param.
            AllocateSpace(x[0].x);

            printf("    ");
            PrintMove(x[0].x, x[1].x);
            printf("    ");
            PrintOperator(x[0].x, x[2].x, expr[i].op);

            FreeUpSpace(x[0].x);
            continue;
        }

        if (x[1].type == 0 && x[2].type == 0) {                                                         // var = var op var.
            int pre = 0;
            AllocateSpace(x[0].x);
            if (stored[x[0].x] > REGISTER_NUMBER && (stored[x[1].x] > REGISTER_NUMBER || stored[x[2].x] > REGISTER_NUMBER)) {
                pre = stored[x[0].x];
                stored[x[0].x] = REGISTER_NUMBER;
            }

            printf("    ");
            PrintMove(x[0].x, x[1].x);
            printf("    ");
            PrintOperator(x[0].x, x[2].x, expr[i].op);

            if (pre) {
                printf("    ");
                printf("mem[%d] = reg[%d];\n", pre - REGISTER_NUMBER, REGISTER_NUMBER);
                stored[x[0].x] = pre;
            }

            FreeUpSpace(x[0].x);
            FreeUpSpace(x[1].x);
            FreeUpSpace(x[2].x);
            continue;
        }

        if ((x[1].type == 2 && x[2].type == 1) || (x[1].type == 1 && x[2].type == 2)) {                 // var = const op param.
            AllocateSpace(x[0].x);

            if (x[1].type == 2) {
                printf("    ");
                Print(x[0].x);
                printf(" = %d;\n", x[1].x);
                printf("    ");
                PrintOperator(x[0].x, x[2].x, expr[i].op);
            }
            else {
                printf("    ");
                PrintMove(x[0].x, x[1].x);
                printf("    ");
                Print(x[0].x);
                printf(" %c= %d;\n", expr[i].op, x[2].x);
            }

            FreeUpSpace(x[0].x);
            continue;
        }

        if ((x[1].type == 2 && x[2].type == 0) || (x[1].type == 0 && x[2].type == 2)) {                 // var = const op var.
            int pre = 0;
            AllocateSpace(x[0].x);
            if (stored[x[0].x] > REGISTER_NUMBER && ((!x[1].type && stored[x[1].x] > REGISTER_NUMBER) || (!x[2].type && stored[x[2].x] > REGISTER_NUMBER))) {
                pre = stored[x[0].x];
                stored[x[0].x] = REGISTER_NUMBER;
            }

            if (x[1].type == 2) {
                printf("    ");
                Print(x[0].x);
                printf(" = %d;\n", x[1].x);
                printf("    ");
                PrintOperator(x[0].x, x[2].x, expr[i].op);
            }
            else {
                printf("    ");
                PrintMove(x[0].x, x[1].x);
                printf("    ");
                Print(x[0].x);
                printf(" %c= %d;\n", expr[i].op, x[2].x);
            }

            if (pre) {
                printf("    ");
                printf("mem[%d] = reg[%d];\n", pre - REGISTER_NUMBER, REGISTER_NUMBER);
                stored[x[0].x] = pre;
            }

            FreeUpSpace(x[0].x);
            if (x[1].type == 0)
                FreeUpSpace(x[1].x);
            else
                FreeUpSpace(x[2].x);
            continue;
        }

        if ((x[1].type == 1 && x[2].type == 0) || (x[1].type == 0 && x[2].type == 1)) {                 // var = param op var.
            int pre = 0;
            AllocateSpace(x[0].x);
            if (stored[x[0].x] > REGISTER_NUMBER && (stored[x[1].x] > REGISTER_NUMBER || stored[x[2].x] > REGISTER_NUMBER)) {
                pre = stored[x[0].x];
                stored[x[0].x] = REGISTER_NUMBER;
            }

            printf("    ");
            PrintMove(x[0].x, x[1].x);
            printf("    ");
            PrintOperator(x[0].x, x[2].x, expr[i].op);

            if (pre) {
                printf("    ");
                printf("mem[%d] = reg[%d];\n", pre - REGISTER_NUMBER, REGISTER_NUMBER);
                stored[x[0].x] = pre;
            }

            FreeUpSpace(x[0].x);
            if (x[1].type == 0)
                FreeUpSpace(x[1].x);
            else
                FreeUpSpace(x[2].x);
        }
    }
}

int main() {
    freopen("1.txt", "r", stdin);
    freopen("1.c", "w", stdout);
    Read();
    printf("#include <stdio.h>\n\n");
    printf("int reg[17], mem[1025];\n\n");
    printf("int main() {");
    puts("");
    Convert();
    printf("    return 0;\n");
    printf("}");
    return 0;
}