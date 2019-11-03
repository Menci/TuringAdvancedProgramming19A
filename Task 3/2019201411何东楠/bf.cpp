
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_N (60)
#define MAX_M (60)
#define MAX_LEN (5)

typedef struct expression
{
	char op;
	int var1, var2;
	long long var3;
	int type;//-> var3;
}expression;

expression P[MAX_M + 5];

int is_digit(char c)
{
	return '0' <= c && c <= '9';
}

long long str_to_num(char *numStr)
{
	long long ret = 0, flag = 1;
	if ((*numStr) == '-') {
		flag = -1;
		numStr ++;
	}
	for (; is_digit(*numStr); ++numStr) {
		ret = ret * 10 + (*numStr) - '0';
	}
	return ret * flag;
}

#define MAX_REG (16)

static int stack_[MAX_N + 5][2];
static int top[2] = {0, 0};

static int type[MAX_N + 5];
static int memid[MAX_N + 5];

static int maxReg = 0, maxArr = 0;
static int out[MAX_N + 5];
static long long x[MAX_N + 5];

void push(int id, int type)
{
	stack_[++top[type]][type] = id;
	if (MAX_REG - top[0] > maxReg) maxReg = MAX_REG - top[0];
	if (MAX_N - top[1] > maxArr) maxArr = MAX_N - top[1];
}

int pop(int type)
{
	return stack_[top[type]--][type];
}

int allocate(int varID)
{
	if (!memid[varID]) {
		memid[varID] = pop(top[0] == 0);
		type[varID] = (top[0] == 0);
	}
	return memid[varID];
}

void print(int varID)
{
	if (!type[varID] || varID > 60) {
		if (varID <= 60) printf("x%d", memid[varID]);
		else printf("a%d", varID - 60);
	}
	else {
		printf("A[%d]", memid[varID]);
	}
}

void printSwap(int var)
{
	printf("A[128]"); putchar('='); print(var); printf(";\n");
	print(var); putchar('='); printf("x1;\n");
	printf("x1 = A[128];\n");
}

void realPrint(int varID)
{
	if (memid[varID]) {
		printf("printf(\"%%d\\n\",");
		print(varID);
		printf(");\n");
		push(memid[varID], type[varID]);
	}
}

int main()
{
	freopen("input.in", "r", stdin);
	freopen("std_out.out", "w", stdout);

	char op[MAX_LEN + 5];
	char var1[MAX_LEN + 5];
	char var2[MAX_LEN + 5];
	char numStr[MAX_LEN + 5];

	memset(type, -1, sizeof type);
	int lst[MAX_N + 5];
	memset(lst, 0, sizeof lst);

	for (int i = 1; i <= MAX_REG; ++i) {
		stack_[++top[0]][0] = i;
	}
	for (int i = 1; i <= MAX_N; ++i) {
		stack_[++top[1]][1] = i;
	}

	int m = 0;
	while (~scanf("%s %*s %s %s %s", var1, var2, op, numStr)) {
		m ++;
		lst[P[m].var1 = str_to_num(var1 + 1)] = m;
		lst[P[m].var2 = str_to_num(var2 + 1) + 60 * (var2[0] == 'a')] = m;
		if (is_digit(numStr[0]) || numStr[0] == '-') {
			P[m].type = 1;
			P[m].var3 = str_to_num(numStr);
		}
		else if (numStr[0] != 'a') {
			P[m].type = 0;
			lst[P[m].var3 = str_to_num(numStr + 1)] = m;
		}
		else {
			P[m].type = 0;
			lst[P[m].var3 = str_to_num(numStr + 1) + 60] = m;
		}
		P[m].op = op[0];
	}
	fclose(stdin);
	freopen("testdata.in", "r", stdin);
	scanf("%lld%lld%lld", &x[61], &x[62], &x[63]);

	for (int o = 1; o <= m; ++o) {
		if (P[o].type) {
			if (P[o].op == '+') x[P[o].var1] = x[P[o].var2] + P[o].var3;
			if (P[o].op == '-') x[P[o].var1] = x[P[o].var2] - P[o].var3;
			if (P[o].op == '*') x[P[o].var1] = x[P[o].var2] * P[o].var3;
			if (P[o].op == '/') {
				x[P[o].var1] = x[P[o].var2] / P[o].var3;
			}
		}
		else {
			if (P[o].op == '+') x[P[o].var1] = x[P[o].var2] + x[P[o].var3];
			if (P[o].op == '-') x[P[o].var1] = x[P[o].var2] - x[P[o].var3];
			if (P[o].op == '*') x[P[o].var1] = x[P[o].var2] * x[P[o].var3];
			if (P[o].op == '/') {
				if (!x[P[o].var3]) fprintf(stderr, "%d\n", o);
				x[P[o].var1] = x[P[o].var2] / x[P[o].var3];
			}
			if (lst[P[o].var3] == o && P[o].var3 <= 60) {
				if (!out[P[o].var3]) printf("%lld\n", x[P[o].var3]);
				out[P[o].var3] = 1;
			}
		}

		if (lst[P[o].var1] == o && P[o].var1 <= 60) {
			if (!out[P[o].var1]) printf("%lld\n", x[P[o].var1]);
			out[P[o].var1] = 1;
		}
		if (lst[P[o].var2] == o && P[o].var2 <= 60) {
			if (!out[P[o].var2]) printf("%lld\n", x[P[o].var2]);
			out[P[o].var2] = 1;
		}

	}

	return 0;
}

