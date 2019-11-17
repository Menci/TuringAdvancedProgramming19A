#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUM_OF_VARS 1000

enum TYPE
{
	PARAMETER, LOCAL_VAR, NUM
}stat;

char ch;
int v[NUM_OF_VARS];

char read_op();
int read();
int calc_stk_pos(int, int);

inline int read()
{
	ch = getchar();
	int x = 0, f = 1;
	while (!isdigit(ch))
	{
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch))
		x = x * 10 + ch -'0',
		ch = getchar();
	return x * f;
}

inline char read_op()
{
	ch = getchar();
	while(ch != '+' && ch !='-' && ch != '*' && ch != '/')
		ch = getchar();
	return ch;
}

inline int calc_stk_pos(int i, int len)
{
	return (len - 1 - i) * 8;
}

int main()
{
	int i, j, k;
	char op;
	puts("movq %rdx, %rcx");
	// %rcx for the third parameter
	puts("movq $0, %r8");
	// %r8 for answer
	// %r9 for tmp

	int len = 0;
	char p1, p2;

	while((ch = getchar()) != EOF)
	{
		if(ch == '\n')
			break;
		
		++len;

		i = read();

		while (!isalpha(ch))
			ch = getchar();
		bool is_parameter = (ch == 'a');

		j = read();
		
		op = read_op();

		ch = getchar();
		while(ch != '-' && !isalpha(ch) && !isdigit(ch))
			ch = getchar();
		if(ch == 'a')
			stat = PARAMETER;
		else if(isalpha(ch))
			stat = LOCAL_VAR;
		else
			stat = NUM, ungetc(ch, stdin);
		
		k = read();
		if(is_parameter)
		{
			switch(j)
			{
				case 1:
					puts("movq %rdi, %rax");
					break;
				case 2:
					puts("movq %rsi, %rax");
					break;
				case 3:
					puts("movq %rcx, %rax");
					break;
			}
		}
		else 
			printf("movq %d(%%rsp), %%rax\n", calc_stk_pos(j,len));
		
		switch(stat)
		{
			case NUM:
				printf("movq $%d, %%r9\n", k);
				switch(op)
				{
					case '+':
						puts("addq %r9, %rax");
						break;
					case '-':
						puts("subq %r9, %rax");
						break;
					case '*':
						puts("imulq %r9, %rax");
						break;
					case '/':
						puts("cqto");
						puts("idivq %r9");
						break;
				}
				break;

			case PARAMETER:
				switch(k)
				{
					case 1:
						p1 = 'd', p2 = 'i';
						break;
					case 2:
						p1 = 's', p2 = 'i';
						break;
					case 3:
						p1 = 'c', p2 = 'x';
						break;
				}
				switch(op)
				{
					case '+':
						printf("addq %%r%c%c, %%rax\n", p1, p2);
						break;
					case '-':
						printf("subq %%r%c%c, %%rax\n", p1, p2);
						break;
					case '*':
						printf("imulq %%r%c%C, %%rax\n", p1, p2);
						break;
					case '/':
						puts("cqto");
						printf("idivq %%r%c%c\n", p1, p2);
						break;
				}
				break;
				
			case LOCAL_VAR:
				switch(op)
				{
					case '+':
						printf("addq %d(%%rsp), %%rax\n", calc_stk_pos(k, len));
						break;
					case '-':
						printf("subq %d(%%rsp), %%rax\n", calc_stk_pos(k, len));
						break;
					case '*':
						printf("imulq %d(%%rsp), %%rax\n", calc_stk_pos(k, len));
						break;
					case '/':
						puts("cqto");
						printf("idivq %d(%%rsp)\n", calc_stk_pos(k, len));
						break;
				}
				break;
		}
		puts("pushq %rax");
		printf("xorq %%rax, %%r8\n");
		while(ch != '\n')
			ch = getchar();
		puts("");
	}
	while(len--)
		puts("popq %r9");
	puts("movq %r8, %rax");
	puts("ret");
	return 0;
}
