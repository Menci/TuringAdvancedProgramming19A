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

uint64_t Calc(uint64_t a, uint64_t b, char ch)
{
	uint64_t INF[] = {9218868437227405312, 18442240474082181120ull};
	uint64_t NAN[] = {9218868437227405313, 18442240474082181121ull};

	my_double A; Init(&A);
	my_double B; Init(&B);
	for (int i = 0; i <= 7; ++i) {
		A.c[i] = a & (uint64_t) 255;
		B.c[i] = b & (uint64_t) 255;
		a >>= 8;
		b >>= 8;
	}

	int is_INF1 = is_INF(A), is_INF2 = is_INF(B);
	int is_NaN1 = is_NaN(A), is_NaN2 = is_NaN(B);

	if (is_NaN1) {
		return TrAns(A);
	}
	if (is_NaN2) {
		return TrAns(B);
	}

	my_double Ans; Init(&Ans);

	int Type1 = get_opt(A);
	int Type2 = get_opt(B);

	if (ch == '+' || ch == '-') {
		int Type0 = ch == '-';
		if (!(Type1 ^ Type2 ^ Type0)) {
			if (!Plus(A, B, &Ans)) {
				return INF[get_opt(A)];
			}
			Ans.c[7] |= get_opt(A) << 7;
		}
		else {
			A.c[7] &= (1 << 7) - 1;
			B.c[7] &= (1 << 7) - 1;
			int Type = ((*(double *) &A) < (*(double *) &B)) ^ Type1;
			if (is_INF1 && is_INF2) {
				return NAN[Type];
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
			return INF[Type1 ^ Type2];
		}
		Ans.c[7] |= (Type1 ^ Type2) << 7;
	}
	else {
		int ret = Divide(A, B, &Ans);
		if (ret == 0) {
			return INF[Type1 ^ Type2];
		}
		if (ret < 0) {
			return NAN[Type1 ^ Type2];
		}
		Ans.c[7] |= (Type1 ^ Type2) << 7;
	}

//	Print(Ans);
//	Print_byte(Ans);
	return TrAns(Ans);
}

int main()
{
//	freopen("IEEE.in", "r", stdin);
	freopen("IEEE.out", "w", stdout);

	Calc(0x800ac9d274e7218a, 0x800f02769e926c40, '+');
/*	my_double A, B, Ans; Init(&Ans);
	Read(&A);
	scanf("%*s");
	Read(&B);
	Minus(B, A, &Ans);
	Print(Ans);
	*/
	return 0;
}
//my_double + my_double

