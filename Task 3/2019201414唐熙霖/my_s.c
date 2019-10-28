#include <math.h>
#include <stdio.h>
#include <ctype.h>
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

int main()
{
	int i, j, k;
	char op;
	while((ch = getchar()) != EOF)
	{
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
			printf("b = a%d;\n", j);
		else 
			printf("b = v[%d];\n", j);
		
		switch(stat)
		{
			case NUM:
				printf("b %c= %d;\n", op, k);
				break;
			case PARAMETER:
				printf("b %c= a%d;\n", op, k);
				break;
			case LOCAL_VAR:
				printf("b %c= v[%d];\n", op, k);
				break;
		}
		printf("v[%d] = b;\n", i);
		while(ch != '\n')
			ch = getchar();
	}
	return 0;
}
