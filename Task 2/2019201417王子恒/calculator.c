#include "double.c"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>  // uint64_t
#include <stdbool.h> // bool

#define N 50000005

typedef long long ll;
ll qpow(ll k, int n) {
	ll ans = 1;
	while(n) {
		if(n & 1) ans *= k;
		k *= k; n >>= 1;
	}
	return ans;
}
typedef struct {
	int a[35005], len, bina;
} BIGNUM;
BIGNUM init() {
	BIGNUM a;
	memset(a.a, 0, sizeof(a.a));
	a.len = 0; a.bina = 10;
	return a;
}
BIGNUM change_to_bignum(__int128 x, int bina) {
	BIGNUM a = init(); a.bina = bina;
	if(!x) a.a[a.len++] = 0;
	while(x) {
		a.a[a.len++] = x % bina;
		x /= bina;
	}
	return a;
}
BIGNUM add_bignum(BIGNUM a, BIGNUM b) {
	BIGNUM c = init(); c.len = max(a.len, b.len);
	int i;
	for(i = 0; i < c.len; ++i) {
		c.a[i] += a.a[i] + b.a[i];
		if(c.a[i] >= 10) {
			c.a[i] -= 10;
			++c.a[i + 1];
			if(c.len == i + 1) ++c.len; 
		}
	}
	return c;
}
BIGNUM mul_bignum(BIGNUM a, BIGNUM b) {
	BIGNUM c = init();
	int i, j;
	for(i = 0; i < a.len; ++i) {
		for(j = 0; j < b.len; ++j) {
			c.a[i + j] += a.a[i] * b.a[j];
		}
	}
	c.len = a.len + b.len - 1;
	for(i = 0; i < c.len; ++i) {
		if(c.a[i] >= 10){
			c.a[i + 1] += c.a[i] / 10;
			c.a[i] %= 10;
			if(i + 1 == c.len) ++c.len;
		}
	}
	while(c.len > 1 && !c.a[c.len - 1]) --c.len;
	return c;
}
BIGNUM change_bina10_to_bina2(BIGNUM a) {
	BIGNUM b = init();
	int i;
	while(a.len > 0) {
		int tmp = 0, flag = 0;
		for(i = a.len - 1; i >= 0; --i) {
			tmp = tmp * 10 + a.a[i];
			if(tmp >> 1) flag = 1;
			else if(!flag) --a.len;
			a.a[i] = tmp >> 1;
			tmp &= 1;
		}
		b.a[b.len++] = tmp;
	}
	return b;
}
DOUBLE scanf_double(char s[], int l, int r) {
	int i, j, dec = 0;
	BIGNUM a = init();
	for(i = l; i <= r; ++i) {
		if(!isdigit(s[i])) break;
		a.a[a.len++] = s[i] - '0';
	}
	l = i + 1;
	for(i = l;i <= r; ++i) {
		a.a[a.len++] = s[i] - '0';
		++dec;
	}
	for(i = 0, j = a.len - 1; i < j; ++i, --j) {
		int t = a.a[i]; 
		a.a[i] = a.a[j]; 
		a.a[j] = t;
	}
	a = change_bina10_to_bina2(a);
	int e = 52 + max(0, a.len - 126);
	__int128 m = 0;
	for(i = a.len - 1; i >= 0 && a.len - i <= 126; --i) {
		m = (m << 1) | a.a[i];
	}
	DOUBLE res = double_round(0, m, e);
	DOUBLE div = double_round(0, (1LL << 50) + (1LL << 52), 3);
	while(dec--) {
		res = divide(res, div);
	}
	return res;
}
void print_double(DOUBLE x) {
	uint64_t s = get_s(x);
	if(is_special_inf(x)) {
		puts(s ? "-inf" : "inf");
		return;
	}
	if(is_special_nan(x)) {
		puts("nan");
		return;
	}
	__int128 m = get_m(x);
	int e = get_e(x);
	int i, j;
	BIGNUM a = change_to_bignum(m, 10);
	e -= MAX_FRAC;
	for(i = 1; i <= e; ++i) a = mul_bignum(a, change_to_bignum(2, 10));
	for(i = e; i < 0; ++i)a = mul_bignum(a, change_to_bignum(5, 10));
	int dec = -e;
	if(s) putchar('-');
	if(a.len <= dec) putchar('0');
	if(a.len < dec) putchar('.'); 
	for(i = 1;i <= dec-a.len; ++i) putchar('0');
	for(i = a.len - 1; i >= 0; --i) {
		if(i + 1 == dec) putchar('.');
		putchar(a.a[i] + '0');
	}
	puts("");
	fflush(stdout);
	return;
}

char priority[256];
char op_stk[N];
DOUBLE su_stk[N];
int op_stk_p, su_stk_p;
int get_end(char s[], int l, int len) {
	int r;
	for(r = l; r < len; ++r) {
		if(!isdigit(s[r]) && s[r] != '.') break;
	}
	return r - 1;
}
void su_stk_add(int op, DOUBLE x, char c) {
	if(op) {
		su_stk[++su_stk_p] = x; return;
	}
	uint64_t a = convert_from_double_type(su_stk[su_stk_p - 1]);
	uint64_t b = convert_from_double_type(su_stk[su_stk_p]);
	uint64_t ans = calculate_function(a, b, c);
	su_stk[--su_stk_p] = convert_to_double_type(ans);
	return;
}
void op_stk_add(char op) {
	if(op == '(') {
		op_stk[++op_stk_p] = op; return;
	}
	while(op_stk_p != -1 && priority[op_stk[op_stk_p]] >= priority[op]) {
		if(!priority[op_stk[op_stk_p]])break;
		su_stk_add(0, ZERO, op_stk[op_stk_p]);
		--op_stk_p;
	}
	if(op==')') --op_stk_p;
	else op_stk[++op_stk_p] = op;
	return;
}
void expression_initialization() {
	priority['('] = 0; priority[')'] = 1;
	priority['+'] = 2; priority['-'] = 2;
	priority['*'] = 3; priority['/'] = 3;
	op_stk_p = -1, su_stk_p = -1;
	return;
}
DOUBLE expression_calculate(char s[], int len) {
	int i, flag = 0;
	char pre = ')';//pre == 1表示为前者是数 
	for(i = 0; i < len; ++i) {
		if(s[i] == ' ') continue;
		if(!isdigit(s[i])) {
			if(s[i] == '+' || s[i] == '-') {
				int j, op = 0;
				for(j = i; j < len; ++j) {
					if(s[j] == ' ') continue;
					if(s[j] == '+')	continue;
					else if(s[j] == '-') op ^= 1;
					else break;
				}
				if(pre != 1 && pre != ')') {
					flag = 1; op_stk_add(pre = '(');
					su_stk_add(1, ZERO, 0); pre = 1;
				}
				op_stk_add(pre = op ? '-' : '+');
				i = j - 1;				
			} else op_stk_add(pre = s[i]);
		} else {
			int j = get_end(s, i, len);
			su_stk_add(1, scanf_double(s, i, j), 0); pre = 1;
			i = j;
			if(flag) {flag = 0; op_stk_add(')'); pre = ')';}
		}
	}
	while(op_stk_p != -1) {
		su_stk_add(0, ZERO, op_stk[op_stk_p--]);
	}
	return su_stk[0];
}
DOUBLE expression_calculate_function(char s[], int len) {
	expression_initialization();
	return expression_calculate(s, len);
}
char s[N];
int main() {
	while(gets(s) != NULL){
		int len = strlen(s);
		print_double(expression_calculate_function(s, len));
	}
	return 0;
}
