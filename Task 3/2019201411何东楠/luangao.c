#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_N (60)
#define MAX_M (60)
#define MAX_LEN (5)

typedef struct expression
{
	char op;
	long long var1, var2, var3;
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

void push(int id, int type)
{
	stack_[++top[type]][type] = id;
}

int pop(int type)
{
	if (MAX_REG - top[0] - 1 > maxReg) maxReg = MAX_REG - top[0] - 1;
	if (MAX_N - top[1] - 1 > maxArr) maxArr = MAX_N - top[1] - 1;
	return stack_[top[type]--][type];
}

int allocate(int varID)
{
	if (!memid[varID]) {
		int flag = top[0] == 0;
		memid[varID] = pop(flag);
		type[varID] = flag;
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
		printf("printf(\"%%lld\\n\",");
		print(varID);
		printf(");\n");
		push(memid[varID], type[varID]);
	}
}

int main()
{
	freopen("input.in", "r", stdin);
	freopen("test.cpp", "w", stdout);

	printf("#include <stdio.h>\n");
	printf("void calc(long long a1, long long a2, long long a3)\n{\nlong long A[200] = {0};\n");
	for (int i = 1; i <= MAX_REG; ++i) {
		printf("long long x%d;\n", i);
	}

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

	for (int o = 1; o <= m; ++o) {
		int var1ID = allocate(P[o].var1);
		int var2ID = P[o].var2 > 60 ? 0 : allocate(P[o].var2);
		int var3ID = P[o].var3 > 60 || P[o].type ? 0 : allocate(P[o].var3);

		if (P[o].type) {
			int flag = type[P[o].var1] == type[P[o].var2] && type[P[o].var1] == 1, formerID = 0;
			if (flag) {
				formerID = memid[P[o].var1];
				printSwap(P[o].var1);
				memid[P[o].var1] = 1;
				type[P[o].var1] = 0;
			}
			print(P[o].var1); putchar('='); print(P[o].var2); printf(";\n");
			print(P[o].var1); putchar(P[o].op); putchar('='); printf("%lld;\n", P[o].var3);
			if (flag) {
				memid[P[o].var1] = formerID;
				type[P[o].var1] = 1;
				printSwap(P[o].var1);
			}
		}
		else {
			int flag = type[P[o].var1] == type[P[o].var2] && type[P[o].var1] == 1, formerID = 0;
			if (flag) {
				formerID = memid[P[o].var1];
				printSwap(P[o].var1);
				memid[P[o].var1] = 1;
				type[P[o].var1] = 0;
			}
			print(P[o].var1); putchar('='); print(P[o].var2); printf(";\n");
			print(P[o].var1); putchar(P[o].op); putchar('='); print(P[o].var3); printf(";\n");
			if (flag) {
				memid[P[o].var1] = formerID;
				type[P[o].var1] = 1;
				printSwap(P[o].var1);
			}
			if (lst[P[o].var3] == o && P[o].var3 <= 60) {
				realPrint(P[o].var3);
				memid[P[o].var3] = 0;
			}
		}

		if (lst[P[o].var1] == o) {
			realPrint(P[o].var1);
			memid[P[o].var1] = 0;
		}
		if (lst[P[o].var2] == o) {
			realPrint(P[o].var2);
			memid[P[o].var2] = 0;
		}

	}

	printf("}\n");
	printf("int main() {\n");
	printf("freopen(\"testdata.in\", \"r\", stdin);\n");
	printf("freopen(\"my_out.out\", \"w\", stdout);\n");
	printf("long long a, b, c;\n");
	printf("scanf(\"%%lld%%lld%%lld\", &a, &b, &c);\n");
	printf("calc(a, b, c);\n");
	printf("}\n");
	fprintf(stderr, "register : %d array : %d\n", maxReg, maxArr);
	return 0;
}

