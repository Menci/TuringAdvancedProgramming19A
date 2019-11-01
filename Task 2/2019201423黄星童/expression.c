#include "mydouble.c"

#define LEN_OF_EXPRESSION 10000005

int num, tot, slen;
char expr[LEN_OF_EXPRESSION], stack[LEN_OF_EXPRESSION], outq[LEN_OF_EXPRESSION];
myDouble numq[LEN_OF_EXPRESSION], stackRPN[LEN_OF_EXPRESSION];

int isopt(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        return 1;
    return 0;
}

int GenNum(int i) {
    int s = 0, dbit = 0;
    myDouble res, integer = POSITIVE_ZERO, decimal = POSITIVE_ZERO;

    if (i >= 2 && isopt(expr[i - 2]) && isopt(expr[i - 1])) {
        if (expr[i - 1] == '-') s = 1;
        if (expr[i - 1] == '+') s = 0;
    }

    for (; isdigit(expr[i]); i++) {
        integer = Multiplication(integer, positivePow10[1][1]);
        integer = Addition(integer, positivePow10[0][expr[i] - '0']);
    }

    if (expr[i] == '.') {
        i++;
        for (int exp = 1; isdigit(expr[i]); i++, exp++) {
            decimal = Addition(decimal, negativePow10[exp][expr[i] - '0']);
            dbit++;
        }
    }

    res = Addition(integer, decimal);
    if (s == 1) res = Negative(res);
    numq[++num] = res;
    outq[++tot] = '#';
    return i - 1;
}

int NotGreater(char now, char top) {
    if (top == '(')
        return 0;
    if ((now == '*' || now == '/') && (top == '+' || top == '-'))
        return 0;
    return 1;
}

void GetRPNExpression() {
    int stn = 0;
    for (int i = 0; i < slen; i++) {
        if (isdigit(expr[i])) {
            if (i >= 2 && isopt(expr[i - 2]) && isopt(expr[i - 1]))
                stn--;
            i = GenNum(i);
            continue;
        }
        if (expr[i] == '(') {
            stack[++stn] = '(';
            continue;
        }
        if (expr[i] == ')') {
            while (stack[stn] != '(')
                outq[++tot] = stack[stn--];
            stn--;
            continue;
        }
        if (isopt(expr[i])) {
            while (stn && NotGreater(expr[i], stack[stn]))
                outq[++tot] = stack[stn--];
            stack[++stn] = expr[i];
            continue;
        }
    }
    while (stn >= 1)
        outq[++tot] = stack[stn--];
}

void Calculate() {
    int nownum = 1, top = 0;
    myDouble a, b;
    for (int i = 1; i <= tot; i++) {
        if (outq[i] == '#') {
            stackRPN[++top] = numq[nownum++];
            continue;
        }
        a = stackRPN[top--];
        b = stackRPN[top--];
        switch (outq[i]) {
            case '+':
                a = Addition(b, a);
                break;
            case '-':
                a = Subtraction(b, a);
                break;
            case '*':
                a = Multiplication(b, a);
                break;
            case '/':
                a = Division(b, a);
                break;
        }
        stackRPN[++top] = a;
    }
    Print(stackRPN[1]);
    puts("");
}

void ReadExpression() {
    char ch = getchar();
    while (ch != EOF && ch != '\n' && ch != '\r') {
        if (ch == ' ') {
            ch = getchar();
            continue;
        }
        expr[slen++] = ch;
        ch = getchar();
    }
}

int main() {
    InitProcess();
    MakeTable();
    char ch;
    while ((ch = getchar()) != EOF) {
        ungetc(ch, stdin);
        slen = tot = num = 0;
        ReadExpression();
        GetRPNExpression();
        Calculate();
        fflush(stdout);
    }
    return 0;
}