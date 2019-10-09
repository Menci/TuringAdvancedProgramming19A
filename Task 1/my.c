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
#define ULLF(x) (*((unsigned long long *)(&(x))))
#define ULLP(x) (*((unsigned long long *)(x)))


long long BIAS = (1 << (11 - 1)) - 1;
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
int cmpgr(const db a, const db b) {
	if(VAL(a.s, W - 1) == VAL(b.s, W - 1)) {
		for(int i = W - 2; i >= 0; --i) {
			if(VAL(a.s, i) != VAL(b.s, i)) {
				if(VAL(a.s, W - 1)) 
					return VAL(a.s, i) < VAL(b.s, i);
				else 
					return VAL(a.s, i) > VAL(b.s, i);
			}
		}
		return 0;
	}
	else {
		return VAL(a.s, W - 1) < VAL(b.s, W - 1);
	}
}
int cmpq(const db a, const db b) {
	for(int i = 0; i < BYTE_SIZE; ++i) 
		if(a.s[i] != b.s[i]) return 0;
	return 1;
}
int cmpleq(const db a, const db b) {
	return cmpq(a, b) || cmple(a, b);
}
int cmpgrq(const db a, const db b) {
	return cmpq(a, b) || cmpgr(a, b);
}
int cmpnq(const db a, const db b) {
	return ! cmpq(a, b);
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


int addition(uint8_t *s, const int x, const int y) {
	// printf("@ %d %d\n", x, y);
	for(int i = x; i < y; ++i) {
		TOGGLE(s, i);
		if(VAL(s, i)) break;
		if(i == y - 1) return 1;
	}
	return 0;
}
int subtraction(uint8_t *s, const int x, const int y) {
	for(int i = x; i < y; ++i) {
		TOGGLE(s, i);
		if(! VAL(s, i)) break;
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


// db make_double(uint8_t *s, uint8_t st, uint8_t ed, long long exp, uint8_t *res) {
// 	db c;
// 	uint8_t t[2 * BYTE_SIZE + 2];
// 	memcpy(t, s, sizeof(s));
// 	res = 0;
// 	uint8_t inc = 0;
// 	if(VAL(t, st)) {
// 		if(VAL(t, st - 1)) inc += addition(t, st, ed + 1); 
// 	}
// 	else {
// 		if(VAL(t, st - 1)) {
// 			uint8_t ok = 0;
// 			for(int i = st - 2; i >= 0; --i) if(VAL, t, i) {
// 				ok = 1;
// 				break;
// 			}
// 			if(ok) inc += addition(t, st, ed + 1);
// 		}
// 	}
// 	if(inc) {

// 	}
// 	return c;
// }

db add(db a, db b) {
	int8_t ta = getType(a), tb = getType(b);
	if(ta == 2 && tb == -2) return NAN_P;
	if(ta == -2 && tb == 2) return NAN_N;
	if(ta == 2 || tb == 2) return NAN_P;
	if(ta == -2 || tb == -2) return NAN_N;
	if(ta == 1 && tb == -1 || ta == -1 && tb == 1) return NAN_N;
	if(ta == 1 && tb == 1) return INF_P;
	if(ta == -1 && tb == -1) return INF_N;
	long long expa = 0, expb = 0, dexp;
	for(int i = W_FRAC; i < W - 1; ++i) 
		if(VAL(a.s, i)) expa += BIT(i - W_FRAC);
	for(int i = W_FRAC; i < W - 1; ++i)
		if(VAL(b.s, i)) expb += BIT(i - W_FRAC);
	if(expa < expb) {
		db tmp = a;
		a = b, b = tmp;
		expa ^= expb, expb ^= expa, expa ^= expb;
	}
	dexp = expa - expb;
	int inc = 0;
	uint8_t s[2 * BYTE_SIZE], flg = 0;
	memset(s, 0, sizeof(s));
	for(int i = 0; i <= W_FRAC; ++i) {
		if((i < W_FRAC && VAL(a.s, i)) || (i == W_FRAC && expa)) 
			addition(s, i, W_FRAC + 2);
		if((i < W_FRAC && VAL(b.s, i)) || (i == W_FRAC && expb)) {
			if(i - dexp >= 0) addition(s, i - dexp, W_FRAC + 2);
			else flg = 1;
		}
	}
	db c;
	memset(c.s, 0, sizeof(c.s));
	uint8_t sgn = (VAL(a.s, W - 1) && VAL(b.s, W - 1));
	if(sgn) SET(c.s, W - 1);
	uint8_t flag = 0;
	if(dexp > 0 && dexp <= W_FRAC && (!VAL(s, W_FRAC + 1))) {
		if(dexp == 1) {
			if(VAL(s, 0) && VAL(b.s, 0)) flag = 1;
		}
		else {
			if(VAL(s, 0)) {
				if(VAL(b.s, dexp - 1)) flag = 1;
			}
			else {
				if(VAL(b.s, dexp - 1)) {
					for(int i = dexp - 2; i >= 0; --i) {
						if(VAL(b.s, i)) {
							flag = 1;
							break;
						}
					}
				}
			}
		}
		if(flag) addition(s, 0, W_FRAC + 2);
	}
	//for(int i = 0; i <= W_FRAC + 1; ++i) printf("%d",(int)VAL(s,i)); puts("");
	if(VAL(s, W_FRAC + 1)) {
		if(expa + 1 >= BIT(W_EXP)) {
			if(sgn) return INF_N;
			else return INF_P;
		}
		long long exp = expa + 1;
		uint8_t st = 1, ed = W_FRAC;
		if(! flag){
			if(VAL(s, st)) {
				if(VAL(s, st - 1)) addition(s, st, W_FRAC + 2); 
			}
			else {
				if(VAL(s, st - 1) && flg) addition(s, st, W_FRAC + 2);
			}
		}
		for(int i = 0; i < W_EXP; ++i) if(exp & BIT(i)) SET(c.s, W_FRAC + i);
		for(int i = st; i <= ed; ++i) if(VAL(s, i)) SET(c.s, i - st);
		return c;
	}
	else {
		if(expa >= BIT(W_EXP)) {
			if(sgn) return INF_N;
			else return INF_P;
		}
		if((!expa) && (!expb) && VAL(s, W_FRAC)) ++expa;
		for(int i = 0; i < W_EXP; ++i) if(expa & BIT(i)) SET(c.s, W_FRAC + i);
		for(int i = 0; i < W_FRAC; ++i) if(VAL(s, i)) SET(c.s, i);
		return c;
	}
}
/*
	23:40 2019/10/7
	鍏充簬涔樻硶鐨勯棶棰橈細
		鏈€澶х殑denorm * 姣旇緝灏忕殑norm锛�1.9 2.0锛�
		缁撴灉涓簄orm
		鍦ㄥ鐞嗕箻娉曠殑鏃跺€欐媶浜嗘暣鏁颁綅鍜屽皬鏁颁綅锛屾帹瀵煎嚭涔樻硶鐨勬祦绋�
		鐒惰€屾垚涓簄orm浠ュ悗澶氬嚭鏉ヤ竴涓�1锛岄渶瑕佸鐞嗕竴涓�.
	
	鍑忔硶鐨勬祦绋嬶細
		鍜屽姞娉曚竴鏍峰厛瀵归綈
		纭畾缁撴灉鐨勭鍙凤紝鐒跺悗鎬绘槸瑕佺敤缁濆鍊煎ぇ鐨� - 缁濆鍊煎皬鐨�
		鍥犱负宸茬粡淇濊瘉浜嗙粷瀵瑰€肩殑澶у皬锛屾墍浠ュ榻愮殑閮ㄥ垎鐩存帴涓€浣嶄竴浣嶅仛鍑忔硶銆�
		瀵逛簬娌℃湁瀵归綈鐨勯儴鍒嗭紝鍙渶瑕�

*/

db sub(db a, db b) {
	int8_t ta = getType(a), tb = getType(b);
	if(ta == 2 && tb == -2) return NAN_P;
	if(ta == -2 && tb == 2) return NAN_N;
	if(ta == 2 || tb == 2) return NAN_P;
	if(ta == -2 || tb == -2) return NAN_N;
	if(ta == 1 && tb == 1 || ta == -1 && tb == -1) return NAN_N;
	if(ta == 1 && tb == -1) return INF_P;
	if(ta == -1 && tb == 1) return INF_N;
	uint8_t sgn = cmple(a, b);
	long long expa = 0, expb = 0;
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL(a.s, i - W_FRAC)) expa += BIT(i - W_FRAC);
		if(VAL(b.s, i - W_FRAC)) expb += BIT(i - W_FRAC);
	}
	if(expa <= expb) {
		uint8_t sw = 0;
		for(int i = W_FRAC - 1; i >= 0; --i) if(VAL(a.s, i) != VAL(b.s, i)) {
			if(VAL(a.s, i) < VAL(b.s, i)) sw = 1;
			break; 
		}
		if(sw || expa < expb) {
			expa ^= expb, expb ^= expa, expa ^= expb;
			sgn ^= 1;
			db c = a;
			a = b, b = c;
		}
	}
	__int128_t x = 0, y = 0;
	int dexp = expa - expb, top = 120;
	if(expa) SET((uint8_t *)(&x), top + 1);
	if(expb && top + 1 - dexp > 0) SET((uint8_t *)(&y), top + 1 - dexp);
	for(int i = 0; i < W_FRAC; ++i) {
		if(VAL(a.s, i)) SET((uint8_t *)(&x), top - i);
		if(VAL(b.s, i) && top - i - dexp >= 0) SET((uint8_t *)(&y), top - i - dexp); 
	}
	x -= y;
	if(x == 0) return ZERO;
	uint8_t *s = (uint8_t *) &x;
	int st = -1, ed = -1;
	for(int i = top + 1; i >= 0; --i) if(VAL(s, i)) {
		ed = i;
		break;
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
	//Frac Bits
	uint8_t s[BYTE_SIZE * 3 + 5];
	memset(s, 0, sizeof(s));
	long long exp = 0;
	for(int i = 0; i <= W_FRAC; ++i) {
		if(VAL(a.s, i) || (norma && i == W_FRAC)) {
			for(int j = 0; j <= W_FRAC; ++j) {
				if(VAL(b.s, j) || (normb && j == W_FRAC))
					addition(s, i + j, 2 * W_FRAC + 2);
			}
		}
	}

	if(norma + normb == 1) ++exp;
	for(int i = W_FRAC; i < W - 1; ++i) {
		if(VAL(a.s, i)) exp += BIT(i - W_FRAC);
		if(VAL(b.s, i)) exp += BIT(i - W_FRAC);
	}
	exp -= BIAS;
	if(exp < -W) return sgn ? ZERO_N : ZERO;
	int st, ed;

	//First Round
	// printf("exp = %d\n",exp);
	// for(int i = 0; i <= 2 * W_FRAC + 1; ++i) printf("%d", (int)VAL(s, i)); puts("");
	// for(int i = W_FRAC; i <= 2 * W_FRAC + 1; ++i)  printf("%d", (int)VAL(s, i)); puts("");
	uint8_t flag = 0;
	if(VAL(s, 2 * W_FRAC + 1)) { // 1x.xxxxx
		//puts("A");
		if(exp >= 0) ++exp, st = W_FRAC + 1, ed = 2 * W_FRAC;
		else st = W_FRAC - exp + 1, ed = 2 * W_FRAC - exp;
	}
	else if(VAL(s, 2 * W_FRAC)) {// 01.xxxxx
		//puts("B");
		if(exp > 0)  st = W_FRAC, ed = 2 * W_FRAC - 1;
		else if(exp == 0) st = W_FRAC + 1, ed = 2 * W_FRAC;
		else {
			ed = 2 * W_FRAC - exp, st = ed - W_FRAC + 1;
			++exp;
		}
	}
	else { // 00.xxxxx
		//puts("C");
		if(exp <= 0) st = W_FRAC - exp + 1, ed = 2 * W_FRAC - exp;
		else if(exp == 1) st = W_FRAC, ed = 2 * W_FRAC - 1, exp = 0;
		else {
			uint8_t ok = 0;
			int pos = -1;
			for(int i = 2 * W_FRAC - 1; i >= 0; --i) {
				//if(2 * W_FRAC - i > exp) break;
				if(VAL(s, i)) {
					ok = 1, pos = i;
					break;
				}
			}
			//printf("ok = %d\n", ok);
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
	if(VAL(s, st)) { // Rounding
		if(VAL(s, st - 1)) flg += addition(s, st, ed + 1);
	}
	else {
		if(VAL(s, st - 1)) {
			uint8_t ok = 0;
			for(int i = st - 2; i >= 0; --i) if(VAL(s, i)) {
				ok = 1;
				break;
			}
			if(ok) flg += addition(s, st, ed + 1);
		}
	}

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
		if(exp >= 0) {
			++exp;
			if(exp >= BIT(W_EXP) - 1) return sgn ? INF_N : INF_P;
			for(int i = W_FRAC; i < W - 1; ++i) {
				if(exp & BIT(i - W_FRAC)) SET(c.s, i); 
			}
		}
		else if(exp == 0) {
			
		}
		else { // exp < 0

		}
		// puts("FUCK");
		return c;
	}
	return c;
}
db div(db a, db b) {

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
	db x = trans(&a), y = trans(&b);
	if(op == '*') {
		db z = mul(x, y);
		return conv(z);
	}
}

int main() {
	//freopen("data.in", "r",stdin);
	// freopen("my.out", "w",stdout);
	int cas = 1;
	//scanf("%d",&cas);
	for(int tim = 1; tim <= cas; ++tim) {
		// if(tim % 10000 == 0) printf("=========================%d=======================\n", tim);
		db a, b;
		double x = 2.36617694e+00, y = -3.54870860e-309;
		//scanf("%lf%lf",&x,&y);
		a=convert(x),b=convert(y);
		// sh(&a), sh(&b);
		db c = mul(a, b);
		double z = x * y;
		// sh(&c), sh(&z);
		
		//sh(&c);
		// output(c, ','), print6f("%.6lf", z);
		// printf("%lf\n", z);
		if(cmp((char*)&c, (char*)&z)) {
			printf("Error! [%d]\n", tim);
			sh(&a), sh(&b), sh(&c), sh(&z);
			exit(0);
		}
		else system("clear");
	}
}