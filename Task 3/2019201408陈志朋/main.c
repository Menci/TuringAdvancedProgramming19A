#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#define pos_num 0
#define N 100
#define LEN 20

#define START_OF_REGISTER1 (1)
#define   END_OF_REGISTER1 (16)

#define START_OF_REGISTER2 (16 + 1)
#define   END_OF_REGISTER2 (16 + 3)

#define START_OF_NUMBER    (16 + 3 + 1)
#define   END_OF_NUMBER    (16 + 3 + 2)

#define START_OF_MEMORY    (16 + 3 + 2 + 1)

char OP[200][10][20];

int pos[N], val[N];
int rg1 = START_OF_REGISTER1,
    rg2 = START_OF_REGISTER2,
	mr  = START_OF_MEMORY;
int pnum;

bool EXIT;

void init(int a1, int a2,int a3) {
	EXIT = 0;
	memset(pos, -1, sizeof(pos));
	memset(val,  0, sizeof(val));
	val[START_OF_REGISTER2 + 0] = a1;
	val[START_OF_REGISTER2 + 1] = a2;
	val[START_OF_REGISTER2 + 2] = a3;
}
bool judge_num(char ch) {
	return ('0' <= ch && ch <= '9');
}
bool judge_reg1(int p) {
	return (START_OF_REGISTER1 <= p && p <= END_OF_REGISTER1);
}
bool judge_reg2(int p) {
	return (START_OF_REGISTER2 <= p && p <= END_OF_REGISTER2);
}
bool judge_mem(int p) {
	return (START_OF_MEMORY <= p);
}
bool judge(char c) {
	return (c == 'a' || c == 'v' || judge_num(c) || c == '-');
}
void print_reg1 (int p) {
	printf ("r%d", p - START_OF_REGISTER1 + 1);
}
void print_reg2 (int p) {
	printf ("a%d", p - START_OF_REGISTER2 + 1);
}
void print_mem (int p) {
	printf ("m[%d]", p - START_OF_MEMORY + 1);
}
void print_mov (int p1, int p2) {
	val[p1] = val[p2];
	
	printf ("    ");
	
	if (judge_reg1(p1)) print_reg1(p1);
	else if (judge_reg2(p1)) print_reg2(p1);
	else if (judge_mem (p1)) print_mem (p1);
	
	printf(" = ");
	
	if (judge_reg1(p2)) print_reg1(p2);
	else if (judge_reg2(p2)) print_reg2(p2);
	else if (judge_mem (p2)) print_mem (p2);
	
	printf (";\n");
	
}
void print_op(int p1, int p2, char op) {
	if (op == '+') val[p1] += val[p2];
	else if (op == '-') val[p1] -= val[p2];
	else if (op == '*') val[p1] *= val[p2];
	else if (op == '/') val[p1] /= val[p2];
	
	printf ("    ");
	
	if (judge_reg1(p1)) printf("r%d ", p1);
	else printf("m[%d] ", p1 - START_OF_MEMORY + 1);
	printf("%c= ", op);
	if (judge_reg1(p2)) printf("r%d;\n", p2 - START_OF_REGISTER1 + 1);
	else if (judge_reg2(p2)) printf("a%d;\n", p2 - START_OF_REGISTER2 + 1);
	else if (judge_mem(p2)) printf("m[%d];\n", p2 - START_OF_MEMORY + 1);
	else  printf("%d;\n", val[p2]);
	
}
void get_number (int *p, int step, int idx) {
	int len = 0, id = 0, i;
	char ch[LEN], c;
	for (c = getchar(); !judge(c); c = getchar())
		if (c == EOF) {
			EXIT = 1; return;
		}
	for (; judge(c); c = getchar()){
			OP[step][idx][len] = ch[len] = c;
			++len;
	}
	if (ch[0] == 'a') {
		id = ch[1] - '0';
		*p = START_OF_REGISTER2 + id - 1;
	}
	else if (ch[0] == 'v') {
		for (i = 1; judge_num(ch[i]) && i < len; ++i)
			id = id * 10 + ch[i] - '0';
		if (pos[id] != -1) *p = pos[id];
		else if (pos[id] == -1) {
			if (rg1 <= END_OF_REGISTER1) {
				*p = pos[id] = rg1++;
			}
			else {
				*p = pos[id] = mr++;
			}
		}
	}
	else if (judge_num(ch[0]) || ch[0] == '-') {
		if (ch[0] == '-') i = 1;
		else i = 0;
		for (; judge_num(ch[i]) && i < len; ++i)
			id = id * 10 + ch[i] - '0';
		if (ch[0] == '-') id = -id;
		*p = pnum; val[pnum] = id; ++pnum; 
	}
}
void get_op(int *p1, int *p2, int *p3, int step) {
	char c;
	pnum = START_OF_NUMBER;
	get_number(p1, step, 1); if (EXIT) return;
	scanf ("%c", &OP[step][2][0]); //get '='
	get_number(p2, step, 3); if (EXIT) return;
	scanf ("%c", &OP[step][4][0]); //get operation
	get_number(p3, step, 5); if (EXIT) return;
}
int step = 0;
void Change(int a1, int a2,int a3) {
	char op;
	int p1 = 0, p2 = 0, p3 = 0;
	init(a1, a2, a3);
	while (1){
		step++; p1 = p2 = p3 = 0;
		get_op(&p1, &p2, &p3, step);
		if (EXIT) return;
		op = OP[step][4][0];
		if (p1 != p2 && p1 != p3) {
			print_mov(p1, p2);
			print_op(p1, p3, op);
		}
		else {
			if (p1 == p2) p2 = p3;
			print_op(p1, p2, op);
		}
	}
}

int main ()
{
	freopen ("in.in", "r", stdin);
	freopen ("std.c", "w", stdout);
	srand(time(0));
	int a1 = rand(), a2 = rand(), a3 = rand(), i;
	
	printf ("#include <stdio.h>\n");
	printf ("int i, a1 = %d, a2 = %d, a3 = %d,", a1, a2, a3);
	for (i = 1; i <= 16; ++i)printf(" r%d = 0,", i);
	for (i = 1; i <= 60; ++i)printf(" v%d = 0,", i);
	printf (" m[1024]={0};\n");
	printf ("int main() {\n");
	printf ("    freopen(\"std.out\", \"w\", stdout);\n");
	
	Change(a1, a2, a3);

	for (i = 1; i <= 60; ++i)
		if (pos[i] != -1) {
			printf ("    v%d = ", i);
			if (judge_reg1(pos[i])) print_reg1(pos[i]);
			else print_mem(pos[i]);
			printf (";\n");
			printf ("    printf (\"v%d = %%d;\\n\", v%d);\n", i, i);
		}
	
	printf ("return 0;\n}\n");
	
	freopen ("me.out", "w", stdout);
	for (i = 1; i <= 60; ++i)
		if (pos[i] != -1)
			printf ("v%d = %d;\n", i, val[pos[i]]);
	
	freopen ("std_check.c", "w", stdout);
	printf ("#include <stdio.h>\n");
	printf ("int i, a1 = %d, a2 = %d, a3 = %d,", a1, a2, a3);
	for (i = 1; i <= 16; ++i)printf(" r%d = 0,", i);
	for (i = 1; i <= 60; ++i)printf(" v%d = 0,", i);
	printf (" m[1024] = {0};\n");
	printf ("int main() {\n");
	printf ("    freopen(\"std1.out\", \"w\", stdout);\n");
	for (i = 1; i < step; ++i)
		printf ("    %s = %s %c %s;\n", OP[i][1], OP[i][3], OP[i][4][0], OP[i][5]);
	for (i = 1; i <= 60; ++i)
		printf ("    printf (\"v%d = %%d;\\n\", v%d);\n", i, i);
	printf ("return 0;\n}\n");
	return 0;
}
