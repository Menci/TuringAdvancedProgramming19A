#include <stdio.h>
#include <string.h>
#include "my_double.h"
#define mxn 100005
#define PREC_FIX 30
#define MAX_NUM_LENTH 350
#define NEG_FIX 100
#define MAX_EXP 1024
db num[mxn];
int cntNum, cnt;
char buff[mxn * 10];
void readLine(char *s) {
	while(((*s) = getchar()) && (*s) != EOF && (*s) != '\n' && (*s) != '\r') ++s;
}
typedef struct MyInt{
	uint8_t s[MAX_NUM_LENTH + 5], len;
}bi;
bi bi_zero, pw2[MAX_EXP + PREC_FIX + NEG_FIX + 5];
#define PW2(x) (pw2[(x) + NEG_FIX])
bi Add_bi(bi a, bi b) {
	bi c;
	int d = 0;
	for(int i = 1; i <= a.len || i <= b.len || d; ++i) {
		c.s[i] = 0;
		if(i <= a.len) c.s[i] += a.s[i];
		if(i <= b.len) c.s[i] += b.s[i];
		if(d) c.s[i] += d, d = 0;
		d = c.s[i] / 10, c.s[i] %= 10;
		c.len = i;
	}
	while(c.len > 1 && (!c.s[c.len])) --c.len;
	return c;
}
bi Div_bi(bi a, int b) {
	bi c;
	int d = 0;
	for(int i = a.len; i >= 1; --i) {
		c.s[i] = a.s[i] + d;
		d = (c.s[i] % b) * 10, c.s[i] /= b; 
	}
	c.len = a.len;
	while(c.len > 1 && (!c.s[c.len])) --c.len;
	return c;
}
db E =   {0, 0, 0, 0, 0, 0, 0xf0, 0x3f};
db ten = {0, 0, 0, 0, 0, 0, 0x24, 0x40};
db pw10[310], Num[10];
void Init() {
	pw10[0] = (db){0, 0, 0, 0, 0, 0, 0xf0, 0x3f};
	pw10[1] = (db){0, 0, 0, 0, 0, 0, 0x24, 0x40};
	Num[0] = ZERO;
	for(int i = 1; i <= 9; ++i) Num[i] = Add(Num[i - 1], E);
	for(int i = 2; i <= 308; ++i) pw10[i] = Mul(pw10[i - 1], pw10[1]);
	PW2(0).len = PREC_FIX + 1;
	PW2(0).s[PREC_FIX + 1] = 1;
	for(int i = 1; i <= BIAS + 1; ++i) PW2(i) = Add_bi(PW2(i - 1) , PW2(i - 1));
	for(int i = -1; i >= -NEG_FIX; --i) PW2(i) = Div_bi(PW2(i + 1), 2);
}

db readFP(char *s, int l, int r, int sgn) {
	db x = ZERO;
	int posd = -1, posn = -1;
	for(int i = l; i < r; ++i) {
		if(s[i] == '.' && posd < 0) posd = i;
		if(s[i] >= '1' && s[i] <= '9' && posn < 0) posn = i;
	}
	if(posd == -1) {
		for(int i = r - 1; i >= l; --i) 
			if(s[i] - '0') x = Add(x, Mul(Num[s[i] - '0'], pw10[r - i - 1]));
	}
	else {
		int loc = (r - 1) < (posn + 20) ? (r - 1) : (posn + 20);
		for(int i = loc; i >= posd + 1; --i)
			if(s[i] - '0') x = Add(x, Div(Num[s[i] - '0'], pw10[i - posd]));
		for(int i = posd - 1; i >= l; --i) 
			if(s[i] - '0') x = Add(x, Mul(Num[s[i] - '0'], pw10[posd - i - 1]));
	}
	if(sgn) SET(x.s, W - 1);
	return x;
}

void printFP(db x) {
	int typ = getType(x);
	if(VAL(x.s, W - 1)) putchar('-');
	if(typ) {
		if(typ == 1 || typ == -1) puts("inf");
		else puts("nan");
		return;
	}
	int exp = 0;
	for(int i = W_FRAC; i < W - 1; ++i)
		if(VAL(x.s, i)) exp += 1 << (i - W_FRAC);
	if(exp == 0) {
		puts("0");
		return;
	}
	exp -= BIAS;
	bi res;
	res = bi_zero;
	for(int i = 0; i <= W_FRAC; ++i) 
		if((VAL(x.s, i) || i == W_FRAC) && i - W_FRAC + exp >= - PREC_FIX)
			res = Add_bi(res, PW2(i - W_FRAC + exp)); 
	for(int i = res.len; i > PREC_FIX; --i) putchar(res.s[i] + '0');
	if(res.len <= PREC_FIX) putchar('0');
	putchar('.');
	for(int i = PREC_FIX; i >= 1; --i) {
		if(i <= res.len) putchar(res.s[i] + '0');
		else putchar('0');
	}
	puts("");
}

void procExpr(char *s) {
	cntNum = cnt = 0;
	static int seq[mxn], stack1[mxn], stack2[mxn];
	int len = strlen(s), tot = 0, p1 = 0, p2 = 0, pre = 0, sgn = 0;
	for(int i = 0; i < len; ++i) if(s[i] >= 0) {
		if(s[i] == ' ' || s[i] == '\t' || s[i] == EOF || s[i] == '\n' || s[i] == '\r') continue;
		if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '(' || s[i] ==')') {
			if(s[i] == '+') {
				if(pre == 0) pre = 1;
				else if(pre == 2) pre = 1, seq[++cnt] = -s[i];
			}
			else if(s[i] == '-') {
				if(pre == 0) sgn ^= 1, pre = 1;
				else if(pre == 1) sgn ^= 1;
				else if(pre == 2) seq[++cnt] = -s[i], pre = 1;
			}
			else if(s[i] == '(' || s[i] == ')') {
				if(s[i] == '(') pre = 0;
				else pre = 2;
				seq[++cnt] = -s[i];
			}
			else seq[++cnt] = -s[i], sgn = 0, pre = 1;
		}
		else {
			int ed = i;
			while((s[ed] >= '0' && s[ed] <= '9') || s[ed] == '.') ++ed;
			num[++cntNum] = readFP(s, i, ed, sgn);
			i = ed - 1;
			seq[++cnt] = cntNum;
			pre = 2, sgn = 0;
		}
	}
	for(int i = 1; i <= cnt; ++i) {
		if(seq[i] > 0) stack2[++p2] = seq[i]; // Number		
		else { // Operator
			if(-seq[i] == ')') {
				while(p1 && -stack1[p1] != '(') {
					stack2[++p2] = stack1[p1--];
					if((-stack2[p2] == '(') || (-stack2[p2] == ')')) --p2;
				}
				--p1;
			}
			else if(-seq[i] == '(') stack1[++p1] = seq[i];
			else if((!p1) || (-stack1[p1] == '(')) stack1[++p1] = seq[i];
			else {
				while(p1) {
					if(-stack1[p1] == '(') break;
					if((-seq[i] == '*' || -seq[i] == '/') && (-stack1[p1] == '+' || -stack1[p1] == '-')) break;
					stack2[++p2] = stack1[p1--];
					if((-stack2[p2] == '(') || (-stack2[p2] == ')')) --p2;
				}
				stack1[++p1] = seq[i];
			}
		}
	}
	while(p1) {
		stack2[++p2] = stack1[p1--];
		if((-stack2[p2] == '(') || (-stack2[p2] == ')')) --p2;
	}
	db a,b;
	for(int i = 1; i <= p2; ++i) {
		if(stack2[i] > 0) stack1[++p1] = stack2[i];
		else {
			int t1 = stack1[p1--], t2 = stack1[p1--];
			a = num[t2], b = num[t1];
			num[++cntNum] = calculate(a, b, -stack2[i]);
			stack1[++p1] = cntNum;
		}
	}
	db nm = num[stack1[1]];
	printFP(nm);
	fflush(stdout);
}

int main(){
	Init();
	while(1) {
		readLine(buff);
		if(strlen(buff) == 0 || (strlen(buff) == 1 && buff[0] == EOF)) break;
		procExpr(buff);
		if(strlen(buff) == 0 || buff[strlen(buff) - 1] == EOF) break;
		int len = strlen(buff);
		for(int i = 0; i < len; ++i) buff[i]= 0;
	}
}