#include<stdio.h>
#include<string.h>
#include<stdlib.h>

const int N = 16, M = 3, S = 8;

long long a[500], ta[500], b[500], tb[500], c[500], tc[500], op[500];

void getv(char *s,long long *res,long long *typ)
{
	switch(s[0])
	{
		case 'a':
			sscanf(&s[1], "%lld", res);
			*typ = 0;
			return ;
		case 'v':
			sscanf(&s[1], "%lld", res);
			*typ = 1;
			return ;
		default:
			sscanf(s, "%lld", res);
			*typ = 2;
			return ;
	}
}

void trans(int x)
{
	switch(tb[x])
	{
		case 0:
			printf("movq %%r%lld, %%rcx\n", 8 + b[x]);
			break;
		case 1:
			printf("movq %lld(%%rsp), %%rcx\n", 8 * (a[x] - b[x] - 1));
			break;
		case 2:
			printf("movq $%lld, %%rcx\n", b[x]);
			break;
	}
	switch(tc[x])
	{
		case 0:
			printf("movq %%r%lld, %%r8\n",8 + c[x]);
			break;
		case 1:
			printf("movq %lld(%%rsp), %%r8\n", 8 * (a[x] - c[x] - 1));
			break;
		case 2:
			printf("movq $%lld, %%r8\n", c[x]);
			break;
	}
	if(op[x] != '/')
	{
		switch(op[x])
		{
			case '+':
				printf("addq %%r8, %%rcx\n");
				break;
			case '-':
				printf("subq %%r8, %%rcx\n");
				break;
			case '*':
				printf("imulq %%r8, %%rcx\n");
				break;
		}
	}
	else
	{
		printf("movq %%rax, %%rsi\n");
		printf("movq %%rcx, %%rax\n");
		printf("cqto\n");
		printf("idivq %%r8\n");
		printf("movq %%rax, %%rcx\n");
		printf("movq %%rsi, %%rax\n");
	}
	printf("xorq %%rcx, %%rax\n");
	printf("pushq %%rcx\n");
}

int main()
{	
	printf("movq $0, %%rax\n");
	printf("movq %%rdi, %%r9\n");
	printf("movq %%rsi, %%r10\n");
	printf("movq %%rdx, %%r11\n");
	
	int n, i;
	for(n = 0; ; n ++)
	{
		char s[50];
		s[0] = getchar();
		if(s[0] == '\n' || s[0] == EOF) break;
		scanf("%s", &s[1]);
		getv(s, &a[n], &ta[n]);
		scanf("%s", s);
		scanf("%s", s);
		getv(s, &b[n], &tb[n]);
		scanf("%s", s);
		op[n] = s[0];
		scanf("%s\n", s);
		getv(s, &c[n], &tc[n]);
	}
	
	for(i = 0; i < n; i ++)
		trans(i);
	for(i = 0; i < n; i ++)
		printf("popq %%rsi\n");
	
	printf("ret\n");
	return 0;
}
