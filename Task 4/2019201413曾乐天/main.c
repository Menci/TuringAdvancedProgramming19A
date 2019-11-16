#include<stdio.h>
#include<assert.h>

#define SIZE 100
#define LEN 100

char s[LEN];
int rspt;

char ram[10][10]={"","rcx","r8","r9","r10","r11"};
char arg[10][10]={"","rdi","rsi","r10"};

int is_digit(int x)
{
	return '0'<=x && x<='9';
}

void carr(int index)
{
	if(index==rspt)
		printf("(%%rsp)");
	else
		printf("%d(%%rsp)",8*(rspt-index));
}

void creg(int index)
{
	printf("%%%s",ram[index]);
}

void cop(char c)
{
	switch(c)
	{
		case '+':printf("addq ");break;
		case '-':printf("subq ");break;
		case '*':printf("imulq ");break;
		case '/':printf("idivq ");break;
		default: printf("faq ");break;
	}
}

void ed()
{
	printf("\n");
}

int readi(char *ch)
{
	int x=0,f=1,p=0;
	while(ch[p]<'0' || '9'<ch[p]){if(ch[p]=='-')f=-1;p++;}
	while('0'<=ch[p] && ch[p]<='9')x=x*10+(ch[p]^48),p++;
	return x*f;
}

void work()
{
	int len=0,tar_id;
	char ch,op;
	
	rspt=0;
	printf("movq $0,%%rax\n");
	printf("movq %%rdx,%%r10\n");
	while((ch=getchar())!=EOF)
	{
		while(len)s[len--]=' ';
		len=0;
		while(ch!='\n')
		{
			s[++len]=ch;
			ch=getchar();
		}
		if(len<2)continue;

		int ptr=1;
		int id1,arg1;
		int id2,arg2;

		while(!is_digit(s[ptr]))ptr++;
		tar_id=readi(s+ptr);

		//printf("tar_id %d\n",tar_id);

		while(is_digit(s[ptr]))ptr++;
		while(s[ptr]!='=')ptr++;
		while(s[++ptr]==' ');

		if(s[ptr]=='a')
			id1=-readi(s+ptr+1);
		else
			id1=readi(s+ptr+1);

		while(is_digit(s[++ptr]));
		while(s[ptr]==' ')ptr++;
		
		//printf("%s %d\n",s+1,ptr);

		op=s[ptr];
		
		while(s[++ptr]==' ');
		if(is_digit(s[ptr]) || s[ptr]=='-')
		{
			id2=0;
			arg2=readi(s+ptr);
		}
		else if(s[ptr]=='a')
			id2=-readi(s+ptr+1);
		else
			id2=readi(s+ptr+1);
		
		if(op!='/')
		{

			printf("movq ");
			if(id1<0)
				printf("%%%s,",arg[-id1]);
			else
				carr(id1),printf(",");
			creg(1);ed();
		
			cop(op);
			if(!id2)
				printf("$%d,",arg2);
			else if(id2<0)
				printf("%%%s,",arg[-id2]);
			else
				carr(id2),printf(",");
			creg(1);ed();

			printf("pushq ");creg(1);ed();
			rspt++;	
			printf("xor ");creg(1);printf(",%%rax\n");
		}
		else
		{
			printf("movq %%rax,");creg(1);ed();

			printf("movq ");
			if(id1<0)
				printf("%%%s,",arg[-id1]);
			else
				carr(id1),printf(",");
			printf("%%rax");ed();

			printf("cqto\n");

			if(!id2)
			{
				printf("movq $%d,",arg2);
				creg(2);ed();
				printf("idivq ");creg(2);ed();
			}
			else
			{
				printf("idivq ");
				if(id2<0)
					printf("%%%s",arg[-id2]);
				else
					carr(id2);
				ed();
			}		
			
			printf("pushq %%rax\n");
			rspt++;	
			printf("xor ");creg(1);printf(",%%rax\n");
		}
		printf("\n");
	}

	while(tar_id--)printf("popq "),creg(1),printf("\n");
	printf("ret\n");
}

int main()
{
	if(0)
	{
		freopen("in.txt","r",stdin);
		freopen("out.txt","w",stdout);
	}
	
	work();
	return 0;
}

