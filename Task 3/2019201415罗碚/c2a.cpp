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
			printf("\tr1 = a%lld;\n", b[x]);
			break;
		case 1:
			printf("\tr1 = v[%lld];\n", b[x]);
			break;
		case 2:
			printf("\tr1 = %lld;\n", b[x]);
			break;
	}
	switch(tc[x])
	{
		case 0:
			printf("\tr2 = a%lld;\n", c[x]);
			break;
		case 1:
			printf("\tr2 = v[%lld];\n", c[x]);
			break;
		case 2:
			printf("\tr2 = %lld;\n", c[x]);
			break;
	}
	switch(op[x])
	{
		case '+':
			printf("\tr1 += r2;\n");
			break;
		case '-':
			printf("\tr1 -= r2;\n");
			break;
		case '*':
			printf("\tr1 *= r2;\n");
			break;
		case '/':
			printf("\tr1 /= r2;\n");
	}
	printf("\tprintf(\"%%d\\n\",r1);\n");
	printf("\tv[%lld] = r1;\n",a[x]);
}

int main()
{	
	freopen("1.txt","r",stdin);
	freopen("1.c","w",stdout);
	
	printf("#include<stdio.h>\n\n");
	printf("int f1(int a1,int a2,int a3)\n{\n\tint v[100];\n");
	
	int n, i;
	for(n = 0; ; n ++)
	{
		char s[50];
		s[0] = getchar();
		if(s[0] == '\n' || s[0] == EOF) break;
		scanf("%s", &s[1]);
		getv(s, &a[n], &ta[n]);
		printf("\tv[%lld] ",a[n]);
		scanf("%s", s);
		printf("%s ",s);
		scanf("%s", s);
		getv(s, &b[n], &tb[n]);
		if(tb[n] != 1)
			printf("%s ",s);
		else
			printf("v[%lld] ",b[n]);
		scanf("%s", s);
		printf("%s ",s);
		op[n] = s[0];
		scanf("%s\n", s);
		getv(s, &c[n], &tc[n]);
		if(tc[n] != 1)
			printf("%s\n",s);
		else
			printf("v[%lld];\n",c[n]);
		printf("\tprintf(\"%%d\\n\",v[%lld]);\n",a[n]);
	}
	
	printf("}\n\nint f2(int a1,int a2,int a3)\n{\n\tint r1,r2,v[100];\n");
	
	for(i = 0; i < n; i ++)
		trans(i);
	
	printf("}\n\nint main()\n{\n\tfreopen(\"1.ans\",\"w\",stdout);\n\tf1(1,2,3);\n\tfreopen(\"1.out\",\"w\",stdout);\n\tf2(1,2,3);\n\treturn 0;\n}\n");
	
	return 0;
}
