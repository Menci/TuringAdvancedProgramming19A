#include <stdio.h>
#include <assert.h>

#define SIZE 100
#define LEN 100

char s[LEN];
int a[10], v[SIZE];

int isdigit(int x)
{
	return '0' <= x && x <= '9';
}

void carr(int index)
{
	printf("v[%d]", index);
}

void creg(int index)
{
	printf("%c", 'a' + index);
}

void cop(char c)
{
	if(c != '=')
		printf(" %c= ", c);
	else
		printf(" = ");
}

void ed()
{
	printf("\n");
}

int readi(char *ch)
{
	int x = 0, f = 1, p = 0;
	while(ch[p] < '0' || '9' < ch[p]){if(ch[p] == '-')f = -1;p++;}
	while('0' <= ch[p] && ch[p] <= '9')x = x * 10 + (ch[p++] ^ 48);
	return x * f;
}

void work()
{
	int len;
	char ch, op;
	
	while((ch = getchar()) != EOF)
	{
		while(len)s[len--] = ' ';
		len = 0;
		while(ch != '\n')
		{
			s[++len] = ch;
			ch = getchar();
		}

		int ptr = 1, tar_id = -1;
		int id1, arg1;
		int id2, arg2;

		while(!isdigit(s[ptr]))ptr++;
		tar_id = readi(s + ptr);

		//printf("tar_id %d\n", tar_id);

		while(isdigit(s[ptr]))ptr++;
		while(s[ptr] != '=')ptr++;
		while(s[++ptr] == ' ');

		if(s[ptr] == 'a')
		{
			id1 = -readi(s + ptr + 1);
			arg1 = a[-id1];
		}
		else
		{
			id1 = readi(s + ptr + 1);
			arg1 = v[id1];
		}

		while(isdigit(s[++ptr]));
		while(s[ptr] == ' ')ptr++;
		
		//printf("%s %d\n", s + 1, ptr);

		op = s[ptr];
		
		while(s[++ptr] == ' ');
		if(isdigit(s[ptr]) || s[ptr] == '-')
		{
			id2 = 0;
			arg2 = readi(s + ptr);
		}
		else if(s[ptr] == 'a')
		{
			id2 = -readi(s + ptr + 1);
			arg2 = a[-id2];
		}
		else
		{
			id2 = readi(s + ptr + 1);
			arg2 = v[id2];
		}

		creg(1);cop('=');
		if(id1 < 0)
			printf("a%d;\n", -id1);
		else
			printf("v[%d];\n", id1);
		
		creg(1);cop(op);
		if(!id2)
			printf("%d;\n", arg2);
		else if(id2 < 0)
			printf("a%d;\n", -id2);
		else
			printf("v[%d];\n", id2);

		carr(tar_id);cop('=');
		creg(1);printf(";\n");
		
		//printf("printf(\"v[%%d] = %%d\\n\", %d, v[%d]);\n", tar_id, tar_id);
	}
}

int main()
{
	if(fopen("in.txt", "r"))
	{
		freopen("in.txt", "r", stdin);
		freopen("out.txt", "w", stdout);
	}
	
	work();
	return 0;
}
