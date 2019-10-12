#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define max(a,b)(a>b?a:b)
#define min(a,b)(a<b?a:b)

#define nan 18442240474082181120ull
#define inf 9218868437227405312ull

#define ull __uint128_t
#define ul uint64_t
#define exp(a) (((a)>>52u)&((1u<<11u)-1u))
#define exp1(a) (exp(a)?exp(a):1)
#define sign(a) ((a)>>63)
#define frac(a) ((a)&((1ull<<52)-1))

#define check(a,b) if(exp(a)==2047&&frac(a)) return (ul)a;\
if(exp(b)==2047&&frac(b)) return (ul)b;\
if(b==inf||b==nan) return (ul)b;\
if(a==inf||a==nan) return (ul)a;

void swap(ull *a,ull *b) {
	static ull tmp;
	tmp=*a,*a=*b,*b=tmp;
}
ull Merge(ull sign,ull exp,ull frac) {
	return (((sign<<11)+exp)<<52)+frac;
}
ull Round(ull s) {
	ull tmp=1,num=128;
	tmp<<=127;
	while((s&tmp)==0&&tmp) tmp>>=1,num--;
	if(s%((ull)1<<(num-54))) s+=((ull)1)<<(num-54);
	else if(s&((ull)1<<(num-53))) s+=((ull)1)<<(num-54);
	if(s&(tmp<<1)) num++;
	return s>>(num-53);
}
ul add(ul A,ul B) {
	ull a=A,b=B;
	if(exp(a)>exp(b)) swap(&a,&b);
	if(exp(a)==exp(b)&&frac(a)>frac(b)) swap(&a,&b);
	ull sign=sign(b),frac,exp;
	check(a, b);
	exp=exp(b),frac=frac(a);
	if(exp(a)) frac+=(1ull<<52);
	frac<<=54,frac>>=min(127,(exp1(b)-exp1(a)));
	if(sign(a)==sign(b)) {
		if(exp(b)) frac+=((frac(b)+(1ull<<52))<<54);
		else frac+=(frac(b)<<54);
		ull tmp=frac;
		frac=Round(frac);
		while(tmp>=(1ull<<53)) exp++,tmp>>=1;
		while(exp<54) tmp>>=1,exp++,frac>>=1; 
		exp-=54;
		if((frac&(1ull<<52))&&exp==0) exp++;
		frac%=(1ull<<52);
	} else {
		if(exp(b)) frac=(((1ull<<52)+frac(b))<<54)-frac;
		else frac=(frac(b)<<54)-frac;
		if(frac==0) return 0;
		ull tmp=frac,tmpp=frac,cnt=54;
		frac=Round(frac);
		tmp>>=54;
		while(exp>1&&tmp<(1ull<<52)) exp--,tmp<<=1,cnt--;
		if(exp==1&&tmp<(1ull<<52)) exp--;
		if(exp==0) frac=tmpp>>cnt;
		frac%=(1ull<<52);
	}
	if(exp>=2048||(exp==2047&&frac)) return sign(b)?nan:inf;
	return (ul)Merge(sign,exp,frac);
}
ul mul(ul a,ul b) {
	check(a, b);
	ull sign=sign(a)^sign(b),frac=frac(a);
	int exp=max(exp(a),1)+max(1,exp(b))-1023;
	if(a==0) return b;
	if(b==0) return a;
	if(exp(a)==0) {
		while(!(frac&(1ull<<52))) frac<<=1,exp--;
	}
	if(exp(a)) frac+=(1ull<<52);
	ull fracb=frac(b);
	if(exp(b)==0) {
		while(!(fracb&(1ull<<52))) fracb<<=1,exp--;
	}
	frac*=fracb+(exp(b)?(1ull<<52):0);
	ull ss=1;
	ss<<=105;
	if(frac==0) return 0;
	while(frac>ss) frac>>=1,exp++;
	if(exp<=0) {
		int last=frac%(1ull<<52);
		frac>>=52;
		while(exp<0&&frac) {
			last|=(frac&1);
			frac>>=1;
			exp++;
		}
		if(exp==0) {
			if((frac&1)&&last) frac+=2;
			frac>>=1;
		}
		if(exp<0) return 0;
	}else frac=Round(frac);
	frac%=(1ull<<52);
	if(exp>=2047) return inf;
	return Merge(sign,exp,frac);
}
ul dev(ul a,ul b) {
	ull sign=sign(a)^sign(b),fraca=frac(a),fracb=frac(b),frac;
	int exp=0;
	check(a, b);
	if(exp(a)==0&&fraca==0) return a;
	if(exp(b)==0&&fracb==0) return inf;
	if(exp(a)) fraca+=(1ull<<52);
	else {while(!(fraca&(1ull<<52))) fraca<<=1,exp--;}
	if(exp(b)) fracb+=(1ull<<52);
	else {while(!(fracb&(1ull<<52))) fracb<<=1,exp++;}
	exp+=max(exp(a),1)-max(exp(b),1)+1023;
	if(fraca<fracb) exp--;
	frac=(fraca<<70)/fracb;
	if(fraca%fracb&&(frac&1)==0) frac++;
	ull last=0,pos=1;pos<<=127;
	while(!(frac&pos)){
		pos>>=1;
	}
	pos>>=52;
	last=(frac%pos)?1:0;
	if(exp==0) last&=((frac&pos)?1:0);
	if(exp>=2047) return inf;
	if(exp<=0) {
		frac/=pos;
		while(exp<0&&frac) {
			last|=(frac&1);
			frac>>=1;
			exp++;
		}
		if(exp==0) {
			if((frac&1)&&last) frac+=2;
			frac>>=1;
		}
		if(exp<0) return 0;
	} else frac=Round(frac);
	frac%=(1ull<<52);
	return Merge(sign,exp,frac);
}
ul Double(ul a,ul b,char op) {
	if(op=='+') return add(a,b);
	if(op=='-') return add(a,b^(1ull<<63));
	if(op=='*') return mul(a,b);
	if(op=='/') return dev(a,b);
}
int main() {
}