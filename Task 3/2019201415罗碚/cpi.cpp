#include<stdio.h>
#include<string.h>
#include<stdlib.h>

const int N=16,M=3;

int x[500],y[500]; //x[i]表示变量v[i]的位置 y[i]表示第i号位置所存的是哪一个变量 
int a[500],ta[500],b[500],tb[500],c[500],tc[500];
char op[500];
int last[500];

void getv(int *res,int *typ)
{
	char s[20];
	scanf("%s",s);
	if(strlen(s)==0)
	{
		*typ=-1;
		return ; 
	}
	switch(s[0])
	{
		case 'a':
			sscanf(&s[1],"%d",res);
			*typ=0;
			return ;
		case 'v':
			sscanf(&s[1],"%d",res);
			*typ=1;
			return ;
		default:
			sscanf(s,"%d",res);
			*typ=2;
			return ;
	}
}

void trans(int xx)
{
	int i,t=-1;
	for(i=0;i<N;i++)
		if(y[i]<0)
		{
			t=i;
			break;
		}
	if(t<0)
	{
		t=rand()&15;
		for(i=N;;i++)
			if(y[i]<0)
			{
				printf("array[%d] = x%d;\n",i-N,t);
				x[y[t]]=i;
				y[i]=y[t];
				y[t]=-1;
				break;
			}
	}
	y[t]=a[xx];
	x[y[t]]=t;
	switch(tb[xx])
	{
		case 0:
			printf("x%d = a%d;\n",t,b[xx]);
			break;
		case 1:
			if(x[b[xx]]<N)
				printf("x%d = x%d;\n",t,x[b[xx]]);
			else
				printf("x%d = array[%d];\n",t,x[b[xx]]-N);
			break;
		case 2:
			printf("x%d = %d;\n",t,b[xx]);
			break;
	}
	switch(tc[xx])
	{
		case 0:
			printf("x%d %c= a%d;\n",t,op[xx],c[xx]);
			break;
		case 1:
			if(x[c[xx]]<N)
				printf("x%d %c= x%d;\n",t,op[xx],x[c[xx]]);
			else
				printf("x%d %c= array[%d];\n",t,op[xx],x[c[xx]]-N);
			break;
		case 2:
			printf("x%d %c= %d;\n",t,op[xx],c[xx]);
			break;
	}
	if(last[b[xx]] == xx)
		x[b[xx]]=y[b[xx]]=-1;
	if(last[b[xx]] == xx)
		x[b[xx]]=y[b[xx]]=-1;
}

int main()
{
	freopen("2.txt","r",stdin);
	
	memset(x,-1,sizeof(x));
	memset(y,-1,sizeof(y));
	memset(last,-1,sizeof(last));
	memset(a,0,sizeof(a));
	memset(b,0,sizeof(b));
	memset(c,0,sizeof(c));
	
	int i;
	for(i=0;i<60;i++)
	{
		char s[50];
		getv(&a[i],&ta[i]);
		if(ta[i]<0) break;
		scanf("%s",s);
		getv(&b[i],&tb[i]);
		scanf("%s",s);
		op[i]=s[0];
		getv(&c[i],&tc[i]);
	//	printf("%c%d = %c%d %c %c%d;\n",ta[i]==0?'a':(ta[i]==1?'v':' '),a[i],tb[i]==0?'a':(tb[i]==1?'v':' '),b[i],op[i],tc[i]==0?'a':(tc[i]==1?'v':' '),c[i]);
	}
	while(i)
	{
		i--;
		if(tb[i]==1)
			if(last[b[i]]<0) last[b[i]]=i;
		if(tc[i]==1)
			if(last[c[i]]<0) last[c[i]]=i;
	}
	for(i=0;i<60;i++)
		trans(i);
    return 0;
}
