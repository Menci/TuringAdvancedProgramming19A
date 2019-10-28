#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "table.h"
#include "double.h"

#define BUFF_SIZE 10000010

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define ull __uint128_t
#define ul uint64_t

double out(ul t) {
	double tmp;
	memcpy(&tmp,&t,sizeof(double));
	return tmp;
}
ull inputNumber(char **c) {
	static bool iosig;
	ull x=0;
	for (iosig = false;!isdigit(**c);(*c)++) {
		if (**c == -1) return 0;
		if (**c == '-') iosig = true;	
	}
	for (x = 0; isdigit(**c);(*c)++) {
		x=mul(x,pow10[1]);
		x=add(x,mpow[((**c)^'0')]);
	}
	ull tx=0;
	if(**c=='.') {
		(*c)++;
		int cnt=1;
		for (; isdigit(**c); (*c)++) {
			tx=mul(tx,pow10[1]);
			tx=add(tx,mpow[((**c)^'0')]);
			cnt++;
		}
		tx=mul(tx,mpow10[cnt-1]);
	}
	if (iosig) x = -x;
	(*c)--;
	return add(x,tx);
}
ull expression[BUFF_SIZE],tmp2[BUFF_SIZE];
ull basis;
ull *stack=tmp2-1,*expression2=expression-1;
int getline_(char s[],int lim){
	int c,i;
	i=0;
	while((c=getchar())!=EOF&&c!='\n'&&i<lim-1)
		s[i++]=c;
	s[i]='\0';
	return i;
}


int input() {
	static char tmp[BUFF_SIZE];
	static int rank[1000];
	stack=tmp2-1,expression2=expression-1;
	{
		rank[43]=rank[45]=1;
		rank[42]=rank[47]=2;
		rank[40]=rank[41]=0;
	}
	if(!getline_(tmp,BUFF_SIZE)) return 0;
	int len=strlen(tmp);
	for(char* i=tmp;i<tmp+len;i++) {
		if(isdigit(*i)) {
			expression2++;
			*expression2=inputNumber(&i);
		} else if(*i=='+'||*i=='-'||*i=='*'||*i=='/'){
			while(stack>=tmp2&&rank[*stack]>=rank[*i]) {
				expression2++;
				*expression2=basis+*stack;
				stack--;
			}
			stack++;
			*stack=*i;
		} else if(*i=='('||*i==')') {
			if(*i=='(') stack++,*stack=*i;
			else {
				while(stack>=tmp2&&*stack!='(') {
					expression2++;
					*expression2=basis+*stack;
					stack--;
				}
				if(stack>=tmp2) stack--;
			}
		}
	}
	while(stack>=tmp2) {
		expression2++;
		*expression2=basis+*stack;
		stack--;
	}
	/*for(ull *i=expression;i<=expression2;i++) {
		if(*i>basis) printf("%c ",(char)(*i-basis));
		else printf("%lf ",out(*i));
	}
	puts("");*/
	return 1;
}
ull calc() {
	stack=tmp2-1;
	for(ull *i=expression;i<=expression2;i++) {
		if(*i<basis) stack++,*stack=*i;
		else {
			ull a,b;
			a=*stack,stack--;
			b=*stack,stack--;
			stack++,*stack=Double(b, a, (char)(*i-basis));
		}
	}
	return *stack;
}
void print(ull x) {
	if(x&(1ull<<63)) {
		printf("-");
		x^=(1ull<<63);
	}
	int fir=0;
	while(x>=pow10[fir]) fir++;
	fir--;
	while(fir>=0) {
		int tmp=0;
		while(x>=pow10[fir]) {
			x=Double(x,pow10[fir],'-');
			tmp++;
		}
		printf("%d",tmp);
		fir--;
	}
	printf(".");
	while(fir>=-30) {
		int tmp=0;
		while(x>=mpow10[-fir]) {
			x=Double(x,mpow10[-fir],'-');
			tmp++;
		}
		printf("%d",tmp);
		fir--;
	}
	printf("\n");
}
int main() {
	basis=1;
	basis<<=70;
	while(input()!=0) {
		ull ans=calc();
		print(ans);
		fflush(stdout);
	}
}