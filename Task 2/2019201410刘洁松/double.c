#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define LENGTH_OF_BYTES 8
#define LENGTH_OF_BIT 8
#define LENGTH_OF_SIG 1
#define LENGTH_OF_EXP 11
#define LENGTH_OF_FRAC 52
#define LIM_OF_EXP 60
#define LIM_OF_FRAC 120
#define LIM_OF_SHIFT 120

#define BEGIN_BYTE_EXP 6
#define BEGIN_BIT_EXP 5
#define BEGIN_BYTE_FRAC 0
#define BEGIN_BIT_FRAC 1

#define SIZE_OF_DEFAULT 15
#define BIAS 1023
#define MAX_EXP 2047
#define MSB 7
#define DIVIDE_BIAS 128
#define LENGTH_OF_BIG 126

#define EXP_MAX 1023
#define EXP_MIN -1022

#define BEGIN_EXP 5
#define END_EXP 7
#define END_FRAC 4

#define big_int __uint128_t

#define NAN 1
#define INF 2
#define NORMAL 3
#define DENORMAL 4
#define ZERO 0

#define ADD '+'
#define SUBTRACT '-'
#define MULTIPLY '*'
#define DIVIDE '/'

#define swap(a, b) (a) ^= (b) ^= (a) ^= (b)

struct double_t {
	unsigned char bytes[LENGTH_OF_BYTES];
	int operator;
};

void init(struct double_t *operant) {
	for(int i = 0; i < LENGTH_OF_BYTES; i++) (*operant). bytes[i] = 0;
}

// operator

struct double_t add( struct double_t operant_1, struct double_t operant_2);
struct double_t subtract(struct double_t operant_1, struct double_t operant_2);
struct double_t multiply(struct double_t operant_1, struct double_t operant_2);
struct double_t divide(struct double_t operant_1, struct double_t operant_2);

// the operation I/O begins

struct double_t read() {
	struct double_t input;
	scanf("%lf", &input);
	return input;
}

void print_double(struct double_t output) {
	//printf("%llu\n", *(unsigned long long*)&output);
	printf("%.60le\n", *(double*)&output);
}

// the operation of I/O ends

// moving current position begins

void get_next_bit(int *current_byte, int *current_bit) {
	if((*current_bit) == LENGTH_OF_BIT) (*current_byte) = (*current_byte) + 1, (*current_bit) = 1;
	else (*current_bit) = (*current_bit) + 1;
}

// moving current position ends

// cast to double begins

struct double_t cast_to_double(int sig, int exp, unsigned long long frac) {
	int current_byte = BEGIN_BYTE_FRAC, current_bit = BEGIN_BIT_FRAC;
	current_byte = 0, current_bit = 1;

	struct double_t return_double;
	init(&return_double);

	for(int i = 0; i < LENGTH_OF_FRAC; i++) {
		if(frac & 1ull) return_double.bytes[current_byte] |= (1 << (current_bit - 1) );
		frac >>= 1ull;
		get_next_bit(&current_byte, &current_bit);
	}

	for(int i = 0; i < LENGTH_OF_EXP; i++) {
		if(exp & 1) return_double.bytes[current_byte] |= 1 << (current_bit - 1);
		exp >>= 1;
		get_next_bit(&current_byte, &current_bit);
	}

	if(sig < 0) return_double.bytes[current_byte] |= 1 << (LENGTH_OF_BIT - 1);

	return return_double;
}

// cast to double ends

// extracting each section begins

int extract_sig(struct double_t operant) {
	return (operant.bytes[ MSB ] >> MSB) ? -1 : 1;
}

int extract_exp(struct double_t operant) {
	int exp = 0;

	int current_byte = BEGIN_BYTE_EXP, current_bit = BEGIN_BIT_EXP;

	for(int i = 0; i < LENGTH_OF_EXP; i++) {
		if( operant.bytes[current_byte] & (1 << (current_bit-1)) ) exp |= 1<<i;
		get_next_bit(&current_byte, &current_bit);
	}

	return exp;
}

unsigned long long extract_frac(struct double_t operant) {
	unsigned long long frac = 0;

	int current_byte = BEGIN_BYTE_FRAC, current_bit = BEGIN_BIT_FRAC;

	for(int i = 0; i < LENGTH_OF_FRAC; i++) {
		if( operant.bytes[current_byte]  & (1<< (current_bit-1)) ) frac |= 1ull<<i;
		get_next_bit(&current_byte, &current_bit);
	}

	return frac;
}

// extracting each section ends

// classify type begins

int get_type(struct double_t operant) {
	int exp = extract_exp(operant);
	unsigned long long frac = extract_frac(operant);
	if( (exp == (1 << LENGTH_OF_EXP) - 1) && frac) return 1;
	if( (exp == (1 << LENGTH_OF_EXP) - 1) && !frac) return 2;
	if(exp > 0 && ( exp < (1 << LENGTH_OF_EXP) - 1 )) return 3;
	if(!exp && frac) return 4;
	if(!exp && !frac) return 0;
}

// classify type ends

// rounding begins

void print(big_int operant) {
	while(operant) {
		printf("%d", operant & 1);
		operant >>= 1;
	}
	puts("");
}

int calc_bit(big_int operant) {
	int cnt = 0;
	while(operant) {
		cnt ++;
		operant >>= 1;
	}
	return cnt;
}

unsigned long long rounding(int* result_exp, big_int operant, int length) { // range [0 - length]
	big_int operant_init = operant; // for case of subtraction in which exp->0 and bring about denormalized number
	while(!(operant >> length)) {
		length --;
		(*result_exp) = (*result_exp) -1;
		if( !(*result_exp) ) {
			if(calc_bit(operant_init) == LENGTH_OF_FRAC + 1) operant_init >>= 1;
			return operant_init;
		}
		operant_init <<= 1;
	}
	while(operant >> (length + 1)) {
		length ++;
		(*result_exp) = (*result_exp) + 1;
	}

	int LSB = length - LENGTH_OF_FRAC;

	if((*result_exp) >= -LIM_OF_FRAC && (*result_exp) < 0 && ( operant >> (-(*result_exp)) ) ) {
		int denormalized_diff = LENGTH_OF_FRAC + (*result_exp);

		for(; length < LENGTH_OF_FRAC - 1; length ++)
			operant <<= 1;

		operant >>= -(*result_exp);
		(*result_exp) = 0;
		LSB = calc_bit(operant) - denormalized_diff;
	}

	if(LSB < 0) { // There are less than LENGTH_OF_FRAC bit in the result
		int shift = LENGTH_OF_FRAC - length;
		operant <<= shift; 
		return (unsigned long long) (operant % (1ull << LENGTH_OF_FRAC) );
	}
	if(LSB == 0) {
		if( (!(*result_exp)) && (operant >> LENGTH_OF_FRAC) )
			operant >>= 1;
		return (unsigned long long) (operant % (1ull << LENGTH_OF_FRAC) );
	}

	bool last_bit = operant & (1ull << (LSB) );
	bool guard_bit = operant & (1ull << (LSB - 1) );
	bool sticky_bit = 0;

	for(int i = 0; i < LSB - 1; i++) {
		sticky_bit |= (operant >> i) & 1;
	}

	int operation_bit = guard_bit & (last_bit | sticky_bit);
	operant >>= LSB;
	if(operation_bit) operant ++;
	
	return (unsigned long long) (operant % (1ull << LENGTH_OF_FRAC) );
}


big_int shift_right(big_int operant, int operant_shift) {
	if(operant_shift > LENGTH_OF_BIG) 
		return 0;
	if(operant_shift == 0) 
		return operant;
	if(operant_shift == 1) {
		if(operant_shift & 1) 
			return (operant >> 1) + 1;
		return operant >> 1;
	}

	if( (operant >> (operant_shift - 1)) == 0) 
		return operant >> operant_shift;

	bool last_bit = (operant >> operant_shift) & 1;
	bool guard_bit = ( operant >> (operant_shift - 1) ) & 1;
	bool sticky_bit = (operant & ( ((big_int)1 << (operant_shift-1)) - 1 ));
	int operation_bit = guard_bit & (last_bit | sticky_bit);
	operant >>= operant_shift;
	if(operation_bit) operant ++;
	return operant;
}

unsigned long long Rounding(int* exp, big_int operant, int length) { // rounding for multiplication and division
	(*exp) = (*exp) - BIAS;
	for(length = LENGTH_OF_BIG; length >= 0; length--)
		if( (operant >> length) & 1 ) break;
	int result_exp = (*exp) + (length - LENGTH_OF_FRAC);

	if(result_exp > EXP_MAX) {
		(*exp) = (1ull << LENGTH_OF_EXP) - 1;
		return 0;
	}

	if(result_exp < EXP_MIN) {
		if(length < LENGTH_OF_FRAC) {
			operant <<= ( LENGTH_OF_FRAC - length );
			(*exp) -= LENGTH_OF_FRAC - length;
		}
		length = LENGTH_OF_FRAC;
		int operant_shift = EXP_MIN - (*exp);
		(*exp) = -BIAS;
		operant = shift_right(operant, operant_shift);
	}

	if(length > LENGTH_OF_FRAC) {
		operant = shift_right(operant, length - LENGTH_OF_FRAC);
		(*exp) = (*exp) + length - LENGTH_OF_FRAC;
	}
	if(length < LENGTH_OF_FRAC) {
		operant <<= ( LENGTH_OF_FRAC - length);
		(*exp) = (*exp) - (LENGTH_OF_FRAC - length);
	}

	(*exp) = (*exp) + BIAS;

	return (unsigned long long) (operant % (1ull << LENGTH_OF_FRAC) );
}

// rounding ends

// operation begins

struct double_t multiply(struct double_t operant_1, struct double_t operant_2) {


	int operant_1_type = get_type(operant_1), operant_2_type = get_type(operant_2);
	if(operant_1_type == NAN) return operant_1;
	if(operant_2_type == NAN) return operant_2;
	int result_sig = extract_sig(operant_1) * extract_sig(operant_2);
	int result_exp = extract_exp(operant_1) + extract_exp(operant_2) - BIAS - LENGTH_OF_FRAC;
	unsigned long long result_frac = 0, operant_1_frac = extract_frac(operant_1), operant_2_frac = extract_frac(operant_2);
	
	if(operant_1_type == 1) return operant_1;
	if(operant_2_type == 1) return operant_2;
	if(operant_1_type == 2 && operant_2_type == 2) {
		return cast_to_double(result_sig, (1 << LENGTH_OF_EXP) - 1, 0);
	}

	if(operant_1_type != DENORMAL)
		operant_1_frac |= (1ull << LENGTH_OF_FRAC);
	else operant_1_frac <<= 1;

	if(operant_2_type != DENORMAL)
		operant_2_frac |= (1ull << LENGTH_OF_FRAC);
	else operant_2_frac <<= 1;

	result_frac = Rounding(&result_exp, ( (big_int)operant_1_frac ) * operant_2_frac, 2 * (LENGTH_OF_FRAC));
	if(result_exp < 0) {
		while(result_exp < 0 && ( result_frac >> 1 ) ) {
			result_frac >>= 1;
			result_exp ++;
		}
		if(result_exp < 0) {
			result_exp = 0;
			result_frac = 0;
		}
	}
	if(result_exp >= (1 << LENGTH_OF_EXP) - 1) {
		result_exp = (1 << LENGTH_OF_EXP) - 1;
		result_frac = 0;
	}

	return cast_to_double(result_sig, result_exp, result_frac);
}

struct double_t add_same_sig(struct double_t operant_1, struct double_t operant_2) {
	int operant_1_exp = extract_exp(operant_1), operant_2_exp = extract_exp(operant_2);
	int operant_1_type = get_type(operant_1), operant_2_type = get_type(operant_2);
	unsigned long long operant_1_frac = extract_frac(operant_1), operant_2_frac = extract_frac(operant_2);

	if(operant_1_exp < operant_2_exp) {
		swap(operant_1_type, operant_2_type);
		swap(operant_1_exp, operant_2_exp);
		swap(operant_1_frac, operant_2_frac);
	}

	int exp_diff = operant_1_exp - operant_2_exp;

	if(exp_diff > LIM_OF_EXP) {
		return cast_to_double(extract_sig(operant_1), operant_1_exp, operant_1_frac);
	}

	if(operant_1_type == DENORMAL) {
		operant_1_frac <<= 1;
	}
	if(operant_2_type == DENORMAL) {
		operant_2_frac <<= 1;
	}
	if(operant_1_type != DENORMAL) 
		operant_1_frac += (1ull << LENGTH_OF_FRAC);
	if(operant_2_type != DENORMAL) 
		operant_2_frac += (1ull << LENGTH_OF_FRAC);

	int result_sig = extract_sig(operant_1), result_exp = operant_1_exp;
	unsigned long long result_frac;
	result_frac = rounding(&result_exp, ( (big_int)operant_1_frac << exp_diff ) + operant_2_frac, LENGTH_OF_FRAC + exp_diff);
	if(result_exp >= (1 << LENGTH_OF_EXP) - 1) {
		result_exp = (1 << LENGTH_OF_EXP) - 1;
		result_frac = 0;
	}

	return cast_to_double(result_sig, result_exp, result_frac);
}

struct double_t add_diff_sig(struct double_t operant_1, struct double_t operant_2) {
	int operant_1_type = get_type(operant_1), operant_2_type = get_type(operant_2);
	int operant_1_sig = extract_sig(operant_1), operant_2_sig = extract_sig(operant_2);
	int operant_1_exp = extract_exp(operant_1), operant_2_exp = extract_exp(operant_2);
	unsigned long long operant_1_frac = extract_frac(operant_1), operant_2_frac = extract_frac(operant_2);

	if( ( operant_1_exp < operant_2_exp) || ( operant_1_exp == operant_2_exp && operant_1_frac < operant_2_frac) ) {
		swap(operant_1_type, operant_2_type);
		swap(operant_1_sig, operant_2_sig);
		swap(operant_1_exp, operant_2_exp);
		swap(operant_1_frac, operant_2_frac);
	}

	int exp_diff = operant_1_exp - operant_2_exp;

	if(exp_diff > LIM_OF_EXP) {
		return cast_to_double(operant_1_sig, operant_1_exp, operant_1_frac);
	}

	if(operant_1_type != DENORMAL)
		operant_1_frac += (1ull << LENGTH_OF_FRAC);
	else operant_1_frac <<= 1;

	if(operant_2_type != DENORMAL) 
		operant_2_frac += (1ull << LENGTH_OF_FRAC);
	else operant_2_frac <<= 1;

	int result_sig = operant_1_sig, result_exp = operant_1_exp;
	unsigned long long result_frac;
	result_frac = rounding(&result_exp, ( (big_int)operant_1_frac << exp_diff ) - operant_2_frac, LENGTH_OF_FRAC + exp_diff);

	if(result_exp >= (1 << LENGTH_OF_EXP) - 1) {
		result_exp = (1 << LENGTH_OF_EXP) - 1;
		result_frac = 0;
	}

	return cast_to_double(result_sig, result_exp, result_frac);
}

struct double_t add(struct double_t operant_1, struct double_t operant_2) {
	int operant_1_type = get_type(operant_1), operant_2_type = get_type(operant_2);
	if(operant_1_type == NAN) return operant_1;
	if(operant_2_type == NAN) return operant_2;

	int operant_1_sig = extract_sig(operant_1), operant_2_sig = extract_sig(operant_2);

	if(operant_1_sig == operant_2_sig) return add_same_sig(operant_1, operant_2);
	return add_diff_sig(operant_1, operant_2);
}

struct double_t subtract(struct double_t operant_1, struct double_t operant_2) {
	operant_2.bytes[LENGTH_OF_BYTES - 1] ^= (1 << (LENGTH_OF_BIT - 1));
	return add(operant_1, operant_2);
}

struct double_t divide(struct double_t operant_1, struct double_t operant_2) {
	int operant_1_type = get_type(operant_1), operant_2_type = get_type(operant_2);
	if(operant_1_type == NAN) return operant_1;
	if(operant_2_type == NAN) return operant_2;
	
	int operant_1_sig = extract_sig(operant_1), operant_2_sig = extract_sig(operant_2);
	int operant_1_exp = extract_exp(operant_1), operant_2_exp = extract_exp(operant_2);
	unsigned long long operant_1_frac = extract_frac(operant_1), operant_2_frac = extract_frac(operant_2);
	

	if(operant_1_type != DENORMAL)
		operant_1_frac |= (1ull << LENGTH_OF_FRAC);
	else operant_1_frac <<= 1;

	if(operant_2_type != DENORMAL)
		operant_2_frac |= (1ull << LENGTH_OF_FRAC);
	else operant_2_frac <<= 1;

	int result_sig = operant_1_sig * operant_2_sig;
	unsigned long long result_frac;

	int result_exp = (operant_1_exp - (DIVIDE_BIAS - LENGTH_OF_FRAC - 1)) - operant_2_exp + BIAS + LENGTH_OF_FRAC;

	big_int divident = ( ( (big_int)operant_1_frac ) << (DIVIDE_BIAS - LENGTH_OF_FRAC - 1) );
	big_int quotient = divident / operant_2_frac;
	if(quotient * operant_2_frac < divident) quotient |= 1;

	result_frac = Rounding(&result_exp, quotient, DIVIDE_BIAS - LENGTH_OF_FRAC - 1 );

	if(result_exp < 0) {
		result_exp = 0;
		result_frac = 0;
	}
	if(result_exp >= (1 << LENGTH_OF_EXP) - 1) {
		result_exp = (1 << LENGTH_OF_EXP) - 1;
		result_frac = 0;
	}

	return cast_to_double(result_sig, result_exp, result_frac);
}

uint64_t calc(uint64_t a, uint64_t b, char operator_t);

/*
int main() {

	freopen("1.in", "r", stdin);
	freopen("double_n.out", "w", stdout);

	unsigned long long a, b;
	char c[5];
	scanf("%llx%llx%s", &a, &b, c);
	printf("%llu\n", calc(a, b, c[0]));
	return 0;


	struct double_t operant_1, operant_2, result;
	init(&operant_1), init(&operant_2), init(&result);
	char operator_s[SIZE_OF_DEFAULT], operator_t;

	operant_1 = read();

	int sig = extract_sig(operant_1), exp = extract_exp(operant_1);
	unsigned long long frac = extract_frac(operant_1);

	scanf("%s", operator_s);
	operant_2 = read();
	print_double(operant_1), print_double(operant_2);

	operator_t = operator_s[0];
	if(operator_t == ADD) result = add( operant_1, operant_2);
	else if(operator_t == SUBTRACT) result = subtract(operant_1, operant_2);
	else if(operator_t == MULTIPLY) result = multiply(operant_1, operant_2);
	else if(operator_t == DIVIDE) result = divide(operant_1, operant_2);

	print_double( result );

	return 0;
}
*/

uint64_t convert_to_int(int sig, int exp, unsigned long long frac) {
	return ((1ull*sig) << 63) ^ ((1ull * exp) << 52) ^ frac;
}

#define get_sig(a) ((a) >> 63)
#define get_exp(a) (((a) << 1) >>53)
#define get_frac(a) ((a) & ( (1ull << 52) - 1 ))

uint64_t calc(uint64_t a, uint64_t b, char operator_t) {

	struct double_t operant_1, operant_2, result;
	init(&operant_1), init(&operant_2), init(&result);

	operant_1 = cast_to_double(get_sig(a) ? -1 : 1, get_exp(a), get_frac(a));
	operant_2 = cast_to_double(get_sig(b) ? -1 : 1, get_exp(b), get_frac(b));

	int sig = extract_sig(operant_1), exp = extract_exp(operant_1);
	unsigned long long frac = extract_frac(operant_1);

	if(operator_t == ADD) result = add( operant_1, operant_2);
	else if(operator_t == SUBTRACT) result = subtract(operant_1, operant_2);
	else if(operator_t == MULTIPLY) result = multiply(operant_1, operant_2);
	else if(operator_t == DIVIDE) result = divide(operant_1, operant_2);

	int result_sig = extract_sig(result), result_exp = extract_exp(result);
	unsigned long long result_frac = extract_frac(result);

	return convert_to_int((result_sig < 0) ? 1 : 0, result_exp, result_frac);
}
