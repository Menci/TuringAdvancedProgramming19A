#include <stdio.h>
#include <string.h>
#include <stdint.h>
typedef struct db db;
const int W = 64;
const int BYTE_SIZE = 8;
const int W_EXP = 11;
const int W_FRAC = 52;
#define BIT(x) (1ll << (x))
#define VAL(x, y)    (((y) < 0 && (y) > W * 2) ? 0 : (((x)[(y) >> 3] >> (y & 7)) & 1))
#define SET(x, y)    (((y) < 0 && (y) > W * 2) ? 0 : (((x)[(y) >> 3]) |=  (1 << ((y) & 7))))
#define CLEAR(x, y)  (((y) < 0 && (y) > W * 2) ? 0 : (((x)[(y) >> 3]) &= ~(1 << ((y) & 7))))
#define TOGGLE(x, y) (((y) < 0 && (y) > W * 2) ? 0 : (((x)[(y) >> 3]) ^=  (1 << ((y) & 7))))
#define ROUND(fl) \
		if(st >= 0 && VAL(s, st)) { \
			if(st - 1 >= 0 && VAL(s, st - 1)) flg += addition(s, st, ed + 1);\
		}\
		else {\
			if(st - 1 >= 0 && VAL(s, st - 1)) {\
				uint8_t ok = 0;\
				for(int i = st - 2; i >= 0; --i) if(VAL(s, i)) {\
					ok = 1;\
					break;\
				}\
				if((ok || (fl)) && st >= 0) flg += addition(s, st, ed + 1);\
			}\
		}


const int64_t BIAS = (1 << (11 - 1)) - 1;
struct db{
	uint8_t s[8];
};
const db ZERO   = {   0,   0,   0,   0,   0,   0,    0,    0};
const db ZERO_N = {   0,   0,   0,   0,   0,   0,    0, 0x80};
const db INF_P  = {   0,   0,   0,   0,   0,   0, 0xf0, 0x7f};
const db INF_N  = {   0,   0,   0,   0,   0,   0, 0xf0, 0xff};
const db NAN_P  = {   0,   0,   0,   0,   0,   0, 0xf8, 0x7f};
const db NAN_N  = {   0,   0,   0,   0,   0,   0, 0xf8, 0xff};

int cmple(const db a, const db b) {
	if(VAL(a.s, W - 1) == VAL(b.s, W - 1)) {
		for(int i = W - 2; i >= 0; --i) {
			if(VAL(a.s, i) != VAL(b.s, i)) {
				if(VAL(a.s, W - 1)) 
					return VAL(a.s, i) > VAL(b.s, i);
				else 
					return VAL(a.s, i) < VAL(b.s, i);
			}
		}
		return 0;
	}
	else {
		return VAL(a.s, W - 1) > VAL(b.s, W - 1);
	}
}
int cmpq(const db a, const db b) {
	for(int i = 0; i < BYTE_SIZE; ++i) 
		if(a.s[i] != b.s[i]) return 0;
	return 1;
}
int getType(const db x) {
	for(int i = W - 2; i >= W_FRAC; --i) {
		if(! VAL(x.s, i)) return 0;
	}
	if(cmpq(x, ZERO)) return 0;
	if(cmpq(x, INF_P)) return 1;
	if(cmpq(x, INF_N)) return -1;
	if(VAL(x.s, W - 1)) return -2;
	else return 2;
}
db getNeg(const db x) {
	db y = x;
	TOGGLE(y.s, W - 1);
	return y;
}

uint8_t addition(uint8_t *s, const int x, const int y) {
	for(int i = x; i < y; ++i) {
		TOGGLE(s, i);
		if(VAL(s, i)) break;
		if(i == y - 1) return 1;
	}
	return 0;
}
uint8_t isNorm(const char *s) {
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL(s, i)) return 1;
	}
	return 0;
}

db add(db a, db b) {
	int8_t ta = getType(a), tb = getType(b);
	if(ta == 2 && tb == -2) return NAN_P;
	if(ta == -2 && tb == 2) return NAN_N;
	if(ta == 2 || tb == 2) return NAN_P;
	if(ta == -2 || tb == -2) return NAN_N;
	if(ta == 1 && tb == -1 || ta == -1 && tb == 1) return NAN_N;
	if(ta == 1 && tb == 1) return INF_P;
	if(ta == -1 && tb == -1) return INF_N;
	int expa = 0, expb = 0, dexp;
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL(a.s, i)) expa += BIT(i - W_FRAC);
		if(VAL(b.s, i)) expb += BIT(i - W_FRAC);
	}
	if(expa < expb) {
		db tmp = a;
		a = b, b = tmp;
		expa ^= expb, expb ^= expa, expa ^= expb;
	}
	dexp = expa - expb;
	if(expa && (! expb)) --dexp;
	int inc = 0;
	uint8_t s[3 * BYTE_SIZE], flg = 0;
	memset(s, 0, sizeof(s));
	for(int i = 0; i <= W_FRAC; ++i) {
		if((i < W_FRAC && VAL(a.s, i)) || (i == W_FRAC && expa)) 
			addition(s, i + W_FRAC, 2 * W_FRAC + 2);
		if((i < W_FRAC && VAL(b.s, i)) || (i == W_FRAC && expb)) {
			if(i - dexp + W_FRAC >= 0) addition(s, i - dexp + W_FRAC, 2 * W_FRAC + 2);
			else flg = 1;
		}
	}
	db c;
	memset(c.s, 0, sizeof(c.s));
	uint8_t sgn = (VAL(a.s, W - 1) && VAL(b.s, W - 1));
	if(sgn) SET(c.s, W - 1);
	uint8_t flag = 0;
	if(dexp > 0 && dexp <= W_FRAC && (!VAL(s, W_FRAC + 1 + W_FRAC))) {
		if(dexp == 1) {
			if(VAL(s, 0 + W_FRAC) && VAL(b.s, 0)) flag = 1;
		}
		else {
			if(VAL(s, 0 + W_FRAC)) {
				if(VAL(b.s, dexp - 1)) flag = 1;
			}
			else {
				if(dexp - 1 >= 0 && VAL(b.s, dexp - 1)) {
					for(int i = dexp - 2; i >= 0; --i) if(VAL(b.s, i)) {
						flag = 1;
						break;
					}
				}
			}
		}
		if(flag) addition(s, 0, W_FRAC + 2 + W_FRAC);
	}
	if(VAL(s, W_FRAC + 1 + W_FRAC)) {
		if(expa + 1 >= BIT(W_EXP)) {
			if(sgn) return INF_N;
			else return INF_P;
		}
		int exp = expa + 1;
		int st = 1 + W_FRAC, ed = W_FRAC + W_FRAC;
		uint8_t flg = 0;
		ROUND(0);
		for(int i = 0; i < W_EXP; ++i) if(exp & BIT(i)) SET(c.s, W_FRAC + i);
		for(int i = st; i <= ed; ++i) if(VAL(s, i)) SET(c.s, i - st);
		if(exp + 1 >= BIT(W_EXP)) {
			if(sgn) return INF_N;
			else return INF_P;
		}
		return c;
	}
	else {
		if(expa + 1 >= BIT(W_EXP)) {
			if(sgn) return INF_N;
			else return INF_P;
		}
		int st = 0 + W_FRAC, ed = W_FRAC - 1 + W_FRAC;
		uint8_t flg = 0;
		ROUND(0);
		if((!expa) && (!expb) && VAL(s, W_FRAC + W_FRAC)) ++expa;
		if(expa + 1 >= BIT(W_EXP)) {
			if(sgn) return INF_N;
			else return INF_P;
		}
		for(int i = 0; i < W_EXP; ++i) if(expa & BIT(i)) SET(c.s, W_FRAC + i);
		for(int i = 0; i < W_FRAC; ++i) if(VAL(s, i + W_FRAC)) SET(c.s, i);
		return c;
	}
}

db sub(db a, db b) {
	int8_t ta = getType(a), tb = getType(b);
	if(ta == 2 && tb == -2) return NAN_P;
	if(ta == -2 && tb == 2) return NAN_N;
	if(ta == 2 || tb == 2) return NAN_P;
	if(ta == -2 || tb == -2) return NAN_N;
	if(ta == 1 && tb == 1 || ta == -1 && tb == -1) return NAN_N;
	if(ta == 1 || tb == -1) return INF_P;
	if(ta == -1 || tb == 1) return INF_N; 
	if(cmpq(b, ZERO) || cmpq(b, ZERO_N)) return a;
	if(cmpq(a, ZERO) || cmpq(a, ZERO_N)) return getNeg(b);
	uint8_t sgn = cmple(a, b);
	long long expa = 0, expb = 0;
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL(a.s, i)) expa += BIT(i - W_FRAC);
		if(VAL(b.s, i)) expb += BIT(i - W_FRAC);
	}
	if(expa <= expb) {
		uint8_t sw = 0;
		for(int i = W_FRAC - 1; i >= 0; --i) if(VAL(a.s, i) != VAL(b.s, i)) {
			if(VAL(a.s, i) < VAL(b.s, i)) sw = 1;
			break;
		}
		if(sw || expa < expb) {
			expa ^= expb, expb ^= expa, expa ^= expb;
			db c = a;
			a = b, b = c;
		}
	}
	db c;
	memset(c.s, 0, sizeof(c.s));
	if(sgn) SET(c.s, W - 1);
	__int128_t x = 0, y = 0;
	int dexp = expa - expb, top = 120;
	if(expa) SET((uint8_t *)(&x), top);
	if(expb && top + 1 - dexp > 0) SET((uint8_t *)(&y), top - dexp);
	if(expa && (! expb)) --dexp;
	if(dexp > W_FRAC + 10) {
		c = a;
		if(sgn) SET(c.s, W - 1);
		else CLEAR(c.s, W - 1);
		return c;
	}
	for(int i = 0; i < W_FRAC; ++i) {
		if(VAL(a.s, i)) SET((uint8_t *)(&x), top - (W_FRAC - i));
		if(VAL(b.s, i) && top - (W_FRAC - i) - dexp >= 0) SET((uint8_t *)(&y), top - (W_FRAC - i) - dexp); 
	}
	x -= y;
	if(x == 0) return ZERO;
	uint8_t *s = (uint8_t *) &x;
	int st, ed;
	if(!expa) ed = top - 1, st = ed - W_FRAC + 1;
	else {
		int pos = -1;
		for(int i = top; i >= 0; --i) if(VAL(s, i)) {
			pos = i;
			break;
		}
		if(top - pos >= expa) {
			ed = top - expa, st = ed - W_FRAC + 1;
			expa = 0;
		}
		else {
			if(pos != top) {
				ed = pos - 1, st = ed - W_FRAC + 1;
				expa -= top - ed - 1; 
			}
			else ed = pos - 1, st = ed - W_FRAC + 1;
		}
	}
	uint8_t flg = 0;
	ROUND(0);
	if(! flg) {
		for(int i = st; i <= ed; ++i) if(i >= 0 && VAL(s, i)) {
			SET(c.s, i - st);
		}
		for(int i = W_FRAC; i < W - 1; ++i) {
			if(expa & BIT(i - W_FRAC)) SET(c.s, i);
		}
		return c;
	}
	else {
		++expa;
		if(expa + 1 >= BIT(W_FRAC)) return sgn ? INF_N : INF_P;
		for(int i = W_FRAC; i < W - 1; ++i) {
			if(expa & BIT(i - W_FRAC)) SET(c.s, i);
		}
		return c;
	}
}

db mul(const db a, const db b) {
	// Sign Bit
	uint8_t sgn = 0;
	if(VAL(a.s, W - 1) != VAL(b.s, W - 1)) sgn = 1;
	//Special Values

	int8_t ta = getType(a), tb = getType(b);
	if(ta || tb) {
		if(ta == tb) {
			if(ta == 1 || ta == -1) return INF_P;
			else return NAN_P;
		}
		if(ta > tb) ta ^= tb, tb ^= ta, ta ^= tb;
		if(ta == -2 && tb == -1) return NAN_P;
		if(ta == -2 && tb ==  1) return NAN_N;
		if(ta == -2 && tb ==  2) return NAN_N;
		if(ta == -1 && tb ==  1) return INF_N;
		if(ta == -1 && tb ==  2) return NAN_N;
		if(ta ==  1 && tb ==  2) return INF_P;
	}
	if(ta == -2 || tb == -2) return NAN_N;
	if(ta == 2 || tb == 2) return NAN_P;
	if(cmpq(a, ZERO) || cmpq(b, ZERO)) {
		return sgn ? ZERO_N : ZERO;
	}
	uint8_t norma = isNorm(a.s), normb = isNorm(b.s), norm = (norma && normb);
	db c;  //Initialize
	memset(c.s, 0, sizeof(c.s));
	if(sgn) SET(c.s, W - 1);
	long long exp = 0;
	__int128_t x = 0, y = 0;
	for(int i = 0; i < W_FRAC; ++i) {
		if(VAL(a.s, i)) x |= BIT(i);
		if(VAL(b.s, i)) y |= BIT(i);
	}
	if(norma) x |= BIT(W_FRAC);
	if(normb) y |= BIT(W_FRAC);
	uint8_t *s = (uint8_t *) &x;
	x *= y;

	if(norma + normb == 1) ++exp;
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL(a.s, i)) exp += BIT(i - W_FRAC);
		if(VAL(b.s, i)) exp += BIT(i - W_FRAC);
	}
	exp -= BIAS;
	if(exp < -W) return sgn ? ZERO_N : ZERO;
	int st, ed;

	//First Round
	uint8_t flag = 0;
	if(VAL(s, 2 * W_FRAC + 1)) { // 1x.xxxxx
		if(exp >= 0) ++exp, st = W_FRAC + 1, ed = 2 * W_FRAC;
		else st = W_FRAC - exp + 1, ed = 2 * W_FRAC - exp;
	}
	else if(VAL(s, 2 * W_FRAC)) {// 01.xxxxx
		if(exp > 0)  st = W_FRAC, ed = 2 * W_FRAC - 1;
		else if(exp == 0) st = W_FRAC + 1, ed = 2 * W_FRAC;
		else {
			ed = 2 * W_FRAC - exp, st = ed - W_FRAC + 1;
			++exp;
		}
	}
	else { // 00.xxxxx
		if(exp <= 0) st = W_FRAC - exp + 1, ed = 2 * W_FRAC - exp;
		else if(exp == 1) st = W_FRAC, ed = 2 * W_FRAC - 1, exp = 0;
		else {
			uint8_t ok = 0;
			int pos = -1;
			for(int i = 2 * W_FRAC - 1; i >= 0; --i) if(VAL(s, i)) {
				ok = 1, pos = i;
				break;
			}
			if(ok) {
				if(2 * W_FRAC - pos <= exp) {
					if(2 * W_FRAC - pos == exp) ed = pos, st = ed - W_FRAC + 1, exp = 0;
					else {
						exp -= 2 * W_FRAC - pos;
						ed = pos - 1, st = ed - W_FRAC + 1;
						if((!exp) && VAL(s, ed + 1)) ++exp;
						if(st < 0) st = 0;
						if(ed < 0) ed = 0;
					}
				}
				else {
					st = W_FRAC - (exp - 1), ed = 2 * W_FRAC - (exp - 1), exp = 0;
				}
			}
			else st = W_FRAC - exp, ed = 2 * W_FRAC - 1 - exp, exp = 0;
		}
	}

	uint8_t flg = 0;
	ROUND(0);

	if(!flg) {
		if(exp >= BIT(W_EXP) - 1) return sgn ? INF_N : INF_P;
		if(exp >= 0) {
			for(int i = W_FRAC; i < W - 1; ++i) {
				if(exp & BIT(i - W_FRAC)) SET(c.s, i); 
			}
			for(int i = st; i <= ed; ++i) 
				if(VAL(s, i)) SET(c.s, i - st);
		}
		else {
			for(int i = st; i <= ed; ++i) {
				if(i >= 0 && i <= 2 * W_FRAC + 2 && VAL(s, i)) SET(c.s, i - st);
			}
		}
		return c;
	}
	else {
		if(exp >= 0) ++exp;
		else exp = 0;
		if(exp >= BIT(W_EXP) - 1) return sgn ? INF_N : INF_P;
		for(int i = W_FRAC; i < W - 1; ++i) {
			if(exp & BIT(i - W_FRAC)) SET(c.s, i); 
		}
		return c;
	}
	return c;
}

db DIVISION(db a, db b) {
	int ta = getType(a), tb = getType(b);
	if(ta == 2) return NAN_P;
	if(ta == -2) return NAN_N;
	if(tb == 2) return NAN_P;
	if(tb == -2) return NAN_N;
	if(ta && tb) return INF_N;
	if(ta == 1) return VAL(b.s, W - 1) ? INF_N : INF_P;
	if(ta == -1) return VAL(b.s, W - 1) ? INF_P : INF_N;
	if(tb == 1) return VAL(a.s, W - 1) ? ZERO_N : ZERO;
	if(tb == -1) return VAL(a.s, W - 1) ? ZERO : ZERO_N;
	long long expa = 0, expb = 0, exp;
	int sgn = VAL(a.s, W - 1) ^ VAL(b.s, W - 1);
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL(a.s, i)) expa |= BIT(i - W_FRAC);
		if(VAL(b.s, i)) expb |= BIT(i - W_FRAC);
	}
	exp = expa - expb + BIAS;
	__int128_t x = 0, y = 0, z;
	uint8_t *s = (uint8_t *) &x;
	uint8_t *t = (uint8_t *) &y;
	for(int i = 0; i < W_FRAC; ++i) {
		if(VAL(a.s, i)) x |= BIT(i);
		if(VAL(b.s, i)) y |= BIT(i);
	}
	if(expa) x |= BIT(W_FRAC);
	if(expb) y |= BIT(W_FRAC);
	if(expa && (!expb)) --exp;
	if((!expa) && expb) ++exp;
	x <<= W;
	z = x % y;
	x /= y;
	int st, ed;
	db c;
	memset(c.s, 0, sizeof(c.s));
	if(sgn) SET(c.s, W - 1);
	if(x == 0) return sgn ? ZERO_N : ZERO;
	int pos, top = W_FRAC + W;
	for(int i = top; i >= 0; --i) if(VAL(s, i)) {
		pos = i;
		break;
	}
	int dlt = W - pos;
	if(exp > 0) {
		if(dlt < 0) ed = pos - 1, exp += (-dlt);
		else if(dlt == 0) ed = pos - 1;
		else {
			if(dlt >= exp) ed = W - exp, exp = 0;
			else ed = pos - 1, exp -= dlt;
		}
	}
	else if(exp == 0) {
		if(dlt < 0) ed = pos - 1, exp += -dlt;
		else if(dlt == 0) ed = pos, exp = 0;
		else ed = W , exp = 0;
	}
	else {
		if(dlt < 0) {
			if(-dlt > -exp) ed = W + (-exp), exp = 1;
			else if(-dlt == -exp) ed = pos, exp = 0;
			else ed = W + (-exp), exp = 0;
		}
		else ed = W + (-exp), exp = 0;
	}
	st = ed - W_FRAC + 1;
	uint8_t flg = 0;
	ROUND(z > 0);
	if(!flg) {
		// printf("%d %d\n", st, ed);
		if(exp + 1 >= BIT(W_EXP)) return sgn ? INF_N : INF_P;
		for(int i = st; i <= ed; ++i)
			if(i >= 0 && i <= W + W_FRAC && VAL(s, i)) SET(c.s, i - st);
		for(int i = W_FRAC; i < W - 1; ++i) 
			if(exp & BIT(i - W_FRAC)) SET(c.s, i);
		return c;
	}
	else {
		if(exp >= 0) ++exp;
		else exp = 0;
		if(exp >= BIT(W_EXP) - 1) return sgn ? INF_N : INF_P;
		for(int i = W_FRAC; i < W - 1; ++i) {
			if(exp & BIT(i - W_FRAC)) SET(c.s, i); 
		}
		return c;
	}
}

db convert(double x) {
	db ret;
	for(int i = 0; i < BYTE_SIZE; ++i) 
		ret.s[i] = *((uint8_t *)(&x) + i);
	return ret;
}
db trans(uint64_t *x) {
	return *((db *)x);
}
void output(db x, char ch) {
	printf("%.6lf%c", *((double *)(x.s)), ch);
}
void input(db *x) {
	scanf("%lf", x);
}
void sh(void *x) {
	for(int i = 0; i < W_FRAC; ++i) {
		if(VAL((char *)x, i)) printf("1");
		else printf("0");
	}
	printf(" ");
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL((char *)x, i)) printf("1");
		else printf("0"); 
	}
	printf(" ");
	printf("%d\n",VAL((char *)x, W - 1));
}
uint8_t cmp(uint8_t *a, uint8_t *b) {
	for(int i = 0; i < BYTE_SIZE; ++i) if(a[i] != b[i]) return 1;
	return 0;
}
uint64_t conv(db x) {
	uint64_t ret = 0;
	*(&ret) = *((uint64_t *)&x);
	return ret;
}
uint64_t calculate(uint64_t a, uint64_t b, char op) {
	db x = trans(&a), y = trans(&b), z;
	if(op == '+') {
		if(VAL(x.s, W - 1) && (!VAL(y.s, W - 1))) z = sub(y, getNeg(x));
		if((!VAL(x.s, W - 1)) && VAL(y.s, W - 1))  z = sub(x, getNeg(y));
		if(VAL(x.s, W - 1) && VAL(y.s, W - 1)) 	z = add(x, y);
		if((!VAL(x.s, W - 1)) && (!VAL(y.s, W - 1))) z = add(x, y);
	}
	if(op == '-') {
		if(VAL(x.s, W - 1) && (!VAL(y.s, W - 1))) z = add(x, getNeg(y));
		if((!VAL(x.s, W - 1)) && VAL(y.s, W - 1))  z = add(x, getNeg(y));
		if(VAL(x.s, W - 1) && VAL(y.s, W - 1)) z = sub(x, y);
		if((!VAL(x.s, W - 1)) && (!VAL(y.s, W - 1))) z = sub(x, y);
	}
	if(op == '*') z = mul(x, y);
	if(op == '/') z = DIVISION(x, y);
	return conv(z);
}

int main() {
	// freopen("data.in", "r",stdin);
	// freopen("my.out", "w",stdout);
	int cas = 1;
	// scanf("%d",&cas);
	for(int tim = 1; tim <= cas; ++tim) {
		// if(tim % 10000 == 0) printf("=========================%d=======================\n", tim);
		db a, b;
		uint64_t xx = 0x11b21dddd1a4b2e7, yy = 0xee4533c48361c678;
		
		double x, y;
		*(&x) = *((double *) &xx);
		*(&y) = *((double *) &yy);
		// scanf("%lf%lf",&x,&y);
		a=convert(x),b=convert(y);
		// sh(&a), sh(&b);
		db c = DIVISION(a, b);
		double z = x / y;
		sh(&c), sh(&z);
		
		//sh(&c);
		// output(c, ','), print6f("%.6lf", z);
		// printf("%lf\n", z);
		if(cmp((char*)&c, (char*)&z)) {
			printf("Error! [%d]\n", tim);
			sh(&a), sh(&b), sh(&c), sh(&z);
			exit(0);
		}
		// else system("clear");
	}
}