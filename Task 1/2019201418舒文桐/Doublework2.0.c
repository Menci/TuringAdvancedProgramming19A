#include<stdio.h>
#include<stdlib.h> 
#include<math.h>
#include <stdint.h>
#include <stdbool.h> // bool
#define ui unsigned int
#define ul unsigned long long
#define uc unsigned char
const int bias=1023;
const unsigned long long DDB=(ul)1<<52;
const __int128 HH=(ul)1<<52;;
const ul TL=(ul)1<<63;
const ul G2=(ul)1<<11;
//干掉常量 
struct  Double
{
	ul W;
	//ui s,Exp;
	//ul Frac;
};
int cnt,dp;
struct  Double NaN,INF,ZERO;
ui S(struct  Double x)
{
	return (x.W>>63)&1;
}
ui Exp(struct Double x)
{
	ui ans=x.W>>52;
	if(ans>=G2) ans-=G2;
	return ans;
}
ul Frac(struct Double x)
{
	ul ans=x.W-((x.W>>52)<<52);
	if(Exp(x)) ans+=DDB;
	return ans;
}
int min(int a,int b)
{
	return a<b?a:b;
}
int check0(struct Double a)
{
	if(!Exp(a)&&!Frac(a)) 
		return 1;
	else
		return 0;
}
int checkdem(struct Double a)
{
	if(!Exp(a)&&Frac(a))
		return 1;
	else 
		return 0;
}
int checkinf(struct Double a)
{
	if(Exp(a)==2047&&!Frac(a))
		return 1;
	else
		return 0;
}
int checknan(struct Double a)
{
	if(Exp(a)==2047&&Frac(a))
		return 1;
	else
		return 0;
}
struct  Double build(ui s,ui exp,ul frac){
	struct  Double X;
	if(frac>=DDB) frac-=DDB;
	X.W=s;
	X.W=(X.W<<11)+exp;
	X.W=(X.W<<52)+frac;
	return X;
}


struct Double trans(uint64_t x)
{
	struct Double X;
	X.W=(ul)x;
	return X;
}
void init(){
	NaN=build(0,2047,1);
	INF=build(0,2047,0);
	ZERO=build(0,0,0);
}

int Len( __int128 x )
{  //求位数 
	if(!x)return 0;
	int len=-1;if(x<0)x=-x;
	while(x)
	{
		len++;
		x>>=1;
	}
	return len;
}
__int128 Round(__int128 C,int *E){
	//等于0呢？
	int len;
	__int128 x=1,s=1;
	if(C<0)
	{
		C=-C;
		s=-1;
	}
	len=Len(C);
	if(len<52) 
	{
		C=C<<(52-len);
		*E-=52-len;
		len=Len(C);
	}
	
	int f=len-52;
	if(*E+f>=-1022)
	{
		int y=f;
		*E+=y;
		if(y)
		{
			if(C&(x<<(y-1)))
			{
				if(C>( (C>>(y-1))<<(y-1) )|| dp)
					C=(C>>y)+1;
				else //1/2
				{
					if(C&(x<<y))
						C=(C>>y)+1;
					else
						C=C>>y;		
				}
			}
			else
				C=C>>y;
		}
		len=Len(C);
		if(len==53)
		{
 	  		C>>=1;
      		*E+=1;
    	}
	}
	else
	{
		int y=-1022-*E;
		*E+=y;
		if(y>=len+3)
		{
			*E=-bias;
			C=0;
		}
		else
		if(y)
		{
			if(C&(x<<(y-1)))
			{
				if(C>( (C>>(y-1))<<(y-1) ) || dp)
					C=(C>>y)+1;
				else //1/2
				{
					if(C&(x<<y))
						C=(C>>y)+1;
					else
						C>>=y;		
				}
			}
			else
				C>>=y;
		}
		if(Len(C)<52) *E=-bias;
	}
	return s*C;
}
int getE(struct Double a)
{
	if(Exp(a)) 
		return ((int)Exp(a)) - bias;
	else
		return 1-bias;
}
__int128 Mmul(ul a,ul b,int *E){
	if(!b||!a)return 0;
	
	__int128 A=a,B=b,C=A*B;
	int len=Len(C),lena=Len(A),lenb=Len(B); 	//lena+lenb小数点前一位 
	*E+=52;
	return Round(C,E);
}
struct  Double mul(struct  Double a,struct  Double b){
//特判
	if(checknan(a)) return a;
	if(checknan(b)) return b;
	if(checkinf(a)&&checkinf(b)) return NaN;
	if(checkinf(a)&&check0(b)) return NaN;
	if(checkinf(b)&&check0(a)) return NaN;
	if(checkinf(a))
	{
		a.W^=((ul)S(b)<<63);
		return a;
	}
	if(checkinf(b))
	{
		b.W^=((ul)S(a)<<63);
		return b;
	}
	if(check0(a))
	{
		a.W^=((ul)S(b)<<63);
		return a;
	}
	if(check0(b))
	{
		b.W^=((ul)S(a)<<63);
		return b;
	}

	
	ui s=S(a)^S(b);
	int aE=getE(a)-52,bE=getE(b)-52;
	int E=aE+bE;
	__int128 cFrac=Mmul(Frac(a),Frac(b),&E);
	if(cFrac<0) cFrac=-cFrac;
	if(E>1023)  return INF;
	return build(s,E+bias,(ul)cFrac);
}
struct  Double add(struct  Double a,struct  Double b){
//特判
	if(checknan(a)) return a;
	if(checknan(b)) return b;
	if(checkinf(a)&&checkinf(b)) return NaN;
	if(checkinf(a)) return a;
	if(checkinf(b)) return b;
	if(check0(a)) return b;
	if(check0(b)) return a;

	
	
	
	int s,aE=getE(a),bE=getE(b),cE;
	if(aE<bE)  //swap
	{
		struct  Double c;
		c=a,a=b,b=c;
		cE=aE,aE=bE,bE=cE;
	}
	if(aE-bE>65) return a; 
	cE=bE;
	__int128 A,B,C;
	A=Frac(a);A=A<<(aE-bE); B=Frac(b);
	if(S(a)) A=-A; if(S(b)) B=-B;
	C=A+B;
	s=C<0?1:0;
	if(C<0)C=-C;
	int len=Len(C);
	C=Round(C,&cE);
	if(cE>1023)  return INF;
	return build(s,cE+bias,(ul)C);
}
struct  Double sub(struct  Double a,struct  Double b){
//特判

	if(checknan(a)) return a;
	if(checknan(b)) return b;
	if(checkinf(a)&&checkinf(b)) return NaN;
	if(checkinf(a)) return a;
	if(checkinf(b)) return b;
	if(check0(a))
	{
		b.W^=((ul)S(a)<<63);
		return b;
	}
	if(check0(b)) return a;
	
	int s,aE=getE(a),bE=getE(b),cE,hehe=0;
	if(aE<bE)  //swap
	{
		struct  Double c;
		c=a,a=b,b=c;
		cE=aE,aE=bE,bE=cE;
		hehe=1;
	}
	if(aE-bE>65)
	{
		a.W^=((ul)hehe<<63);
		return a; 
	}
	cE=bE;
	__int128 A,B,C;
	A=Frac(a);A=A<<(aE-bE); B=Frac(b);
	if(S(a)) A=-A; if(S(b)) B=-B;
	C=A-B;
	s=C<0?1:0;
	if(C<0)C=-C;
	int len=Len(C);
	C=Round(C,&cE);
	if(cE>1023)
	{
		INF.W^=((ul)hehe<<63);
		return INF;
	}
	return build(s^hehe,cE+bias,(ul)C);
}
struct Double Div(struct  Double a,struct  Double b)
{
//特判
	if(checknan(a)) return a;
	if(checknan(b)) return b;
	if(checkinf(a)&&checkinf(b)) return NaN;
	if(checkinf(a))
	{
		a.W^=((ul)S(b)<<63);
		return a;
	}
	if(check0(a))
	{
		a.W^=((ul)S(b)<<63);
		if(check0(b)) return NaN;
		if(checkinf(b)) return a;
		return a;
	}
	
	if(checkinf(b))
	{
		if(S(a)^S(b)) ZERO.W+=TL;
		return ZERO;
	}
	if(check0(b))
	{
		if(S(a)^S(b)) INF.W+=TL;
		return INF;
	}
	
	
	int s=S(a)^S(b),aE=getE(a),bE=getE(b),cE,len;
	cE=aE-bE;
	__int128 A=Frac(a),B=Frac(b),C;
	if(A%B)
	{
		dp=1;
		A=A<<68;
		cE=cE-68;
		C=A/B;
		cE+=52;
		C=Round(C,&cE);
	}
	else
	{
		A=A<<68;
		cE=cE-68;
		C=A/B;
		cE+=52;
		C=Round(C,&cE);
	}
	if(C<0)C=-C;
	
	if(cE>1023)  return INF;
	return build(s,cE+bias,(ul)C);
}
uint64_t Leaves(uint64_t a, uint64_t b, char op) {
    static bool initialized = false;
    if (!initialized) {
        init();
        initialized = true;
    }

    struct Double x = trans(a);
    struct Double y = trans(b);
    struct Double result;
    if (op == '+') result =add(x, y);
    if (op == '-') result =sub(x, y);

    if (op == '*') result =mul(x, y);
    if (op == '/') result =Div(x, y);
    
    return result.W;
}
