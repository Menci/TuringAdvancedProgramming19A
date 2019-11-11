#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BIAS (60)
#define MAX_N (60)
#define MAX_M (60)
#define MAX_LEN (10)

typedef struct expression
{
	char op;
	long long var1, var2, var3;
	int type;//-> var3;
}expression;

expression P[MAX_M + 10];

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

#define MAX_REG (12)

static int stack_[MAX_N + 10][2];
static int top[2] = {0, 0};

static int type[MAX_N + 10];
static int memid[MAX_N + 10];

static int maxReg = 0, maxArr = 0;
static char printStr[MAX_REG + 10][10];

void push(int id, int type)
{
	stack_[++top[type]][type] = id;
}

int pop(int tp)
{
	if (top[0] && MAX_REG - top[0] + 1 > maxReg) maxReg = MAX_REG - top[0] + 1;
	if (top[1] && MAX_N - top[1] + 1 > maxArr) maxArr = MAX_N - top[1] + 1;
	return stack_[top[tp]--][tp];
}

void allocate(int varID)
{
	if (!memid[varID]) {
		int flag = top[0] == 0;
		memid[varID] = pop(flag);
		type[varID] = flag;
	}
}

void print(int varID)
{
	if (!type[varID]) {
		printf("%s", printStr[memid[varID]]);
	}
	else {
		printf("%d(%%rsp)", memid[varID] * 8);
	}
}

void realPrint(int varID, int times)
{
	if (memid[varID]) {
		if (times) {
			printf("xorq ");
			print(varID);
			printf(", %%rbx\n");
		}
		push(memid[varID], type[varID]);
		memid[varID] = 0;
	}
}

void init()
{
	top[0] = top[1] = 0;
	memset(type, -1, sizeof type);
	memset(memid, 0, sizeof memid);
	for (int i = 4; i <= MAX_REG; ++i) {
		stack_[++top[0]][0] = i;
	}
	for (int i = MAX_N; i >= 1; --i) {
		stack_[++top[1]][1] = i;
	}
	for (int i = BIAS + 1; i <= BIAS + 3; ++i) {
		type[i] = 0;
		memid[i] = i - BIAS;
	}
}

void pushq(char *str)
{
	printf("pushq %s\n", str);
}

void popq(char *str)
{
	printf("popq %s\n", str);
}

void print_calc1(char *source, long long varID, int ty)
{
	printf("movq ");
	if (ty) {
		printf("$%lld", varID);
	}
	else {
		print(varID);
	}
	printf(", %s\n", source);
}

void print_calc2(char *op, char *source, long long varID, int ty)
{
	printf("%s ", op);
	if (ty) {
		printf("$%lld", varID);
	}
	else {
		print(varID);
	}
	printf(", %s\n", source);

}

void normal_print(char *op, int varID1, int varID2, long long varID3, int ty) 
{
	print_calc1("%rdx", varID2, 0);
	print_calc2(op, "%rdx", varID3, ty);
	printf("movq %%rdx, "); print(varID1); putchar('\n');
}

void print_init()
{
	for (int i = 1; i <= MAX_REG; ++i) {
		printStr[i][0] = '%';
		printStr[i][1] = 'r'; 
	}
	printStr[1][2] = 'd'; printStr[1][3] = 'i';
	printStr[2][2] = 's'; printStr[2][3] = 'i';
	printStr[3][2] = 'c'; printStr[3][3] = 'x';
	printStr[4][2] = 'b'; printStr[4][3] = 'p';
	for (int i = 5; i <= MAX_REG; ++i) {
		if (i + 3 >= 10) {
			printStr[i][2] = '1';
			printStr[i][3] = '0' + (i + 3 - 10);
		}
		else {
			printStr[i][2] = i + 3 + '0';
		}
	}
}

int main()
{
//	freopen("input.in", "r", stdin);
//	freopen("output.out", "w", stdout);

	init();
	print_init();

	char op[MAX_LEN + 10], numStr[MAX_LEN + 10];
	char var1[MAX_LEN + 10], var2[MAX_LEN + 10];

	int lst[MAX_N + 10];
	memset(lst, 0, sizeof lst);

	int m = 0;
	while (~scanf("%s %*s %s %s %s", var1, var2, op, numStr)) {
		m ++;
		lst[P[m].var1 = str_to_num(var1 + 1)] = m;
		lst[P[m].var2 = str_to_num(var2 + 1) + BIAS * (var2[0] == 'a')] = m;
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
			lst[P[m].var3 = str_to_num(numStr + 1) + BIAS] = m;
		}
		P[m].op = op[0];
	}

	for (int o = 1; o <= m; ++o) {
		allocate(P[o].var1);
		if (P[o].var2 <= BIAS) allocate(P[o].var2);
		if (P[o].var3 <= BIAS && !P[o].type) allocate(P[o].var3);

		if (!P[o].type) {
			if (lst[P[o].var3] == o) {
				realPrint(P[o].var3, 0);
			}
		}

		if (lst[P[o].var1] == o) {
			realPrint(P[o].var1, 0);
		}
		if (lst[P[o].var2] == o) {
			realPrint(P[o].var2, 0);
		}

	}

	pushq("%rbp");
	pushq("%rbx");
	pushq("%r12");
	pushq("%r13");
	pushq("%r14");
	pushq("%r15");
	if (maxArr != 0) printf("subq $%d, %%rsp\n", (maxArr + 1) * 8);
	printf("movq %%rdx, %%rcx\n");
	printf("movq $0, %%rbx\n");
	printf("xorq %%rdi, %%rbx\n");
	printf("xorq %%rsi, %%rbx\n");
	printf("xorq %%rcx, %%rbx\n");

	init();

	for (int o = 1; o <= m; ++o) {
		allocate(P[o].var1);
		allocate(P[o].var2);
		if (!P[o].type) allocate(P[o].var3);

		if (P[o].op == '+') {
			normal_print("addq", P[o].var1, P[o].var2, P[o].var3, P[o].type);
		}
		if (P[o].op == '-') {
			normal_print("subq", P[o].var1, P[o].var2, P[o].var3, P[o].type);
		}
		if (P[o].op == '*') {
			normal_print("imulq", P[o].var1, P[o].var2, P[o].var3, P[o].type);
		}
		if (P[o].op == '/') {
			print_calc1("%rdx", 0, 1);
			print_calc1("%rax", P[o].var2, 0);
			if (P[o].type) {
				printf("movq $%lld, %d(%%rsp)\n", P[o].var3, (maxArr + 1) * 8);
				printf("cqto\n");
				printf("idivq %d(%%rsp)\n", (maxArr + 1) * 8);
			}
			else {
				printf("cqto\n");
				printf("idivq ");
				print(P[o].var3);
				putchar('\n');
			}
			printf("movq %%rax, "); print(P[o].var1); putchar('\n');
		}

		if (lst[P[o].var1] == o) {
			realPrint(P[o].var1, 1);
		}
		if (lst[P[o].var2] == o) {
			realPrint(P[o].var2, 1);
		}
		if (!P[o].type && lst[P[o].var3] == o) {
			realPrint(P[o].var3, 1);
		}
	}

	printf("movq %%rbx, %%rax\n");
	if (maxArr != 0) printf("addq $%d, %%rsp\n", (maxArr + 1) * 8);
	popq("%r15");
	popq("%r14");
	popq("%r13");
	popq("%r12");
	popq("%rbx");
	popq("%rbp");
	printf("ret\n");

	return 0;
}

