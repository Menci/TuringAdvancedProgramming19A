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

unsigned long long Fast_Mul(unsigned long long a,unsigned long long b, int *flag,int *flag2,int ws)
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
    int ww = 0;
    if(ws + *flag == 0 && (t>>(len_t - 53)) >= (1ull<<BITS_OF_FRAC)) ww = 1;
	int i, f1 = 0,f2 = 0,f3= 1 ,j;
    for(i = len_t - 54 + ww;i >= 0;i --){
    	if(i == len_t - 54 + ww&& isone(t, i)) f1 = 1;
    	else if(i != len_t - 54 + ww&& isone(t, i)) f2 = 1, f3 = 0;
    }
    frac_t = t >> (len_t - 53 + ww);
	if((f1&&f3&&isone(t,len_t - 53 + ww))||(f1&&f2)){
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
	int exp_t, exp_a, exp_b,zj_a,zj_b;
	int flag = 0,flag2;
	unsigned long long frac_t, frac_a, frac_b;
	if(Greater_Than_Zero(a) == 0) sgn_t ^= 1;
	if(Greater_Than_Zero(b) == 0) sgn_t ^= 1;
    My_Double_to_number(a, &sgn_a, &exp_a, &frac_a);
    My_Double_to_number(b, &sgn_b, &exp_b, &frac_b);
    if(exp_a) frac_a += (1ull << BITS_OF_FRAC);
    else{
    	zj_a = 0;
    	while(exp_a>=-1083&&(frac_a<<zj_a)<(1ull<<BITS_OF_FRAC)) zj_a++,exp_a --;
    	exp_a ++;
    }
    if(exp_b) frac_b += (1ull << BITS_OF_FRAC);
    else{
    	zj_b = 0;
		while(exp_b>=-1083&&(frac_b<<zj_b)<(1ull<<BITS_OF_FRAC)) zj_b++,exp_b --;
		exp_b ++;
	}
    exp_t = exp_a + exp_b - BIAS;
    frac_t = Fast_Mul(frac_a, frac_b, &flag,&flag2, exp_t);
    exp_t += flag;
    if(exp_t >= 2047){
		exp_t = 2047;
    	frac_t = 0;
    	return number_to_My_Double(sgn_t,exp_t,frac_t);
    }
    if(exp_t < 0){
		if(exp_t < -53){
    		exp_t = 0;
    		frac_t = 0;
			return number_to_My_Double(sgn_t,exp_t,frac_t);
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
	    	if((f1 && f4) || (f1 && f3 )) frac_t++;
	    }
    }
    if(frac_t >= (1ull << BITS_OF_FRAC)){
    	if(exp_t == 0) exp_t = 1;
		frac_t -= (1ull << BITS_OF_FRAC);
	}
    return number_to_My_Double(sgn_t, exp_t, frac_t);
}

unsigned long long Fast_Div(unsigned long long a, unsigned long long b, int *flag, int *flag2,int ws){
	__int128 t, one = 1,hht;
	long long len_a, len_b, len_t,frac_t;
    for(len_a = 0;(1ull << len_a) <= a;len_a ++);
    for(len_b = 0;(1ull << len_b) <= b;len_b ++);
	t = a;
	t <<= 60;
	hht=t;
	t /= b;
    for(len_t = 0;(one << len_t) <= t;len_t ++);
    if(len_t < len_a - len_b + 61) *flag = 1;
    else *flag = 0;
    int ww = 0;
    if(ws - *flag == 0 && (t>>(len_t - 53)) >= (1ull<<BITS_OF_FRAC)) ww = 1;
	int i, f1 = 0,f2 = 0,f3= 1 ,j;
    for(i = len_t - 54 + ww;i >= 0;i --){
    	if(i == len_t - 54 + ww&& isone(t, i)) f1 = 1;
    	else if(i != len_t - 54 + ww&& isone(t, i)) f2 = 1, f3 = 0;
    }
    if(len_t - 53 + ww > 0) frac_t = t >> (len_t - 53 + ww);
	if((f1&&f3&&isone(t,len_t - 53 + ww))||(f1&&f2)||(f1&&(hht%b!=0))){
		*flag2 = 1;
		frac_t ++;
	}
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
	int flag = 0,flag2 = 0, zj_a, zj_b;
	unsigned long long frac_t, frac_a, frac_b;
	if(Greater_Than_Zero(a) == 0) sgn_t ^= 1;
	if(Greater_Than_Zero(b) == 0) sgn_t ^= 1;
    My_Double_to_number(a, &sgn_a, &exp_a, &frac_a);
    My_Double_to_number(b, &sgn_b, &exp_b, &frac_b);
    if(exp_a) frac_a += (1ull << BITS_OF_FRAC);
    else{
    	zj_a = 0;
    	while(exp_a >= -1083 && (frac_a << zj_a) < (1ull << BITS_OF_FRAC)) zj_a ++, exp_a --;
    	frac_a <<= zj_a;
    	exp_a ++;
    }
    if(exp_b) frac_b += (1ull << BITS_OF_FRAC);
    else{
    	zj_b = 0;
		while(exp_b >= -1083 && (frac_b << zj_b) < (1ull << BITS_OF_FRAC)) zj_b ++, exp_b --;
		exp_b ++;
	}
    exp_t = exp_a - exp_b + BIAS;
    frac_t = Fast_Div(frac_a, frac_b, &flag, &flag2, exp_t);
    exp_t -= flag;
    if(exp_t >= 2047){
    	exp_t = 2047;
    	frac_t = 0;
		return number_to_My_Double(sgn_t,exp_t,frac_t);
    }
	if(exp_t < 0){
		if(exp_t < -53){
    		exp_t = 0;
    		frac_t = 0;
			return number_to_My_Double(sgn_t,exp_t,frac_t);
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
	    	if((f1 && f4) || (f1 && f3)) frac_t++;
	    }
    }
    if(frac_t >= (1ull << BITS_OF_FRAC)){
    	if(!exp_t) exp_t = 1;
		frac_t -= (1ull << BITS_OF_FRAC);
	}
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

struct My_Double sta[1000000];
char ys[1000000];
char s[1000000];
unsigned long long sjz[1000000];
int yxj[2000];
struct My_Double int_to_My_Double(int x){
	struct My_Double a;
	double res = x;
	memcpy(a.sav, &res, sizeof(double));
	return a;
}
int main()
{
	Load();
	yxj['+'] = 2;
	yxj['-'] = 2;
	yxj['*'] = 1;
	yxj['/'] = 1;
	yxj['('] = 100;
	int j=0;
	while(1){
		fflush(stdout);
		char ch;
		memset(sta,0,sizeof(sta));
		memset(ys,0,sizeof(ys));
		memset(sjz,0,sizeof(sjz));
		memset(s,0,sizeof(s));
		int len = 0, i, pla = 0, top_s = 0, top_y = 0, flag = 0, is_negative, ned = 1;
		struct My_Double sav = int_to_My_Double(1), number = My_Double_Pzero, ans = My_Double_Pzero;
		while((ch = getchar()) != -1){
			if(ch == ' ') continue;
			if(ch == -1) return 0;
			if(ch == '\n') break;
			s[len ++] = ch;
		}
		while(s[pla] != 0){
			if(ned && (s[pla] == '+' || s[pla] == '-' || (s[pla] <= '9' && s[pla] >= '0'))){
				if(s[pla] == '+') pla ++, is_negative = 1;
				else if(s[pla] == '-') pla ++, is_negative = -1;
				else is_negative = 1;
				while(1){
					if((s[pla] < '0'||s[pla] > '9') && s[pla] != '.'){
						if(is_negative == 1) sta[top_s ++] = number;
						else sta[top_s ++] = inverse_My_Double(number);
						number = My_Double_Pzero, flag = 0, sav = int_to_My_Double(1), ned = 0;
						break;
					}
					if(s[pla] == '.') flag = 1;
					else{
						if(flag) number = Add(number, Mul(sav, int_to_My_Double(s[pla] - '0')));
						else number = Add(Mul(number, int_to_My_Double(10)), int_to_My_Double(s[pla] - '0'));
					}
					if(flag) sav = Div(sav, int_to_My_Double(10));
					pla ++;
				}
			}
			else{
				if(s[pla] == '('){
					ys[top_y ++] = '(';
					pla ++;
					continue;
				}
				if(s[pla] == ')'){
					while(ys[top_y - 1] != '('){
						if(ys[top_y - 1] == '*') sta[top_s - 2] = Mul(sta[top_s - 2], sta[top_s - 1]);
						else if(ys[top_y - 1] == '/') sta[top_s - 2] = Div(sta[top_s - 2], sta[top_s - 1]);
						else if(ys[top_y - 1] == '-') sta[top_s - 2] = Sub(sta[top_s - 2], sta[top_s - 1]);
						else if(ys[top_y - 1] == '+') sta[top_s - 2] = Add(sta[top_s - 2], sta[top_s - 1]);
						top_s --;
						top_y --;
					}
					top_y --;
					pla ++;
					continue;
				}
				ned = 1;
				while(top_y && yxj[s[pla]] >= yxj[ys[top_y - 1]]){
					if(ys[top_y - 1] == '*') sta[top_s - 2] = Mul(sta[top_s - 2], sta[top_s - 1]);
					else if(ys[top_y - 1] == '/') sta[top_s - 2] = Div(sta[top_s - 2], sta[top_s - 1]);
					else if(ys[top_y - 1] == '-') sta[top_s - 2] = Sub(sta[top_s - 2], sta[top_s - 1]);
					else if(ys[top_y - 1] == '+') sta[top_s - 2] = Add(sta[top_s - 2], sta[top_s - 1]);
					top_s --;
					top_y --;
				}
				ys[top_y ++] = s[pla];
				pla++;
			}
		}
		while(top_y){
			if(ys[top_y - 1] == '*') sta[top_s - 2] = Mul(sta[top_s - 2], sta[top_s - 1]);
			else if(ys[top_y - 1] == '/') sta[top_s - 2] = Div(sta[top_s - 2], sta[top_s - 1]);
			else if(ys[top_y - 1] == '-') sta[top_s - 2] = Sub(sta[top_s - 2], sta[top_s - 1]);
			else if(ys[top_y - 1] == '+') sta[top_s - 2] = Add(sta[top_s - 2], sta[top_s - 1]);
			top_s --;
			top_y --;
		}
		int sgn,exp;
		unsigned long long frac;
		if(is_nan(sta[0])) puts("nan");
		else if(My_Double_to_double(sta[0]) == My_Double_to_double(My_Double_Pinf)) puts("inf");
		else if(My_Double_to_double(sta[0]) == My_Double_to_double(My_Double_Ninf)) puts("-inf");
		else{
			My_Double_to_number(sta[0], &sgn, &exp, &frac);
			if(sgn == 1) putchar('-');
			if(exp) frac += 1ll << BITS_OF_FRAC, exp--;
			exp -= 1022;
			exp -= 52;
			sjz[2000] = frac;
			for(i = 2000;i <= 3000;i ++){
				sjz[i + 1] = sjz[i] / 10;
				sjz[i] %= 10;
			}
			while(exp < 0){
				exp ++;
				for(i = 300;i <= 4000;i ++){
					if(sjz[i] % 2) sjz[i - 1] += 5;
					sjz[i] /= 2;
				}
			}
			while(exp > 0){
				exp --;
				for(i = 0;i<= 4000;i ++) sjz[i] *= 2;
				for(i = 0;i<= 4000;i ++){
					sjz[i + 1] += sjz[i] / 10;
					sjz[i] %= 10;
				}
			}
		}
		int flat = 0;
		for(i = 4000;i >= 1980;i --){
			if(i == 2000) flat = 1;
			if(sjz[i]) flat = 1;
			if(flat) putchar('0' + sjz[i]);
			if(i == 2000) putchar('.');
		}
		puts(""); 
	}
	return 0;
}
