#include<stdio.h>
#include<stdlib.h> 
#include<math.h>
#include<string.h>
#include<ctype.h>
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
void double_init(){
	cnt=dp=0;
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
	if(S(a)) {A=-A;} if(S(b)) B=-B;
	C=A+B;
	s=C<0?1:0;
	if(C<0)C=-C;
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
	
	
	int s=S(a)^S(b),aE=getE(a),bE=getE(b),cE;
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
#define o 10005
struct Double Num[o];
int Ans[o],A[o],B[o],C[o],D[o],QX[o],QZ[o],ZE[o],W[1105][1105],U[o],V[o],RR[o],I[o];
int fls,num,top;
char St[o],sa[o];

void jiafa(int A[], int B[], int C[]){        //C=A+B
    int i, jw;                              //i从1循环至max(A[0], B[0])
    //for( i = 0 ;i <= 1500+A[0]+B[0] ; i++ ) C[i]=0;
    C[0] = 0;
    for(i=1, jw=0; i<=A[0] || i<=B[0]; i++){
        C[i]=A[i]+B[i]+jw;                  //按位相加 
        jw=C[i]/10;                         //求进位 
        C[i]%=10;                           //与10求余
    }
    C[i]=jw;                                //存放最后一个进位
    C[0]=C[i] ? i : i-1;                    //确定C的位数
    return;
}
void init_jiafa(int a,int b){        //C=A+B
    int i, jw;                              //i从1循环至max(A[0], B[0])
    for(i=1, jw=0; i<=W[a][0] ; i++){
        W[b][i]=W[a][i]+W[a][i]+jw;                  //按位相加 
        jw=W[b][i]/10;                         //求进位 
        W[b][i]%=10;                           //与10求余
    }
    W[b][i]=jw;                                //存放最后一个进位
    W[b][0]=W[b][i] ? i : i-1;                    //确定C的位数
    return;
}
bool compare(int L[], int R[]){             //实现两个高精度数字的大小比较 
    if(L[0]>R[0])       return true;        //位数高的大
    if(L[0]<R[0])       return false;       //位数低的小
    int i;
    for(i=L[0]; i>=1; i--){                 //位数相同逐位比较
        if(L[i]>R[i])   return true;        //数字大的大
        if(L[i]<R[i])   return false;       //数字小的小
    }
    return true;                            //能执行到这里说明相等，返回true
}
bool comparebig(int L[], int R[]){             //实现两个高精度数字的大小比较 
    if(L[0]>R[0])       return true;        //位数高的大
    if(L[0]<R[0])       return false;       //位数低的小
    int i;
    for(i=L[0]; i>=1; i--){                 //位数相同逐位比较
        if(L[i]>R[i])   return true;        //数字大的大
        if(L[i]<R[i])   return false;       //数字小的小
    }
    return false;                            //能执行到这里说明相等，返回true
}
void jianfa(int L[], int R[], int C[]){  //C=L-R
    int i;
   // for( i = 0 ;i <= 1500+L[0]+R[0] ; i++ ) C[i]=0;
   	C[0] = 0;
    for(i=1; i<=L[0]; i++)
        C[i]=L[i];                          //把L的所有数位复制到C
    C[0]=L[0];                              //C的位数暂等于L的位数
    for(i=1; i<=R[0]; i++){                 //右值有多少位，就减多少次
        if(C[i]<R[i]){
            C[i+1]--;                       //向高位借1 
            C[i]+=10;                       //当前位+10 
        }
        C[i]-=R[i];                         //按位减 
    }
    if(i<C[0])  return;                     //计算有效位数：L[0]-R[0]>=2
    else{                                   //计算有效位数：L[0]-R[0]==1 or 0 
        while(C[i]==0 && i>=1)  i--;
        C[0]=(i==0) ? 1 : i;                //例如10000-9999
    }
    return;
}
void chengfa(int A[], int B[], int C[]){        //C=A*B
    int i,j;
    for( i = 0 ;i <= 1500+A[0]+B[0] ; i++ ) C[i]=0;
    for(i=1; i<=B[0]; i++){                     //B[0]轮乘法
        int jw;                                 //进位 
        for(j=1, jw=0; j<=A[0]; j++){           //每轮按位乘A[0]次
            C[i+j-1]=C[i+j-1]+A[j]*B[i]+jw;     //错位相加
            jw=C[i+j-1]/10;                     //求进位
            C[i+j-1]%=10;                       //进位后，与10求余
        }
        C[i+A[0]]=jw;                           //存储最后一个进位
    }
    int len=A[0]+B[0];                          //最大可能位数 
    while(C[len]==0 && len>1)   len--;          //确定有效位数
    C[0]=len;
    return;
}
double try[o];
void init()
{
	memset(Ans,0,sizeof(Ans));
	memset(A,0,sizeof(A));
	memset(B,0,sizeof(B));
	memset(C,0,sizeof(C));
	memset(D,0,sizeof(D));
	memset(I,0,sizeof(I));
	memset(QX,0,sizeof(QX));
	memset(QZ,0,sizeof(QZ));
	memset(ZE,0,sizeof(ZE));
	memset(W,0,sizeof(W));
	memset(U,0,sizeof(U));
	memset(V,0,sizeof(V));
	memset(RR,0,sizeof(RR));
	memset(sa,0,sizeof(sa));
	fls=num=top=0;
	memset(St,0,sizeof(St));
    num = 10;
    W[0][0] = 1;W[0][1] = 1;
    W[1][0] = 1;W[1][1] = 2;
    int i;
    Num[9] = build( 0 , 0+bias , DDB );
    try[9]=1.0;
    try[10]=-1.0;
    Num[10] = build( 1 , 0+bias , DDB );
    for( i = 1 ; i <= 1100 ; i++ ) init_jiafa( i , i+1 );
}
int what( char a )
{
    if( a == '+' ) return 1;
    if( a == '-' ) return 2;
    if( a == '*' ) return 3;
    if( a == '/' ) return 4;
}
bool solve()
{
	char a;
	bool last=1; //上一个是不是运算符
	
	
	if( gets(sa) == NULL ) return 0;
	int len = strlen(sa),i,j,h,k,r;
	char lw=' ';
	for( i = 0 ;i < len ; i++ )
	{
		a = sa[i] ;
		if( a == ' ') continue;
		last = 0;
		if( !i ) last=1;
		else if( !isdigit( lw ) && lw != ')' ) last=1;
		
		lw = sa[i];
		if( last &&( a == '+' || a == '-' ) )
		{
			if( a == '+') Ans[++fls]=9; //数字处理
			if( a == '-') Ans[++fls]=10; //数字处理
			a = '*' ; last = 1 ; lw = a;
		}
		if( isdigit( a ) )
		{
		    last = 0;
		    j = i;
		    while( j+1 <= len-1 && ( sa[j+1]=='.' || isdigit( sa[j+1] ) ) ) j++;
		    
		    lw = sa[j] ;
		    
		    for( k = 0 ; k <= 1100 ; k++ ) QZ[k] = QX[k] = ZE[k] = A[k] = B[k] = V[k] = 0;
		    A[0] = B[0] = 0;
		    //double try	
			double x=0,y=0.1;
		    for( h = i ; h <= j && sa[h] !='.' ; h++ ) ;
		    for( r = h-1 ; r >= i ; r-- ) A[++A[0]] = sa[r] - '0';
		   
		    h++;
		    for( r = j ; r >= h ; r-- ) B[++B[0]] = sa[r] - '0';
		    
		    for( r = A[0] ; r >= 1 ; r-- ) x = x*10 + (double)A[r];
		    for( r = B[0] ; r >= 1 ; r--,y*=0.1 ) x += y * (double)B[r];
		    for( k = 0 ; k <= 1100 ; k++ )
		    {
		        C[0]=W[k][0];
		        for( h = 1; h <= C[0] ; h++ ) C[h] = W[k][h];
		        if( comparebig( C , A ) ) break;
		    }
		    k--;
		    //for( r = 1; r <= A[0] ; r++ ) printf("%d",A[r]);
		    //printf("\n");
		    //for( r = 1; r <= B[0] ; r++ ) printf("%d",B[r]);
		    //printf("????\n");
		    //printf("k: %d\n",k);
		    if ( k >= 1025 ) 
		    {
		        Num[++num] = INF;
		        try[num] = x;
		        //printf("%.30lf\n",x);
		        Ans[++fls] = num;
		        i = j;
		        continue;
		    }
		    if( k != -1 )
		    {
		        while( k >= 0 && A[0] )
		        {
		            //printf("C:");
		            C[0]=W[k][0];
		            for( h = 1; h <= C[0] ; h++ ) C[h] = W[k][h];//printf("%d",C[r]);
		            
		            if( compare ( A , C ) )
		            {
		                QZ[k] = 1; 
		                jianfa(  A , C, D );
		                for( r = 0 ; r <= D[0] ; r++ ) A[r] = D[r];//printf("%d",D[r]);
		                //printf("\n");
		                
		            }
		            k--;
		       
		        }
		    }
		    
		    if( B[0] )
		    {
		        int rem = 1;
		        while( rem <= 1100 )
		        {
		            jiafa( B , B , D );
		            if( D[0] > B[0] )
		            {
		                QX[rem] = 1;
		                D[0]--;
		            }
		            for( r = 0 ; r <= D[0] ; r++ ) B[r] = D[r];
		            rem++;
		        }		       
		    }
		    int li = -1,ri = 0,E = 0 ;
		    for( r = 0 ; r <= 1100 ; r++ ) if( QZ[r] ) li = r;
		    for( r = 1 ; r <= 1100 ; r++ ) if( QX[r] ) ri = r;
		    //printf("%d %d\n",li,ri);
		    if( li == -1 )
		    {
		        for( r = 1 ; r <= 1100 ; r++ )  
		            if( QX[r] )
		            {
		                li = r;
		                break;
		            }
		        if( li == -1 )
		        {
		             Num[++num] = ZERO;
		             Ans[++fls] = num;
		             i = j;
		             continue;
		        }
		        for( r = li; r <= ri ; r++ ) ZE[++ZE[0]] = QX[r];
		        E = -li;
		        
		    }
		    else
		    {
		        for( r = li; r >= 0 ; r-- ) ZE[++ZE[0]] = QZ[r];
		        for( r = 1; r <= ri ; r++ ) ZE[++ZE[0]] = QX[r];
		        E = li;
		    } 
		    //for ( r  = 1 ; r <= ZE[0] ; r++ ) printf("%d",ZE[r]);
		    //printf("\n");
		    bool ok=0; //长度55
		    for ( r  = 56 ; r <= ZE[0] ; r++ ) if( ZE[r] ) ok = 1;
		    if( ok ) ZE[55] = 1;
		    E-=2;
		    unsigned long long Frac=0;
		    for( r = 1 ; r <= 55 ; r++ )
		    {
		        Frac<<=1;
		        if( ZE[r] ) Frac|=1;
		    }
		    //printf("E: %d\n",E);
		    Frac = Round( Frac , &E );
		    //printf("E: %d\n",E);
		    
		    Num[++num] = build( 0 , E+bias , Frac );
		    try[num] = x;
		    //printf("%.30lf\n",x);
		   // printf("sb %d\n",checknan(Num[num]));
		    Ans[++fls] = num;
		    //printf("Ans::: %lf\n",*((double*)&Num[num].W));
		    i = j;
		    continue;
		}
		if( a == '(' ) St[++top] = a;
		if( a == ')' )
		{
			while( top && St[top] != '(' ) Ans[++fls] = what( St[top--] ) ;
			top--;
		}
		if( a == '*' || a == '/' )
		{
		    while( top && ( St[top] == '*' || St[top] == '/' ) )  Ans[++fls] = what( St[top--] );
		    St[++top] = a;
		}
		if( a == '+' || a == '-' )
		{
		    while( St[top] != '(' && St[top] != ')' && top )  Ans[++fls] = what( St[top--] );
		    St[++top] = a;
		}
	}
	while( top ) Ans[++fls] = what( St[top--] );
	struct Double X;
	for( i = 1; i <= fls ; i++ )
	{	
	    //printf("ANS %d\n ",Ans[i]);
	    if ( Ans[i] >= 9 ) 
	    {
	        U[++top] = Ans[i];   
	    }
	    else
	    {   double x;
	        int aa = U[top-1],bb = U[top];
	        top-=2;
	        if( Ans[i] == 1) X = add( Num[aa] , Num[bb] ),x = try[aa] + try[bb];
	        if( Ans[i] == 2) X = sub( Num[aa] , Num[bb] ),x = try[aa] - try[bb];
	        if( Ans[i] == 3) X = mul( Num[aa] , Num[bb] ),x = try[aa] * try[bb];
	        if( Ans[i] == 4) X = Div( Num[aa] , Num[bb] ),x = try[aa] / try[bb];
	        Num[++num] = X;
	        try[num] = x;
	        U[++top] = num;
	       // printf("%.30lf\n",x);
	        //printf("Ans::: %lf\n",*((double*)&X.W));
	        //printf("sb %d\n",checknan(X));
	    }
	}
	//printf("%d\n",U[1]);
	if( check0( Num[U[1]] ) )
	{
		printf( "0.00000000" );
		return 1;
	}
	if( checkinf( Num[U[1]] ) )
	{
		printf( "inf" );
		return 1;
	}
	if( checknan( Num[U[1]] ) )
	{
		printf( "nan" );
		return 1;
	}
	unsigned long long M;
	int E,si;
	M = Frac( Num[U[1]] );
	si = S( Num[U[1]] );
	E = getE( Num[U[1]] );
	E-=52;
	memset(V,0,sizeof(V));memset(D,0,sizeof(D));
	//printf("Ans::: %lf\n",*((double*)&Num[U[1]].W));
	
	if( si ) printf("-");
	//printf("%lld\n",M);
	//double ai=(double) (1LL<<(-E)),bi=(double)M;
	//printf("%lf %lf\n",ai,bi);
	//printf("SSS %lf\n",bi/ai);
	while( M )
	{
		V[++V[0]] = M%10LL;
		M/=10LL;
	}
	//printf("%.30lf",try[U[1]]);
	//return 1;
	//printf("E: %d\n",E);
	if( E >= 0 )
	{
		
		for( i = 1 ; i <= E ; i++ ) 
		{
			jiafa( V , V , D );
			for( r = 0 ; r <= D[0] ; r++ ) V[r] = D[r];
		}
		//printf("%.30lf",try[U[1]]);
		//return 1;
		for( i = V[0] ; i >= 1; i-- ) printf( "%d" , V[i] );
		printf(".00000000");
		return 1;
	}
	
	if( E < 0 ) 
	{
		I[0]=1;I[1]=5;
		for( i = 1 ; i <= -E ; i++ ) 
		{
			//memset(D,0,sizeof(D));
			chengfa( V , I , D );
			for( r = 0 ; r <= D[0] ; r++ ) V[r] = D[r];
		}
		int pos = -E - 20;
		if( pos > 0 )
		{
			RR[0] = pos+1;
			RR[pos+1] = 1;
			jiafa( V , RR , D );
			for( r = 0 ; r <= D[0] ; r++ ) V[r] = D[r];
		}
		//printf("L %d",V[0]);
		if( -E >= V[0] ) V[0] = -E + 1;
		for( i = V[0] ; i > -E ; i-- ) printf( "%d" , V[i] );
		printf( "." );
		if( pos <= 0 ) pos = 0;
		for( i = -E  ; i > pos ; i-- ) printf( "%d" , V[i] );
		
		
		return 1;
	}
	
	return 1;
}


int main(){
	double_init();
	while(1)
	{
		init();
		fflush(stdout);
		if(!solve())break;
		printf("\n");
		fflush(stdout);
	}	
	
	
	
	return 0;
}
