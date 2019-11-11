#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define pos_num 0
#define N 100
#define LEN 20

#define START_OF_CALLER_SAVED (1)
#define   END_OF_CALLER_SAVED (2)

#define START_OF_CALLEE_SAVED (2 + 1)
#define   END_OF_CALLEE_SAVED (2 + 4)

#define FREE_REGISTER (2 + 3)

#define START_OF_RETURN_VAL (2 + 4 + 1)
#define   END_OF_RETURN_VAL (2 + 4 + 1)

#define START_OF_ARGUEMENT (2 + 4 + 1 + 1)
#define   END_OF_ARGUEMENT (2 + 4 + 1 + 3)

#define START_OF_REGISTER (1)
#define   END_OF_REGISTER (2 + 4 + 1 + 3)

#define END_OF_REGISTER_AVAILABLE (2 + 2)

#define START_OF_NUMBER    (16 + 3 + 1)
#define   END_OF_NUMBER    (16 + 3 + 2)

#define START_OF_MEMORY    (16 + 3 + 2 + 1)

char OP[N][10][LEN];
char name_of_reg[N][LEN];

int pos[N], val[N];
int rg1 = START_OF_REGISTER,
	mr  = START_OF_NUMBER;
int pnum;

bool EXIT;

bool judge_num(char ch) {
	return ('0' <= ch && ch <= '9');
}
bool judge_reg(int p) {
	return (START_OF_REGISTER <= p && p <= END_OF_REGISTER);
}
bool judge_mem(int p) {
	return (START_OF_MEMORY <= p);
}
bool judge(char c) {
	return (c == 'a' || c == 'v' || judge_num(c) || c == '-');
}
void print_reg (int p) {
	printf ("%s", name_of_reg[p]);
}
void print_mem (int p) {
	printf ("%d(%%rsp)", (mr - p) * 8);
}
void print_num (int p) {
	printf ("$%d", val[p]);
}
void print_push (int p) {
	printf ("pushq %s\n", name_of_reg[p]);
	fflush(stdout);
}
void print_pop (int p) {
	printf ("popq %s\n", name_of_reg[p]);
	fflush(stdout);
}
void print_new () {
	printf ("subq $8 , %%rsp\n");
	fflush(stdout);
}
void print_del (int val) {
	printf ("addq $%d , %%rsp\n", val * 8);
	fflush(stdout);
}
void print_clear () {
	printf ("movq $0 , %%rdx\n");
	fflush(stdout);
}
void print_mov (int p1, int p2) {
	printf("movq ");
	
	if (judge_reg(p2)) print_reg(p2);
	else print_mem (p2);
	
	printf(" , ");

	if (judge_reg(p1)) print_reg(p1);
	else print_mem (p1);
	
	printf("\n");
	fflush(stdout);
}
void print_mov_num (int p) {
	printf("movq ");
	print_num(p);
	printf(" , ");
	print_reg (FREE_REGISTER);
	printf("\n");
	fflush(stdout);
	
	print_mov(p, FREE_REGISTER);
}
void print_xor (int p1, int p2) {
	printf("xorq ");
	
	if (judge_reg(p2)) print_reg(p2);
	else print_mem (p2);
	
	printf(" , ");

	if (judge_reg(p1)) print_reg(p1);
	else print_mem (p1);
	
	printf("\n");
	fflush(stdout);
}
void print_op(int p1, int p2, char op) {
	if (op == '/') {
		printf("movq %%rdx , %%r15\n");
		print_mov(START_OF_RETURN_VAL, p1);
		// print_clear();
		printf("cqto\n");
		printf("idivq ");
		if (judge_reg(p2)) print_reg(p2);
		else print_mem(p2);
		printf("\n"); fflush(stdout);
		printf("movq %%r15 , %%rdx\n"); fflush(stdout);
		print_mov(p1, START_OF_RETURN_VAL);
	}
	else {
		if (op == '+') printf("addq ");
		else if (op == '*') printf("imulq ");
		else if (op == '-') printf("subq ");

		if (judge_reg(p2)) print_reg(p2);
		else print_mem (p2);
		
		printf(" , ");

		if (judge_reg(p1)) print_reg(p1);
		else print_mem (p1);
		printf("\n");
		fflush(stdout);
	}
}
void init() {
	EXIT = 0;
	memset(pos, -1, sizeof(pos));
	memset(val,  0, sizeof(val));
	
	name_of_reg[START_OF_CALLER_SAVED + 0][0] = '%';
	name_of_reg[START_OF_CALLER_SAVED + 0][1] = 'r';
	name_of_reg[START_OF_CALLER_SAVED + 0][2] = '1';
	name_of_reg[START_OF_CALLER_SAVED + 0][3] = '0';   //caller saved - 1
	
	name_of_reg[START_OF_CALLER_SAVED + 1][0] = '%';
	name_of_reg[START_OF_CALLER_SAVED + 1][1] = 'r';
	name_of_reg[START_OF_CALLER_SAVED + 1][2] = '1';
	name_of_reg[START_OF_CALLER_SAVED + 1][3] = '1';   //caller saved - 2
	
/*------------------------------------------------------------------------------------------------------*/

	name_of_reg[START_OF_CALLEE_SAVED + 0][0] = '%';
	name_of_reg[START_OF_CALLEE_SAVED + 0][1] = 'r';
	name_of_reg[START_OF_CALLEE_SAVED + 0][2] = 'b';
	name_of_reg[START_OF_CALLEE_SAVED + 0][3] = 'x';   //callee saved - 1
	
	name_of_reg[START_OF_CALLEE_SAVED + 1][0] = '%';
	name_of_reg[START_OF_CALLEE_SAVED + 1][1] = 'r';
	name_of_reg[START_OF_CALLEE_SAVED + 1][2] = '1';
	name_of_reg[START_OF_CALLEE_SAVED + 1][3] = '2';   //callee saved - 2
	
	name_of_reg[START_OF_CALLEE_SAVED + 2][0] = '%';
	name_of_reg[START_OF_CALLEE_SAVED + 2][1] = 'r';
	name_of_reg[START_OF_CALLEE_SAVED + 2][2] = '1';
	name_of_reg[START_OF_CALLEE_SAVED + 2][3] = '4';   //callee saved - 4
	
	name_of_reg[START_OF_CALLEE_SAVED + 3][0] = '%';
	name_of_reg[START_OF_CALLEE_SAVED + 3][1] = 'r';
	name_of_reg[START_OF_CALLEE_SAVED + 3][2] = '1';
	name_of_reg[START_OF_CALLEE_SAVED + 3][3] = '5';   //callee saved - 5

/*------------------------------------------------------------------------------------------------------*/

	name_of_reg[START_OF_RETURN_VAL + 0][0] = '%';
	name_of_reg[START_OF_RETURN_VAL + 0][1] = 'r';
	name_of_reg[START_OF_RETURN_VAL + 0][2] = 'a';
	name_of_reg[START_OF_RETURN_VAL + 0][3] = 'x';   //return val

/*------------------------------------------------------------------------------------------------------*/

	name_of_reg[START_OF_ARGUEMENT + 0][0] = '%';
	name_of_reg[START_OF_ARGUEMENT + 0][1] = 'r';
	name_of_reg[START_OF_ARGUEMENT + 0][2] = 'd';
	name_of_reg[START_OF_ARGUEMENT + 0][3] = 'i';   //arguement - 1
	
	name_of_reg[START_OF_ARGUEMENT + 1][0] = '%';
	name_of_reg[START_OF_ARGUEMENT + 1][1] = 'r';
	name_of_reg[START_OF_ARGUEMENT + 1][2] = 's';
	name_of_reg[START_OF_ARGUEMENT + 1][3] = 'i';   //arguement - 2
	
	name_of_reg[START_OF_ARGUEMENT + 2][0] = '%';
	name_of_reg[START_OF_ARGUEMENT + 2][1] = 'r';
	name_of_reg[START_OF_ARGUEMENT + 2][2] = '1';
	name_of_reg[START_OF_ARGUEMENT + 2][3] = '3';   //arguement - 3
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
		*p = START_OF_ARGUEMENT + id - 1;
	}
	else if (ch[0] == 'v') {
		for (i = 1; judge_num(ch[i]) && i < len; ++i)
			id = id * 10 + ch[i] - '0';
		if (pos[id] != -1) *p = pos[id];
		else if (pos[id] == -1) {
			if (rg1 <= END_OF_REGISTER_AVAILABLE) {
				*p = pos[id] = rg1++;
			}
			else {
				*p = pos[id] = mr++;
				print_new();
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
		print_mov_num(*p);
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
void Change() {
	char op;
	int p1 = 0, p2 = 0, p3 = 0, i;
	init();
	for (i = START_OF_CALLEE_SAVED; i <= END_OF_CALLEE_SAVED; ++i)
		print_push(i);
	printf("movq %%rdx , %%r13\n");fflush(stdout);
	print_new(); mr++; print_new(); mr++;
	while (1){
		// step++; 
		p1 = p2 = p3 = 0;
		get_op(&p1, &p2, &p3, step);
		if (EXIT) break;
		op = OP[step][4][0];
		print_mov(FREE_REGISTER, p2);
		print_op(FREE_REGISTER, p3, op);
		print_mov(p1, FREE_REGISTER);
	}
	printf("movq $0 , %%rax\n"); fflush(stdout);
	for (i = 1; i <= 60; ++i) 
		if (pos[i] != -1) print_xor(START_OF_RETURN_VAL, pos[i]);
	if (mr > START_OF_MEMORY) print_del(mr - START_OF_MEMORY);
	print_del(2); mr -= 2;
	for (i = END_OF_CALLEE_SAVED; i >= START_OF_CALLEE_SAVED; --i) print_pop(i);
	printf ("ret\n");
	fflush(stdout);
}

int main ()
{
	Change();
	return 0;
}
