#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_OF_BITS_IN_BYTE 8

#define NUM_OF_BYTES 8
#define NUM_OF_FRAC 52

#define MOVABLE_BITS_OF_LONG_LONG 63
#define MOVABLE_BITS_OF_INT128 126

#define FIRST_BYTE_OF_DOUBLE 0
#define LAST_BYTE_OF_DOUBLE  7
#define FIRST_BIT_OF_DOUBLE 0
#define LAST_BIT_OF_DOUBLE  63

#define Bias 1023

#define BIT_OF_SIGN 63

#define NUM_OF_BITS_OF_EXP 11
#define START_BIT_OF_EXP 52
#define END_BIT_OF_EXP 62
#define EXP_MAX 0x7ff
#define E_MAX 1023
#define E_MIN -1022

#define START_BYTE_OF_FRAC 0
#define CONVERTIBLE_BYTES_OF_FRAC 6
#define START_BIT_OF_FRAC_AFTER_SET_BYTE 48
#define START_BIT_OF_FRAC 0
#define END_BIT_OF_FRAC 51
#define NUM_OF_BITS_OF_FRAC 52
#define BIT_OF_M 52

#define GREATER 1
#define LESS 0

#define BIT_LEFT_FOR_ROUNDING 0
#define NUM_OF_BITS_LEFT_FOR_ROUNDING 1

#define TOP_BIT_OF_INT128 126
#define BITS_LEFT_FOR_ADD_ROUNDING 54
#define BITS_SURPLUS_IN_MULTIPLICATION 52
#define BITS_ABSENT_IN_DIVISION 52
#define A_PIECE_OF_BITS_LEFT_FOR_DIVIDE_ROUNDING 10

typedef unsigned char *byte_pointer;

//FILE *input;
//FILE *output;

typedef struct my_double
{
	unsigned char bytes[NUM_OF_BYTES];
} my_double;

void show_bytes(byte_pointer, size_t);
void show_bits(byte_pointer, size_t);
my_double encode_into_my_double(double num);
double decode_into_double(my_double num);
my_double convert_to_my_double(uint64_t);
uint64_t convert_from_my_double(my_double);
my_double encode_uint_into_my_double(int num);

bool kth_bit(my_double, int);
void set_kth_bit(my_double *, int, bool);
void set_sign(my_double *, bool);
void set_exponent(my_double *, int);
void set_fraction(my_double *, unsigned long long);

bool calc_sign_bit(my_double);
int calc_sign(my_double);
int calc_exponent(my_double);
unsigned long long calc_fraction(my_double);
int calc_E(my_double);
unsigned long long calc_M(my_double);

bool is_denomalized(my_double);
bool is_nomalized(my_double);
bool is_special(my_double);
bool is_zero(my_double);
bool is_infinity(my_double);
bool is_nan(my_double);

my_double gen_my_double(bool, int, unsigned long long);
my_double gen_nan();
my_double gen_zero();
my_double gen_infinity();

my_double abs_my_double(my_double);
bool cmp_abs(my_double, my_double);
void swap(my_double *, my_double *);

__int128 rounding(__int128, int);
my_double adjust(int, __int128);

my_double minus(my_double);
my_double calc_add(my_double a, my_double b);
my_double calc_subtract(my_double a, my_double b);
my_double calc_multiply(my_double a, my_double b);
my_double calc_divide(my_double a, my_double b);

int max(int, int);
int min(int, int);

inline int max(int a, int b)
{
	return a > b ? a : b;
}

inline int min(int a, int b)
{
	return a < b ? a : b;
}

uint64_t my_calc_function(uint64_t, uint64_t, char);
void initialize();

void show_bytes(byte_pointer start, size_t len)
{
	size_t i;
	for (i = 0; i < len; ++i)
		printf("%.2x ", start[i]);
	putchar('\n');
	return;
}
void show_bits(byte_pointer start, size_t len)
{
	int i, j;
	for (i = 0; i < len; ++i)
		for (j = 0; j < NUM_OF_BITS_IN_BYTE; ++j)
			printf("%d ", (start[i] >> j) & 1);
	putchar('\n');
	return;
}

inline my_double encode_into_my_double(double num)
{
	int i;
	my_double res;
	byte_pointer pointer = (byte_pointer)&num;
	for (i = FIRST_BYTE_OF_DOUBLE; i <= LAST_BYTE_OF_DOUBLE; ++i, ++pointer)
		res.bytes[i] = *pointer;
	return res;
}

inline double decode_into_double(my_double num)
{
	int i;
	double res;
	byte_pointer pointer = (byte_pointer)&res;
	for (i = FIRST_BYTE_OF_DOUBLE; i <= LAST_BYTE_OF_DOUBLE; ++i, ++pointer)
		*pointer = num.bytes[i];
	return res;
}

inline my_double convert_to_my_double(uint64_t num)
{
	int i;
	my_double res;
	byte_pointer pointer = (byte_pointer)&num;
	for (i = FIRST_BYTE_OF_DOUBLE; i <= LAST_BYTE_OF_DOUBLE; ++i, ++pointer)
		res.bytes[i] = *pointer;
	return res;
}

inline uint64_t convert_from_my_double(my_double num)
{
	int i;
	uint64_t res;
	byte_pointer pointer = (byte_pointer)&res;
	for (i = FIRST_BYTE_OF_DOUBLE; i <= LAST_BYTE_OF_DOUBLE; ++i, ++pointer)
		*pointer = num.bytes[i];
	return res;
}

inline my_double encode_uint_into_my_double(int num)
{
	if (!num)
		return gen_zero();
	my_double res;
	res = adjust(BIT_OF_M, num);
	set_sign(&res, 0);
	return res;
}

inline bool kth_bit(my_double num, int k)
{
	int i = k >> 3;
	k &= 7;
	return (num.bytes[i] >> k) & 1;
}

inline void set_kth_bit(my_double *num, int k, bool bit)
{
	int i = k >> 3;
	k &= 7;
	(*num).bytes[i] &= (UCHAR_MAX ^ (1 << k));
	(*num).bytes[i] |= (bit << k);
}

inline void set_sign(my_double *num, bool sign)
{
	set_kth_bit(num, BIT_OF_SIGN, sign);
}

inline void set_exponent(my_double *num, int exp)
{
	int i, j;
	for (i = START_BIT_OF_EXP, j = 0; i <= END_BIT_OF_EXP; ++i, ++j)
		set_kth_bit(num, i, (bool)((exp >> j) & 1));
}

inline void set_fraction(my_double *num, unsigned long long frac)
{
	int i;
	byte_pointer frac_pointer = (byte_pointer)&frac;
	byte_pointer num_pointer = (byte_pointer)num;

	for (i = START_BYTE_OF_FRAC; i < CONVERTIBLE_BYTES_OF_FRAC; ++i, frac_pointer++, num_pointer++)
		*num_pointer = *frac_pointer;
	for (i = START_BIT_OF_FRAC_AFTER_SET_BYTE; i <= END_BIT_OF_FRAC; ++i)
		set_kth_bit(num, i, (bool)((frac >> i) & 1));
}

inline bool calc_sign_bit(my_double num)
{
	return kth_bit(num, BIT_OF_SIGN);
}

inline int calc_sign(my_double num)
{
	return kth_bit(num, BIT_OF_SIGN) ? -1 : 1;
}

inline int calc_exponent(my_double num)
{
	return (convert_from_my_double(num) >> START_BIT_OF_EXP) & ((1 << NUM_OF_BITS_OF_EXP) - 1);
}

unsigned long long calc_fraction(my_double num)
{
	return convert_from_my_double(num) & ((1ull << BIT_OF_M) - 1);
}

inline int calc_E(my_double num)
{
	return is_denomalized(num) ? 1 - Bias : calc_exponent(num) - Bias;
}

inline unsigned long long calc_M(my_double num)
{
	return calc_fraction(num) + ((unsigned long long) is_nomalized(num) << BIT_OF_M);
}

inline bool is_denomalized(my_double num)
{
	return calc_exponent(num) == 0;
}

inline bool is_nomalized(my_double num)
{
	return !is_denomalized(num) && !is_special(num);
}

inline bool is_special(my_double num)
{
	return calc_exponent(num) == EXP_MAX;
}

inline bool is_zero(my_double num)
{
	return is_denomalized(num) && calc_fraction(num) == 0;
}

inline bool is_infinity(my_double num)
{
	return is_special(num) && calc_fraction(num) == 0;
}

inline bool is_nan(my_double num)
{
	return is_special(num) && calc_fraction(num) != 0;
}

inline my_double gen_my_double(bool sign, int exp, unsigned long long frac)
{
	my_double res;
	set_sign(&res, sign);
	set_exponent(&res, exp);
	set_fraction(&res, frac);
	return res;
}

inline my_double gen_nan()
{
	return gen_my_double(1, EXP_MAX, (1ull << BIT_OF_M) - 1);
}

inline my_double gen_zero()
{
	return gen_my_double(0, 0, 0);
}

inline my_double gen_infinity()
{
	return gen_my_double(0, EXP_MAX, 0);
}

inline my_double abs_my_double(my_double num)
{
	return gen_my_double(0, calc_exponent(num), calc_fraction(num));
}

inline bool cmp_abs(my_double a, my_double b)
{
	return calc_exponent(a) == calc_exponent(b) ? calc_fraction(a) > calc_fraction(b) \
		: calc_exponent(a) > calc_exponent(b);
}

inline void swap(my_double *a, my_double *b)
{
	my_double tmp = *a;
	*a = *b, *b = tmp;
}

inline __int128 rounding(__int128 num, int shft)
{
	if (shft > MOVABLE_BITS_OF_INT128)
		return 0;

	if (shft == 0)
		return num;
	if (shft == 1)
	{
		if ((num & 1) == 0) return num >> 1;
		if ((num >> 1) & 1) return (num >> 1) + 1;
	}

	if (((num >> (shft - 1)) & 1) == 0)
		return num >> shft;

	__int128 res;
	__int128 litter = num & ((((__int128)1) << (shft - 1)) - 1);

	if (litter)
		res = (num >> shft) + 1;
	else
		((num >> shft) & 1) ? (res = (num >> shft) + 1) : (res = (num >> shft));

	return res;
}

my_double adjust(int E, __int128 M)
{
	my_double res;

	int i;
	for (i = TOP_BIT_OF_INT128; ~i; --i)
		if ((M >> i) & 1)
			break;

	int expected_E = E + (i - BIT_OF_M);

	if (expected_E > E_MAX)
		return gen_infinity();
	if (expected_E < E_MIN)
	{
		if (i < BIT_OF_M)
			M <<= (BIT_OF_M - i), E -= (BIT_OF_M - i);

		i = BIT_OF_M;
		int shft = E_MIN - E;
		E = -Bias;
		M = rounding(M, shft);
	}

	if (i > BIT_OF_M)
	{
		M = rounding(M, i - BIT_OF_M), E += (i - BIT_OF_M);
		if (M == (((__int128)1) << (BIT_OF_M + 1)))
			++E;
	}
	if (i < BIT_OF_M)
		M <<= (BIT_OF_M - i), E -= (BIT_OF_M - i);

	set_exponent(&res, E + Bias);
	set_fraction(&res, (unsigned long long) M);

	return res;
}

my_double minus(my_double a)
{
	set_sign(&a, calc_sign_bit(a) ^ 1);
	return a;
}

inline my_double calc_add(my_double a, my_double b)
{
	my_double res;

	if (is_zero(a))
		return b;
	if (is_zero(b))
		return a;

	if (is_infinity(a))
	{
		if (is_infinity(b))
		{
			if (calc_sign(a) * calc_sign(b) == 1) return a;
			return gen_nan();
		}
		return a;
	}
	if (is_infinity(b))
		return b;

	if (is_nan(a))
		return a;
	if (is_nan(b))
		return b;

	if (cmp_abs(a, b) == LESS)
		swap(&a, &b);

	bool sign = calc_sign_bit(a);
	int sign_a = calc_sign(a), sign_b = calc_sign(b);
	a = abs_my_double(a), b = abs_my_double(b);

	if (sign_a * sign_b == 1)
	{
		int E, E_a = calc_E(a), E_b = calc_E(b);
		__int128 M, M_a = calc_M(a), M_b = calc_M(b);

		M_a <<= BITS_LEFT_FOR_ADD_ROUNDING,
			M_b <<= BITS_LEFT_FOR_ADD_ROUNDING;
		int shft = E_a - E_b;

		if (shft)
			M_b >>= min(shft, MOVABLE_BITS_OF_INT128);

		M = M_a + M_b;
		E = E_a - BITS_LEFT_FOR_ADD_ROUNDING;
		res = adjust(E, M);
	}
	else
	{
		int E, E_a = calc_E(a), E_b = calc_E(b);
		__int128 M, M_a = calc_M(a), M_b = calc_M(b);
		M_a <<= BITS_LEFT_FOR_ADD_ROUNDING,
			M_b <<= BITS_LEFT_FOR_ADD_ROUNDING;
		int shft = E_a - E_b;

		if (shft)
			M_b >>= min(shft, MOVABLE_BITS_OF_INT128);

		M = M_a - M_b;
		E = E_a - BITS_LEFT_FOR_ADD_ROUNDING;
		res = adjust(E, M);
	}

	set_sign(&res, sign);
	return res;
}

inline my_double calc_subtract(my_double a, my_double b)
{
	return calc_add(a, minus(b));
}

inline my_double calc_multiply(my_double a, my_double b)
{
	my_double res;

	if (is_nan(a) || is_nan(b))
		return gen_nan();

	if (is_zero(b))
		swap(&a, &b);
	if (is_zero(a))
	{
		if (is_infinity(b))
			return gen_nan();

		res = gen_zero();
		set_sign(&res, calc_sign_bit(a) ^ calc_sign_bit(b));
		return res;
	}

	if (is_infinity(a) || is_infinity(b))
	{
		res = gen_infinity();
		set_sign(&res, calc_sign_bit(a) ^ calc_sign_bit(b));
	}

	bool sign = calc_sign_bit(a) ^ calc_sign_bit(b);
	int E, E_a = calc_E(a), E_b = calc_E(b);
	__int128 M, M_a = calc_M(a), M_b = calc_M(b);

	E = E_a + E_b - BITS_SURPLUS_IN_MULTIPLICATION;
	M = M_a * M_b;

	res = adjust(E, M);
	set_sign(&res, sign);

	return res;
}

inline my_double calc_divide(my_double a, my_double b)
{
	my_double res;

	if (is_nan(a) || is_nan(b))
		return gen_nan();

	if (is_zero(b))
	{
		if (is_zero(a))
			return gen_nan();

		res = gen_infinity();
		set_sign(&res, calc_sign_bit(a) ^ calc_sign_bit(b));
		return res;
	}

	if (is_zero(a))
	{
		res = gen_zero();
		set_sign(&res, calc_sign_bit(a) ^ calc_sign_bit(b));
		return res;
	}

	if (is_infinity(a))
	{
		if (is_infinity(b))
			return gen_nan();

		res = gen_infinity();
		set_sign(&res, calc_sign_bit(a) ^ calc_sign_bit(b));
		return res;
	}

	if (is_infinity(b))
	{
		res = gen_zero();
		set_sign(&res, calc_sign_bit(a) ^ calc_sign_bit(b));
		return res;
	}

	bool sign = calc_sign_bit(a) ^ calc_sign_bit(b);
	int E, E_a = calc_E(a), E_b = calc_E(b);
	__int128 M, M_a = calc_M(a), M_b = calc_M(b), standard;

	standard = ((__int128)1) << (BITS_ABSENT_IN_DIVISION + \
		BITS_ABSENT_IN_DIVISION + A_PIECE_OF_BITS_LEFT_FOR_DIVIDE_ROUNDING);
	M_a <<= BITS_ABSENT_IN_DIVISION;
	int pieces = 0;
	while (M_a < standard)
		pieces++, M_a <<= A_PIECE_OF_BITS_LEFT_FOR_DIVIDE_ROUNDING;

	E = E_a - E_b - pieces * A_PIECE_OF_BITS_LEFT_FOR_DIVIDE_ROUNDING;
	M = M_a / M_b;

	if (M * M_b < M_a) M |= 1;

	res = adjust(E, M);
	set_sign(&res, sign);

	return res;
}

uint64_t my_calc_function(uint64_t a, uint64_t b, char op)
{
	static bool initialized = false;
	if (!initialized)
	{
		initialize();
		initialized = true;
	}

	my_double x = convert_to_my_double(a),
		y = convert_to_my_double(b),
		res;
	switch (op)
	{
	case '+':
		res = calc_add(x, y);
		break;
	case '-':
		res = calc_subtract(x, y);
		break;
	case '*':
		res = calc_multiply(x, y);
		break;
	case '/':
		res = calc_divide(x, y);
		break;
	}

	return convert_from_my_double(res);
}

void initialize()
{
}

