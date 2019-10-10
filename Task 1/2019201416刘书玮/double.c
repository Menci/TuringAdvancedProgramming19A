#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define LEN sizeof(double)
#define BYTES_OF_DOUBLE 8
#define BITS_OF_FRAC 52
#define BITS_OF_EXP 11
const int BIAS = (1 << (BITS_OF_EXP - 1)) -1;
struct My_Double
{
	unsigned char sav[BYTES_OF_DOUBLE];
};

struct My_Double My_Double_Nzero, My_Double_Pzero;
struct My_Double My_Double_Pinf, My_Double_Ninf;
struct My_Double My_Double_PNaN, My_Double_NNaN;

struct My_Double ull_to_My_Double(unsigned long long a){
	struct My_Double t;
	memcpy(t.sav, &a, sizeof(double));
	return t;
}
void My_Double_to_number(struct My_Double md, int* sgn, int* exp, unsigned long long* frac)
{
	unsigned long long res = 0;
	memcpy(&res, md.sav, sizeof(double));
	*sgn = res >> (BITS_OF_FRAC + BITS_OF_EXP);
	*exp = (res >> BITS_OF_FRAC) & ( (1 << BITS_OF_EXP) - 1);
	*frac = res & ( (1ll << BITS_OF_FRAC) - 1);
	return;
}
unsigned long long My_Double_to_ull(struct My_Double md){
	unsigned long long res;
	memcpy(&res, md.sav, sizeof(double));
	return res;
}
struct My_Double number_to_My_Double(int sgn, int exp, unsigned long long frac)
{
	unsigned long long res = 0;
	struct My_Double t;
	res += ( (unsigned long long) sgn) << (BITS_OF_FRAC + BITS_OF_EXP);
	res += ( (unsigned long long) exp) << BITS_OF_FRAC;
	res += frac;
	memcpy(t.sav, &res, sizeof(double));
	return t;
}

void Load()
{
    My_Double_Nzero = number_to_My_Double(1, 0x0, 0x0);
    My_Double_Pzero = number_to_My_Double(0, 0x0, 0x0);
    My_Double_Pinf  = number_to_My_Double(0, 0x7ff, 0x0);
    My_Double_Ninf  = number_to_My_Double(1, 0x7ff, 0x0);
    My_Double_PNaN  = number_to_My_Double(0, 0x7ff, 0x1);
    My_Double_NNaN  = number_to_My_Double(1, 0x7ff, 0x1);
    return;
}

void Clear_My_Double(struct My_Double* a)
{
	memset(a, 0, sizeof(a));
	return;
}

struct My_Double inverse_My_Double(struct My_Double a)
{
	a.sav[7] ^= 128;
	return a;
}

int Greater_Than_Zero(struct My_Double a)
{
	if(a.sav[7] >= 128) return 0;
	return 1;
}

int isone(__int128 t,int i){
	__int128 one = 1;
	if(t & (one << i)) return 1;
	return 0;
}
struct My_Double Real_Add(struct My_Double a, struct My_Double b)
{
	int sgn_a, sgn_b, sgn_t, exp_a, exp_b, exp_t;
	unsigned long long frac_a, frac_b, frac_ans, len_t;
	__int128 frac_t, frac_aa, frac_bb, one = 1;
	sgn_t=0,exp_t=0,frac_t=0;
	My_Double_to_number(a, &sgn_a, &exp_a, &frac_a);
	My_Double_to_number(b, &sgn_b, &exp_b, &frac_b);
	sgn_t = sgn_a;
	if(exp_a){
		frac_a |= (1ull << BITS_OF_FRAC);
		exp_a -= 1;
	}
	if(exp_b){
		frac_b |= (1ull << BITS_OF_FRAC);
		exp_b -= 1;
	}
	if(exp_a - exp_b > 53) return a;
	if(exp_b - exp_a > 53) return b;
	if(exp_a > exp_b){
		frac_aa = (one * frac_a) << (exp_a - exp_b);
		frac_bb = frac_b;
		exp_a = exp_b;
	}
	else if(exp_b > exp_a){
		frac_bb = (one * frac_b) << (exp_b - exp_a);
		frac_aa = frac_a;
		exp_b = exp_a;
	}
	else{
		frac_aa = frac_a;
		frac_bb = frac_b;
	}
	frac_t = frac_aa + frac_bb;
	exp_t = exp_a;
    for(len_t = 0;(one << len_t) <= frac_t;len_t ++);
	if(len_t <= 52){
    	frac_ans = frac_t;
    }
    else if(len_t == 53){
    	frac_ans = frac_t;
    	frac_ans -= (1ull << BITS_OF_FRAC);
    	exp_t += 1;
    }
    else{
    	int i, f1 = 0,f2 = 0,f3= 1 ,j;
    	for(i = len_t - 54;i >= 0;i --){
    		if(i == len_t - 54 && isone(frac_t, i)) f1 = 1;
    		else if(i != len_t - 54 && isone(frac_t, i)) f2 = 1, f3 = 0;
    	}
    	frac_ans = frac_t >> (len_t - 53);
    	exp_t += (len_t - 53);
    	if((f1&&f3&&isone(frac_t,len_t - 53))||(f1&&f2)) frac_ans ++;
    	frac_ans -= (1ull << BITS_OF_FRAC);
    	exp_t += 1;
    }
	if(exp_t >= (1 << BITS_OF_EXP) - 1) return My_Double_Pinf;
	return number_to_My_Double(sgn_t, exp_t, frac_ans);
}

struct My_Double Real_Sub(struct My_Double a, struct My_Double b)
{
	int sgn_a, sgn_b, sgn_t, exp_a, exp_b, exp_t;
	unsigned long long frac_a, frac_b, frac_ans, len_t;
	__int128 frac_t, frac_aa, frac_bb, one = 1;
	sgn_t=0,exp_t=0,frac_t=0;
	My_Double_to_number(a, &sgn_a, &exp_a, &frac_a);
	My_Double_to_number(b, &sgn_b, &exp_b, &frac_b);
	if(exp_a){
		frac_a |= (1ull << BITS_OF_FRAC);
		exp_a -= 1;
	}
	if(exp_b){
		frac_b |= (1ull << BITS_OF_FRAC);
		exp_b -= 1;
	}
	if(exp_a - exp_b > 53) return a;
	if(exp_b - exp_a > 53) return b;
	if(exp_a > exp_b){
		frac_aa = (one * frac_a) << (exp_a - exp_b);
		frac_bb = frac_b;
		exp_a = exp_b;
	}
	else if(exp_b > exp_a){
		frac_bb = (one * frac_b) << (exp_b - exp_a);
		frac_aa = frac_a;
		exp_b = exp_a;
	}
	else{
		frac_aa = frac_a;
		frac_bb = frac_b;
	}
	frac_t = frac_aa - frac_bb;
	if(frac_t > 0) sgn_t = sgn_a;
	else if(frac_t == 0) return My_Double_Pzero;
	else sgn_t = sgn_a ^ 1, frac_t = -frac_t;
	exp_t = exp_a;
	while((1ull << BITS_OF_FRAC) > frac_t && exp_t){
		exp_t --;
		frac_t <<= 1;
	}
    for(len_t = 0;(one << len_t) <= frac_t;len_t ++);
	if(len_t <= 52){
    	frac_ans = frac_t;
    }
    else if(len_t == 53){
    	frac_ans = frac_t;
    	frac_ans -= (1ull << BITS_OF_FRAC);
    	exp_t += 1;
    }
    else{
    	int i, f1 = 0,f2 = 0,f3= 1 ,j;
    	for(i = len_t - 54;i >= 0;i --){
    		if(i == len_t - 54 && isone(frac_t, i)) f1 = 1;
    		else if(i != len_t - 54 && isone(frac_t, i)) f2 = 1, f3 = 0;
    	}
    	frac_ans = frac_t >> (len_t - 53);
    	exp_t += (len_t - 53);
		if((f1&&f3&&isone(frac_t,len_t - 53))||(f1&&f2)) frac_ans ++;
    	frac_ans -= (1ull << BITS_OF_FRAC);
    	exp_t += 1;
    }
	if(exp_t >= (1 << BITS_OF_EXP) - 1) return My_Double_Pinf;
	return number_to_My_Double(sgn_t, exp_t, frac_ans);
}

int is_nan(struct My_Double a){
	int exp,sgn;
	unsigned long long frac;
	My_Double_to_number(a,&sgn,&exp,&frac);
	if(exp==2047&&frac!=0) return 1;
	return 0;
}

struct My_Double Add(struct My_Double a, struct My_Double b)
{
	if(is_nan(a)) return a;
	if(is_nan(b)) return b;
	if(Greater_Than_Zero(a) == Greater_Than_Zero(b)) return Real_Add(a, b);
	else{
		if(Greater_Than_Zero(a)) return Real_Sub(a, b);
		else return Real_Sub(b, a);
	}
}

struct My_Double Sub(struct My_Double a, struct My_Double b)
{
	if(is_nan(a)) return a;
	if(is_nan(b)) return b;
	if(Greater_Than_Zero(a) != Greater_Than_Zero(b)) return Real_Add(a, inverse_My_Double(b));
	else{
		if(Greater_Than_Zero(a)) return Real_Sub(a, inverse_My_Double(b));
		else return inverse_My_Double(Real_Sub(b, inverse_My_Double(a)));
	}
}

unsigned long long Fast_Mul(unsigned long long a,unsigned long long b, int *flag,int *flag2)
{
	*flag2=0;
	__int128 t, one = 1;
	if(a == 0 || b == 0) return 0;
    unsigned long long len_a, len_b, len_t, frac_t;
    for(len_a = 0;(1ull << len_a) <= a;len_a ++);
    for(len_b = 0;(1ull << len_b) <= b;len_b ++);
    t = one * a * b;
    for(len_t = 0;(one << len_t) <= t;len_t ++);
    if(len_t > len_a + len_b - 1) *flag = 1;
    else *flag = 0;
	int i, f1 = 0,f2 = 0,f3= 1 ,j;
    for(i = len_t - 54;i >= 0;i --){
    	if(i == len_t - 54 && isone(t, i)) f1 = 1;
    	else if(i != len_t - 54 && isone(t, i)) f2 = 1, f3 = 0;
    }
    frac_t = t >> (len_t - 53);
	if((f1&&f3&&isone(t,len_t - 53))||(f1&&f2)){
		frac_t ++;
		*flag2 = 1;
	}
    return frac_t;
}

struct My_Double Mul(struct My_Double a, struct My_Double b)
{
	if(is_nan(a)) return a;
	if(is_nan(b)) return b;
	struct My_Double t;
	Clear_My_Double(&t);
	int sgn_t=0, sgn_a, sgn_b;
	int exp_t, exp_a, exp_b;
	int flag = 0,flag2;
	unsigned long long frac_t, frac_a, frac_b;
	if(Greater_Than_Zero(a) == 0) sgn_t ^= 1;
	if(Greater_Than_Zero(b) == 0) sgn_t ^= 1;
    My_Double_to_number(a, &sgn_a, &exp_a, &frac_a);
    My_Double_to_number(b, &sgn_b, &exp_b, &frac_b);
    exp_t = exp_a + exp_b - BIAS;
    if(exp_a == 0) exp_t -=1;
    if(exp_b == 0) exp_t -=1;
    if(exp_a) frac_a += (1ull << BITS_OF_FRAC);
    if(exp_b) frac_b += (1ull << BITS_OF_FRAC);
    frac_t = Fast_Mul(frac_a, frac_b, &flag,&flag2);
    //exp_t += flag;
    printf("%d %d %d %d\n",exp_a,exp_b,exp_t,flag);
    printf("%d\n",exp_t);
    if(exp_t >= 2047){
		exp_t = 2047;
    	frac_t = 0;
    	return number_to_My_Double(sgn_t,exp_t,frac_t);
    }
    if(exp_t < 0){
		if(exp_t < -63){
    		exp_t = 0;
    		frac_t = 0;
    	}
    	else{
    		frac_t -= flag2;
	    	int i,f1 = 0,f2 = 0,f3 = 1, f4 = 0;
	    	for(i=0;i<(-exp_t);i++){
	    		if(isone(frac_t,i)) f3 = 0,f4 = 1;
	    	}
	    	if(isone(frac_t,-exp_t)) f1 = 1;
	    	if(isone(frac_t,-exp_t+1)) f2 = 1;
	    	frac_t >>= (-exp_t+1);
	    	exp_t = 0;
	    	if((f1 && f4) || (f1 && f3 &&f2)) frac_t++;
	    }
    }
    if(frac_t >= (1ull << BITS_OF_FRAC)){
    	if(exp_t == 0) exp_t = 1;
		frac_t -= (1ull << BITS_OF_FRAC);
	}
    return number_to_My_Double(sgn_t, exp_t, frac_t);
}

unsigned long long Fast_Div(unsigned long long a, unsigned long long b, int *flag){
	__int128 t, one = 1,hht;
	unsigned long long len_a, len_b, len_t,frac_t;
    for(len_a = 0;(1ull << len_a) <= a;len_a ++);
    for(len_b = 0;(1ull << len_b) <= b;len_b ++);
	t = a;
	t <<= 60;
	hht=t;
	t /= b;
    for(len_t = 0;(one << len_t) <= t;len_t ++);
    if(len_t < len_a - len_b + 61) *flag = 1;
    else *flag = 0;
	int i, f1 = 0,f2 = 0,f3= 1 ,j;
    for(i = len_t - 54;i >= 0;i --){
    	if(i == len_t - 54 && isone(t, i)) f1 = 1;
    	else if(i != len_t - 54 && isone(t, i)) f2 = 1, f3 = 0;
    }
    frac_t = t >> (len_t - 53);
	if((f1&&f3&&isone(t,len_t - 53))||(f1&&f2)||(f1&&(hht%b!=0))) frac_t ++;
    return frac_t;
}
struct My_Double Div(struct My_Double a, struct My_Double b)
{
	if(is_nan(a)) return a;
	if(is_nan(b)) return b;
	struct My_Double t;
	Clear_My_Double(&t);
	int sgn_t=0, sgn_a, sgn_b;
	int exp_t, exp_a, exp_b;
	int flag = 0,flag2 = 0;
	unsigned long long frac_t, frac_a, frac_b;
	if(Greater_Than_Zero(a) == 0) sgn_t ^= 1;
	if(Greater_Than_Zero(b) == 0) sgn_t ^= 1;
    My_Double_to_number(a, &sgn_a, &exp_a, &frac_a);
    My_Double_to_number(b, &sgn_b, &exp_b, &frac_b);
    exp_t = exp_a - exp_b + BIAS;
    if(exp_a) frac_a += (1ull << BITS_OF_FRAC);
    if(exp_b) frac_b += (1ull << BITS_OF_FRAC);
    frac_t = Fast_Div(frac_a, frac_b, &flag);
    exp_t -= flag;
    if(exp_t >= 2047){
    	exp_t = 2047;
    	frac_t = 0;
    	return t;
    }
	if(exp_t < 0){
		if(exp_t < -63){
    		exp_t = 0;
    		frac_t = 0;
    	}
    	else{
    		frac_t -= flag2;
	    	int i,f1 = 0,f2 = 0,f3 = 1, f4 = 0;
	    	for(i=0;i<(-exp_t);i++){
	    		if(isone(frac_t,i)) f3 = 0,f4 = 1;
	    	}
	    	if(isone(frac_t,-exp_t)) f1 = 1;
	    	if(isone(frac_t,-exp_t+1)) f2 = 1;
	    	frac_t >>= (-exp_t+1);
	    	exp_t = 0;
	    	if((f1 && f4) || (f1 && f3 &&f2)) frac_t++;
	    }
    }
    if(frac_t >= (1ull << BITS_OF_FRAC)) frac_t -= (1ull << BITS_OF_FRAC);
    return number_to_My_Double(sgn_t, exp_t, frac_t);
}

struct My_Double double_to_My_Double(double a)
{
	struct My_Double res;
	memcpy(res.sav, &a, sizeof(double));
}

double My_Double_to_double(struct My_Double a)
{
	double res = 0;
	memcpy(&res, a.sav, sizeof(double));
	return res;
}

int input(struct My_Double* a)
{
	double res=rand()*1.0/rand();
	if(scanf("%lf", &res)!=EOF){
		*a = double_to_My_Double(res);
		return 1;
	}
	else return 0;
}

unsigned long long your_calculate_function(unsigned long long a,unsigned long long b,char op){
	static bool initialized = false;
	if(!initialized){
		Load();
		initialized = true;
	}
	struct My_Double x = ull_to_My_Double(a);
	struct My_Double y = ull_to_My_Double(b);
	struct My_Double ans;
	switch(op){
		case '+':
			ans = Add(x, y);
			break;
		case '-':
			ans = Sub(x, y);
			break;
		case '*':
			ans = Mul(x, y);
			break;
		case '/':
			ans = Div(x, y);
			break;
		default:
			break;
	}
	return My_Double_to_ull(ans);
}
void ejz(double a){
	unsigned long long i=0,w;
	memcpy(&w,&a,sizeof(double));
	for(i=63;i>=0;i--){
		if(isone(w,i)) putchar('1');
		else putchar('0');
		if(i==0) break;
	}
	puts("");
}
int main()
{
	/*inf-inf nan
	  inf+inf inf
	  nan+??? nan
	  
	*/
    Load();
    unsigned long long l=0x8003783ef8a188d5 ,r=0x5ab5b67d3b158409,w;
    double ll,rr,sav;
    memcpy(&ll,&l,sizeof(double));
    memcpy(&rr,&r,sizeof(double));
    w=your_calculate_function(l,r,'*');
    memcpy(&sav,&w,sizeof(double));
    ejz(sav);
    ejz(ll*rr);
    printf("%.1000lf\n",your_calculate_function(l,r,'*'));
    printf("%.1000lf\n",ll*rr);
	struct My_Double a, b, ans;
	double ans2;
	char symbol;
	while(input(&a)){
		scanf("%c",&symbol);
		input(&b);
		switch(symbol){
			case '+':
				ans = Add(a, b);
				ans2 = My_Double_to_double(a) + My_Double_to_double(b);
				break;
			case '-':
				ans = Sub(a, b);
				ans2 = My_Double_to_double(a) - My_Double_to_double(b);
				break;
			case '*':
				ans = Mul(a, b);
				ans2 = My_Double_to_double(a) * My_Double_to_double(b);
				break;
			case '/':
				ans = Div(a, b);
				ans2 = My_Double_to_double(a) / My_Double_to_double(b);
				break;
			default:
				break;
		}
		printf("%.50lf\n", My_Double_to_double(ans));
		printf("%.50lf\n",My_Double_to_double(a)*My_Double_to_double(b));
	}
	return 0;
}
