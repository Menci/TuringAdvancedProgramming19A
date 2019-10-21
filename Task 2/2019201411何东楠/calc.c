#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define FRAC_BEG  (0)
#define FRAC_END (51)
#define FRAC_LEN (52)

#define EXP_BEG (52)
#define EXP_END (62)

#define K (11)
#define BIAS ((1 << (K - 1)) - 1)

#define MAX_BIT (8)
#define EACHBIT (8)

#define MAX_LEN (100000)

void swap_int(int *a, int *b) {
	int tmp_ = *a;
	*a = *b;
	*b = tmp_;
}

typedef struct 
{
	unsigned char c[MAX_BIT];
}my_double;

void Init(my_double *rhs) 
{
	memset((*rhs).c, 0, sizeof (*rhs).c);
}

void swap_str(my_double *a, my_double *b)
{
	my_double tmp_ = *a;
	*a = *b;
	*b = tmp_;
}

void Read(my_double *rhs)
{
	scanf("%lf", &(*rhs));
}

void Print(my_double rhs)
{
	printf("%.50le\n", *(double *) &rhs);
}

int get_exp(my_double rhs)
{
	return ((rhs.c[MAX_BIT - 1] & ((1 << (EACHBIT - 1)) - 1)) << 4) + (rhs.c[MAX_BIT - 2] >> 4);
}

int get_opt(my_double rhs)
{
	return (rhs.c[MAX_BIT - 1] >> (EACHBIT - 1)) & 1;
}

int get_bit(my_double rhs, int Pos)
{
	int i = Pos / 8, bias_ = Pos % 8;

	if (Pos > FRAC_LEN) {
		return 0;
	}
	if (Pos < 0) {
		return 0;
	}
	if (Pos == FRAC_LEN) {
		return get_exp(rhs) != 0;
	}
//	Printf("%d %d\n", i, bias_);
	return (rhs.c[i] >> bias_) & 1;
}

int get_num(my_double rhs, int Pos)
{
	int i = Pos / 8, bias_ = Pos % 8;
	return (rhs.c[i] >> bias_) & 1;
}

void Print_byte(my_double rhs)
{
	for (int i = MAX_BIT - 1; i >= 0; --i) {
//		Printf("%d", rhs.c[i]);
		for (int j = EACHBIT - 1; j >= 0; --j)
			printf("%d", (rhs.c[i] >> j) & 1);
//			get_bit(rhs, i * MAX_BIT + j);
	}
	printf("!\n");
}

void modify_num(my_double *rhs, int Pos, int newVal)
{
	int i = Pos / 8, bias_ = Pos % 8;
	int curVal = (*rhs).c[i] & (1 << bias_);
	(*rhs).c[i] += - curVal + (newVal << bias_);
}

//calc A ? B
int Plus(my_double A, my_double B, my_double *Ans)
{
//	Print_byte(A); Print_byte(B);
	int expA = get_exp(A);
	int expB = get_exp(B);
	if (expA < expB) {
		swap_str(&A, &B);
		swap_int(&expA, &expB);
	}

	int bias_ = expA - expB;
	if (expA != 0 && expB == 0) bias_ --;
	if (bias_ > FRAC_LEN + 2) {
		*Ans = A;
		(*Ans).c[7] &= ((1 << 7) - 1);
		return 1;
	}

	char ret[FRAC_LEN*2 + 10] = {0};
	for (int i = FRAC_BEG; i <= FRAC_END; ++i) {
		ret[i] += get_bit(A, i) + get_bit(B, i + bias_);

		if (ret[i] >= 2) {
			ret[i + 1] += ret[i] / 2;
			ret[i] &= 1;
		}
//		printf("%d", ret[i]);
	}
//	printf("\n");

	int Limit = (expA + expB == 0) ? 0 : 1;
	ret[FRAC_LEN] += (expA != 0) + (bias_ == 0) * (expB != 0);
	int _add = ret[FRAC_LEN] > Limit;

//	printf("%d\n", bias_ + _add - 1);
	if (bias_ + _add - 1 >= 0 && (get_bit(A, _add - 1) + get_bit(B, bias_ + _add - 1)) % 2) {
		int Type = 0;//0 - even; 1 - +1
		for (int i = bias_ - 2 + _add; i >= 0; --i) {
			if (get_bit(B, i)) { 
				Type = 1;
				break;
			}
		}

		if (Type == 1 || (Type == 0 && ret[0 + _add] == 1)) {
			ret[0 + _add] ++;
			for (int cur = _add; ret[cur] >= 2 && cur < FRAC_LEN + _add; cur ++) {
				ret[cur + 1] ++;
			   	ret[cur] &= 1;
			}
		}
	}

	int newExp = expA + _add;
	if (expA + expB == 0) _add = 0;

	if (newExp >= (1 << K) - 1) return 0;
	for (int i = EXP_BEG; i <= EXP_END; ++i) {
		modify_num(&(*Ans), i, newExp & 1);
		newExp >>= 1;
	}

	for (int i = FRAC_BEG; i <= FRAC_END; ++i) {
		modify_num(&(*Ans), i, ret[i + _add] & 1);//special to d + d
	}

	return 1;
}

//default - (A > B) A - B
//N - D = D; N - N = D;
void Minus(my_double A, my_double B, my_double *Ans)
{
//	Print_byte(A), Print_byte(B);
	int expA = get_exp(A);
	int expB = get_exp(B);
	int bias_ = expA - expB;
	if (expA != 0 && expB == 0) bias_ --;
	if (bias_ > FRAC_LEN + 2) {
		*Ans = A;
		return ;
	}

	char ret[FRAC_LEN * 2 + 10] = {0};
	for (int i = -bias_; i <= FRAC_END + 1; ++i) {
		ret[i + bias_] = get_bit(A, i) - get_bit(B, i + bias_);
//		if (i == FRAC_END + 1) Printf("%d\n", ret[i + bias_]);
	}
	for (int i = -bias_; i <= FRAC_END; ++i) {
		if (ret[i + bias_] < 0) {
			ret[i + bias_] += 2;
		   	ret[i + bias_ + 1] --;
		}
	}

//	for (int i = FRAC_END + 1; i >= -bias_; --i)
//		printf("%d", ret[i + bias_]); printf("\n");
	int delta_ = 0, exist1 = 0;
	for (int i = FRAC_END; i >= -bias_; --i) {
		delta_ ++;
		if (ret[i + bias_] == 1) {
			exist1 = 1;
			break;
		}
	}

	int newExp = expA;
	if (ret[FRAC_END + 1 + bias_] == 0 && expA + expB != 0 && exist1) {
		if (newExp > delta_) {
			newExp -= delta_;
		}
		else {
			delta_ = newExp - 1;//N - D = D !!!!!!!
			newExp = 0;
		}

		if (bias_ - delta_ > 0) {
			if (ret[FRAC_END - delta_ - FRAC_LEN + bias_] == 1) {
				int Type = 0;//0 - even; 1 - +1
				for (int i = FRAC_END - delta_ - FRAC_LEN - 1; i >= -bias_; --i) {
					if (ret[i + bias_] == 1) {
						Type = 1;
						break;
					}
				}

				if (Type == 1 || (Type == 0 && ret[-delta_ + bias_] == 1)) {
					ret[-delta_ + bias_] ++;
					for (int cur = -delta_; ret[cur + bias_] >= 2 && cur <= FRAC_LEN - delta_ - 1; cur++) {
						ret[cur + bias_ + 1] ++;
						ret[cur + bias_] &= 1;
					}
				}
			}
		}
		for (int i = -delta_, cur = 0; i <= FRAC_LEN - delta_ - 1; i++, cur++) {
			if (i + bias_ >= 0) {
				modify_num(&(*Ans), cur, ret[i + bias_]);
			}
			else {
				modify_num(&(*Ans), cur, 0);
			}
		}

	}
	else {
		if (bias_ - 1 >= 0 && ret[bias_ - 1] > 0) {
			int Type = 0;//0 - even; 1 - +1
			for (int i = -2; i >= -bias_; --i) {
				if (ret[i + bias_] > 0) { 
					Type = 1;
					break;
				}
			}

			if (Type == 1 || (Type == 0 && ret[0 + bias_] == 1)) {
				ret[0 + bias_] ++;
				for (int cur = bias_; ret[cur] >= 2 && cur < FRAC_LEN + bias_; cur ++) {
					ret[cur + 1] ++;
					ret[cur] &= 1;
				}
			}
		}
		
		for (int i = FRAC_BEG; i <= FRAC_END; ++i) {
			modify_num(&(*Ans), i, ret[i + bias_]);
		}
	}

	if (ret[FRAC_END + 1 + bias_] == 0 && !exist1) 
		newExp = 0;

	for (int i = EXP_BEG; i <= EXP_END; ++i) {
		modify_num(&(*Ans), i, newExp & 1);
		newExp >>= 1;
	}
}

#define MAX_INTER (2 * FRAC_LEN)

__int128 _1 = 1;

__int128 get_frac(my_double rhs)//contain 1./0.
{
	__int128 ret = 0, bias_ = 0, tmp_ =0;
	for (int i = 0; i < 6; ++i) {
		tmp_ = rhs.c[i];
		ret += tmp_ << bias_;
		bias_ += EACHBIT;
	}
	tmp_ = rhs.c[6] & 15;
	ret += (tmp_ << bias_) + ((_1*(get_exp(rhs) != 0)) << (bias_ + 4));
	return ret;
}

int Rounding(__int128 *val, int Pos, int flag)
{
	if (!((*val) & (_1 << Pos))) return 0;
	if (flag) (*val) += _1 << (Pos + 1);
	else if ((*val) & ((_1 << Pos) - 1)) (*val) += _1 << (Pos + 1);
	else if ((*val) & (_1 << (Pos + 1))) (*val) += _1 << (Pos + 1);
	else return 0;
	return 1;
}

void Print_128(__int128 tmp_, int len)
{
	for (int i = len; i >= 0; --i) {
		printf("%d", (int) ((tmp_ >> i) & _1));
	}
	printf("\n");
}

int Multy(my_double A, my_double B, my_double *Ans)
{
	int expA = get_exp(A), expB = get_exp(B);
	int newExp = expA + expB - BIAS;
	__int128 fracA = get_frac(A), fracB = get_frac(B);
	__int128 newFrac = fracA * fracB;
//	Print_128(fracA, FRAC_LEN);
//	Print_128(fracB, FRAC_LEN);
//	Print_128(newFrac, MAX_INTER + 1);

	if (newExp == -BIAS) {
		return 1;
	}
	if (newExp < -2 * FRAC_LEN) {
		return 1;
	}

	if (!expA || !expB) {
		newExp ++;
	}

	int flag = 0;
	if (newExp >= 0) {
		if (newFrac < (_1 << MAX_INTER)) {
			while (newExp && newFrac < (_1 << MAX_INTER)) {
				newExp --;
				newFrac <<= _1;
			}
		}
	}
	else {
		if (newFrac & ((_1 << (-newExp)) - 1)) flag = 1;
		newFrac >>= -newExp;
		newExp = 0;
	}

	int xx = 0;
	if (newFrac >= (_1 << (MAX_INTER+1))) {
		if (newFrac & _1) flag |= 1;
		newFrac >>= _1;
		newExp ++;
		if (newExp == 0) xx = 1;
	}

//	Print_128(newFrac, MAX_INTER + 1);
//	Rounding(&newFrac, MAX_INTER - FRAC_LEN - 1, flag);

/*	if (newFrac >= (_1 << (MAX_INTER+1))) {
		newFrac >>= _1;
		newExp ++;
	}*/

	if (newExp >= (1 << K) - 1) {
		return 0;
	}

	if (newExp == 0 && !xx) {
		newFrac >>= 1;
/*		if (newFrac < (_1 << MAX_INTER)) {
			flag |= newFrac & 1;
			newFrac >>= 1;
		}
		else {
			newExp = 1;
		}*/
	}
//	Print_128(newFrac, MAX_INTER + 1);
	Rounding(&newFrac, MAX_INTER - FRAC_LEN - 1, flag);

	for (int i = MAX_INTER - FRAC_LEN, cur = 0; i <= MAX_INTER - 1; ++i, ++cur) {
		modify_num(&(*Ans), cur, (newFrac >> i) & 1);//FIXME
	}
	for (int i = EXP_BEG; i <= EXP_END; ++i) {
		modify_num(&(*Ans), i, newExp & 1);
		newExp >>= 1;
	}
//	Print_byte(*Ans);
	return 1;
}

#define DBIAS (70)

int Divide(my_double A, my_double B, my_double *Ans)
{
	int expA = get_exp(A), expB = get_exp(B);
	int newExp = expA - expB + BIAS;
	if (expB == 0) newExp --;
	if (expA == 0) newExp ++;
	__int128 fracA = get_frac(A) << DBIAS, fracB = get_frac(B);

	if (fracB == 0) 
		return -1; //NaN

	__int128 newFrac = fracA / fracB;
//	Print_128(newFrac, DBIAS);

	if (newExp < -DBIAS) return 1;

	if (newExp < 0) {
		newFrac >>= -newExp;
		newExp = 0;
	}

	int xx = 0;
	while (newFrac >= (_1 << (DBIAS + 1))) {
		newExp ++;
		newFrac >>= 1;
		if (newExp == 0) xx = 1;
	}

	while (newExp && newFrac < (_1 << DBIAS)) {
		newExp --;
		newFrac <<= 1;
	}

	if (newExp == 0 && !xx) {
		newFrac >>= 1;
	}

	if (newExp >= (1 << K) - 1) 
		return 0; //Inf

	Rounding(&newFrac, 69 - FRAC_LEN, fracA % fracB != 0);
	for (int i = DBIAS - FRAC_LEN, cur = 0; i <= DBIAS-1; ++i, ++cur) {
		modify_num(&(*Ans), cur, (newFrac >> i) & _1);
	}
	for (int i = EXP_BEG; i <= EXP_END; ++i) {
		modify_num(&(*Ans), i, newExp & 1);
		newExp >>= 1;
	}
	return 1;
}

int is_INF(my_double rhs)
{
	if (get_exp(rhs) == (1 << (K + 1) - 1) && get_frac(rhs) == (__int128)0) {
		return get_opt(rhs) ? -1 : +1;
	}
	return 0;
}

int is_NaN(my_double rhs)
{
	return get_exp(rhs) == ((1 << K) - 1) && get_frac(rhs) != (__int128)0;
}

uint64_t TrAns(my_double Ans) 
{
	uint64_t _Ans = 0;
	for (int i = 0; i <= 7; ++i) {
		uint64_t Tmp = Ans.c[i];
		_Ans += Tmp << ((uint64_t) (i * 8));
	}
	return _Ans;
}

my_double _INF[2], _NAN[2], round_1;

void pretreat()
{
	uint64_t INF[] = {9218868437227405312, 18442240474082181120ull};
	uint64_t NAN[] = {9218868437227405313, 18442240474082181121ull};

	for (int u = 0; u < 2; ++u) {
		Init(&_INF[u]);
		Init(&_NAN[u]);
		for (int i = 0; i <= 7; ++i) {
			_INF[u].c[i] = INF[u] & (uint64_t) 255;
			_NAN[u].c[i] = NAN[u] & (uint64_t) 255;
			INF[u] >>= 8;
			NAN[u] >>= 8;
		}
	}
}

my_double Calc(my_double A, my_double B, char ch)
{
	int is_INF1 = is_INF(A), is_INF2 = is_INF(B);
	int is_NaN1 = is_NaN(A), is_NaN2 = is_NaN(B);

	if (is_NaN1) {
		return A;
	}
	if (is_NaN2) {
		return B;
	}

	my_double Ans; Init(&Ans);

	int Type1 = get_opt(A);
	int Type2 = get_opt(B);

	if (ch == '+' || ch == '-') {
		int Type0 = ch == '-';
		if (!(Type1 ^ Type2 ^ Type0)) {
			if (!Plus(A, B, &Ans)) {
				return _INF[get_opt(A)];
			}
			Ans.c[7] |= get_opt(A) << 7;
		}
		else {
			A.c[7] &= (1 << 7) - 1;
			B.c[7] &= (1 << 7) - 1;
			int Type = ((*(double *) &A) < (*(double *) &B)) ^ Type1;
			if (is_INF1 && is_INF2) {
				return _NAN[Type];
			}
			if ((*(double *) &A) < (*(double *) &B)) {
				Minus(B, A, &Ans);
			}
			else {
				Minus(A, B, &Ans);
			}
			Ans.c[7] |= Type << 7;
		}
	}
	else if (ch == '*') {
		if (!Multy(A, B, &Ans)) {
			return _INF[Type1 ^ Type2];
		}
		Ans.c[7] |= (Type1 ^ Type2) << 7;
	}
	else {
		int ret = Divide(A, B, &Ans);
		if (ret == 0) {
			return _INF[Type1 ^ Type2];
		}
		if (ret < 0) {
			return _NAN[Type1 ^ Type2];
		}
		Ans.c[7] |= (Type1 ^ Type2) << 7;
	}

//	Print(Ans);
//	Print_byte(Ans);
	return Ans;
}

char strNew[MAX_LEN] = {0};

int get_integer(char *str, int *len)
{
	int rest = 0;

//	printf("%s\n", str);
	for (int i = (*len) - 1; i >= 0; --i) {
		int d = rest * 10 + (str[i] - '0');
		strNew[i] = d / 2 + '0';
		rest = d % 2;
	}

	for (int i = 0; i < (*len); ++i) 
		str[i] = strNew[i];
	while (str[(*len) - 1] == '0') 
		(*len) --;
//	printf("%s\n", str);

	return rest;
}

int get_back(char *str, int *len)
{
	str[(*len)] = 0;
	for (int i = (*len) - 1; i >= 0; --i) {
		str[i] = str[i] * 2;
		if (i != (*len) - 1) {
			str[i] += str[i + 1] / 10;
			str[i + 1] %= 10;
		}
	}
	int flag = str[0] >= 10;
	str[0] %= 10;
	while ((*len) && str[(*len) - 1] == 0) (*len) --;
	return flag;
}

int is_num(char c)
{
	return '0' <= c && c <= '9';
}

int numTmp[MAX_LEN + 5];
char numStr[MAX_LEN + 5];

my_double trans_to_num(char *str)
{

	my_double rhs; Init(&rhs);
	int len = strlen(str), ed = 0;
	if (len == 1 && str[0] == '0') return rhs;

	int flag = 0;

	if (str[0] == '-') {
		modify_num(&rhs, 63, 1);
		for (int i = 0; i < len - 1; ++i) str[i] = str[i + 1];
	}

	for (ed = 0; ed < len && is_num(str[ed]); ++ed);
	for (int i = 0; i < ed; ++i) {
		numStr[i] = str[ed - 1 - i];
	}

	int cur = 6, _clock = 3, e = 0, pos = 0;
	int cnt = 0;

	//integer
	if (str[0] != '0') {
		int integerLen = ed;
		while (integerLen) {
			numTmp[cnt++] = get_integer(numStr, &integerLen);
			e ++;
		}
		e--;

		for (int i = 0; i < ed; ++i) {
			numStr[i] = str[ed - 1 - i];
		}
		integerLen = ed;
		get_integer(numStr, &integerLen);

		while (pos < cnt - 1 && pos < FRAC_LEN) {
			if (_clock < 0) {
				cur --, _clock = 7;
			}

			rhs.c[cur] += numTmp[cnt-pos-2] << _clock;

			_clock --; pos ++;
		}
		if (cnt - 1 >= FRAC_LEN && integerLen) {
			if (numTmp[cnt-pos-2]) {
				if (cnt - 1 == FRAC_LEN && (rhs.c[0] & 1)) flag = 1;
				else if (cnt - 1 > FRAC_LEN) flag = 1;
			}
		}
	}

	if (ed != len && pos < FRAC_LEN) {

		//frac
		for (int i = ed + 1; i < len; ++i) {
			numStr[i - ed - 1] = str[i] - '0';
		}
		int fracLen = len - ed - 1;
		if (str[0] == '0') {
			e--;
			while (get_back(numStr, &fracLen) == 0) e--;
		}
		if (e <= -BIAS) {
			for (int i = 1; i <= -e - BIAS + 1; ++i) {
				if (_clock < 0) {
					cur --, _clock = 7;
				}
				_clock --; pos ++;
			}
			if (pos < FRAC_LEN) {
				if (_clock < 0) {
					cur --, _clock = 7;
				}

				rhs.c[cur] += 1 << _clock;

				_clock --; pos ++;
			}
		}
		while (pos < FRAC_LEN && fracLen) {
			if (_clock < 0) {
				cur --, _clock = 7;
			}

			int bit = get_back(numStr, &fracLen);
			rhs.c[cur] += bit << _clock;

			_clock --; pos ++;
		}
		if (fracLen && get_back(numStr, &fracLen)) {
			if (fracLen == 0) {
				if (rhs.c[0] & 1) flag = 1;
			}
			else {
				flag = 1;
			}
		}
	}

	e += (1 << (K - 1)) - 1;
	if (e <= 0) return rhs;

	for (int i = EXP_BEG; i <= EXP_END; ++i) {
		modify_num(&rhs, i, e & 1);
		e >>= 1;
	}

	if (flag) {
		for (int i = FRAC_BEG; i <= FRAC_END; ++i) {
			if (get_num(rhs, i)) modify_num(&rhs, i, 0);
			else {
				modify_num(&rhs, i, 1);
				flag = 0;
				break;
			}
		}
		if (flag) {
			for (int i = EXP_BEG; i <= EXP_END; ++i) {
				if (get_num(rhs, i)) modify_num(&rhs, i, 0);
				else {
					modify_num(&rhs, i, 1);
					break;
				}
			}
		}
	}

	return rhs;
}

int res[MAX_LEN + 5], resLen;
int tmp[MAX_LEN + 5], tmpLen;

//#define mod ((unsigned long long) 1e19)

void PRINT(__int128 x) {
	if (x >= 10) PRINT(x / 10);
	printf("%d", (int) (x % (_1 * 10)));
}

void print_my_double(my_double rhs)
{
	__int128 base1 = 1e11;
	__int128 base2 = 1e11;
	__int128 base = base1 * base2;
	int e = get_exp(rhs) - BIAS;
	if (get_exp(rhs) && get_frac(rhs) && get_opt(rhs)) {
		putchar('-');
		fflush(stdout);
	}
	if (e <= FRAC_LEN) {
		__int128 num = get_frac(rhs);
		if (FRAC_LEN - e < 100) {
			num = (num * base) / (_1 << (FRAC_LEN - e));
			printf("%llu.", (unsigned long long) (num / base));
			__int128 mod = base / 10;
			for (int i = 1; i <= 22; ++i) {
				printf("%d", num / mod % 10);
				fflush(stdout);
				mod /= 10;
			}
			putchar('\n');
			fflush(stdout);
		}
		else {
			printf("0\n");
			fflush(stdout);
		}
	}
	else {
		memset(res, 0, sizeof res); resLen = 0;
		memset(tmp, 0, sizeof tmp); tmpLen = 1;
		tmp[1] = 1;
		for (int times = 1; times <= e - FRAC_LEN; ++times) {
			for (int j = 1; j <= tmpLen; ++j) tmp[j] = tmp[j] * 2;
			for (int j = 1; j <= tmpLen; ++j) if (tmp[j] >= 10) {
				tmp[j + 1] += tmp[j] / 10;
				tmp[j] %= 10;
			}
			while (tmp[tmpLen + 1]) tmpLen ++;
		}

		int lst = 0;
		for (int i = FRAC_BEG; i <= FRAC_END + 1; ++i) if (get_bit(rhs, i)) {
			for (int times = 1; times <= i - lst; ++times) {
				for (int j = 1; j <= tmpLen; ++j) tmp[j] = tmp[j] * 2;
				for (int j = 1; j <= tmpLen; ++j) if (tmp[j] >= 10) {
					tmp[j + 1] += tmp[j] / 10;
					tmp[j] %= 10;
				}
				while (tmp[tmpLen + 1]) tmpLen ++;
			}

			resLen = tmpLen > resLen ? tmpLen : resLen;
			for (int j = 1; j <= resLen; ++j) res[j] += tmp[j];

			while (res[resLen + 1]) resLen ++;
			for (int j = 1; j <= resLen; ++j) if (res[j] >= 10) {
				res[j + 1] += res[j] / 10;
				res[j] %= 10;
			}
			while (res[resLen + 1]) resLen ++;

			lst = i;
		}
		for (int i = resLen; i >= 1; --i) {
			printf("%d", res[i]);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);
	}
}

int cnt = 0, top = 0;
typedef struct Node
{
	my_double rhs;
	char opt;
}Node;

Node Q[MAX_LEN + 5];

void push(char c)
{
	Q[++cnt].opt = c;
}

char num[MAX_LEN + 5];
char stk[MAX_LEN + 5];
my_double scalc[MAX_LEN + 5];

void solve()
{
	for (int i = 1; i <= cnt; ++i) {
		if (Q[i].opt == 0) {
			scalc[++top] = Q[i].rhs;
//			Print(Q[i].rhs);
		}
		else {
//			printf("%c\n", Q[i].opt);
			my_double a = scalc[top];
			my_double b = scalc[top - 1];
			top -= 2;
			scalc[++top] = Calc(b, a, Q[i].opt);
		}
	}

	assert(top == 1);
	print_my_double(scalc[top]);
}

int main()
{
//	freopen("/tmp/tmpg69qxt7p", "r", stdin);
//	freopen("output.out", "w", stdout);

	pretreat();
	char buf;
	int pfr[256 + 5];
	memset(pfr, -1, sizeof pfr);

	pfr['+'] = 0; pfr['-'] = 0;
	pfr['*'] = 1; pfr['/'] = 1;
	pfr['('] = -1; pfr[')'] = -1;


	int pre_num = 0, len = 0;
	cnt = 0, top = 0;

	while ((buf = getchar()) != EOF) {
		if (buf == ' ') continue;

		if (!is_num(buf) && buf != '.') {
			if (!pre_num && len) {
				Q[++cnt].rhs = trans_to_num(num);
				Q[cnt].opt = 0;
				pre_num = 1;
				for (int j = 0; j < len; ++j) num[j] = 0;
				len = 0;
			}
			if (!pre_num && len == 0 && (buf == '+' || buf == '-')) {
				num[len++] = buf;
			}
		}
		else if (!pre_num) {
			num[len++] = buf;
		}

		if (buf != '\n') {
			if (pre_num) {
				if (pfr[buf] >= 0) {
					while (top && pfr[stk[top]] > pfr[buf] && pfr[stk[top]] >= 0) {
						Q[++cnt].opt = stk[top];
						top --;
					}
					stk[++top] = buf;
					pre_num = 0;
				}
			}
			if (buf == '(') {
				stk[++top] = buf;
			}
			else if (buf == ')') {
				while (stk[top] != '(') {
					Q[++cnt].opt = stk[top];
					top --;
				}
				top --;
			}
		}
		else  {
			while (top) push(stk[top--]);

			if (cnt) solve();

			cnt = 0;
			top = 0;
			pre_num = 0;
		}
	}

	if (len) {
		Q[++cnt].rhs = trans_to_num(num);
		Q[cnt].opt = 0;
		pre_num = 1;
		for (int j = 0; j < len; ++j) num[j] = 0;
		len = 0;
	}

	while (top) push(stk[top--]);
	if (cnt) solve();

	return 0;
}

