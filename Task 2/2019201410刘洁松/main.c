#include <stdio.h>
#include <string.h>
#include "double.c"

#define MAX_LENGTH 10000000
#define SHIFT_BIT 60

// operant 1: + 2: - 3: * 4: / 0: number

struct double_t st[MAX_LENGTH];
struct double_t expression[MAX_LENGTH];

char number[MAX_LENGTH];

struct double_t int_to_double(int operant) {
	if(!operant) return cast_to_double(1, 0, 0);
	int result_exp = BIAS + LENGTH_OF_FRAC;
	unsigned long long result_frac = operant;
	result_frac = Rounding(&result_exp, result_frac, 0);
	return cast_to_double(1, result_exp, result_frac);
}

struct double_t read_number(int sig) {
	struct double_t result;
	init(&result);
	struct double_t base = int_to_double(10);
	struct double_t pw = int_to_double(1);
	char c;
	int pos = 0;
	c = getchar();
	while((c >= '0' && c <= '9') || c == '.') {
		number[++pos] = c;
		c = getchar();
	}
	ungetc(c, stdin);

	int is_frac = 0;
	for(int i = 1; i <= pos; i++) {
		if(number[i] == '.') {
			is_frac = 1;
			pw = divide(pw, base);
		} else if(!is_frac) {
			result = multiply(result, base);
			result = add(result, int_to_double(number[i] - '0'));
		} else {
			result = add(result, multiply(int_to_double(number[i] - '0'), pw));
			pw = divide(pw, base);
		}
	}
	if(sig < 0) result.bytes[ MSB ] ^= (1 << MSB);
	return result;
}

int get_prior(char c) {
	if(c == '+' || c == '-') return 1;
	if(c == '*' || c == '/') return 2;
	return 0;
}

bool is_digit(char c) {
	return c >= '0' && c <= '9';
}
bool is_operator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

int read_expression() {
	char c;
	int last_operator = 1, length = 0;
	int top = 0;
	for(c = getchar(); ; c = getchar()) {
		if(c == '\n') break; 
		if(c == ' ') continue;
		if(is_digit(c)) {
			int sig = 1;
			if(is_digit(c)) ungetc(c, stdin);

			expression[++length] = read_number(sig);
			expression[length].operator = 0;

		} else {
			if(c == '(') st[++top].operator = c;
			else if(c == ')') {
				while(top && st[top].operator != '(') {
					expression[++length] = st[top--];
				}
				top--;
			} else if(!top || st[top].operator == '(') {
				st[++top].operator = c;
			} else if(c == '+' || c == '-' || c == '*' || c == '/') {
				while(top && get_prior(st[top].operator) >= get_prior(c)) {
					expression[++length] = st[top];
					top --;
				}
				st[++top].operator = c;
			} 
		}
	}
	while(top) expression[++length] = st[top--];
	return length;
}

struct double_t calc_expression(int length) {
	struct double_t result;
	init(&result);
	int top = 0;
	for(int i = 1; i <= length; i++) {
		if(expression[i].operator == 0) st[++top] = expression[i];
		else {
			struct double_t operant_2 = st[top--];
			struct double_t operant_1 = st[top--];

			if(expression[i].operator == ADD) st[++top] = add(operant_1, operant_2);
			else if(expression[i].operator == SUBTRACT) st[++top] = subtract(operant_1, operant_2);
			else if(expression[i].operator == MULTIPLY) st[++top] = multiply(operant_1, operant_2);
			else if(expression[i].operator == DIVIDE) st[++top] = divide(operant_1, operant_2);
		}
	}

	result = st[top];
	return result;
}

int bit[MAX_LENGTH];

void printf_bit(int length) {
	for(int i = length; i >= 1; i--) 
		printf("%d", bit[i]);
	puts("");
}

void print_expression(struct double_t operant) {
	int operant_sig = extract_sig(operant);
	int operant_exp = extract_exp(operant) - LENGTH_OF_FRAC - BIAS;
	unsigned long long operant_frac = extract_frac(operant);

	if(operant_sig < 0) {
		putchar('-');
		fflush(stdout);
	}

	int operant_type = get_type(operant);
	if(operant_type == NAN) {
		printf("nan\n");
		fflush(stdout);
		return;
	}
	if(operant_type == INF) {
		printf("inf\n");
		fflush(stdout);
		return;
	}
	
	if(get_type(operant) != DENORMAL) operant_frac |= (1ull << LENGTH_OF_FRAC);
	else operant_frac <<= 1;

	int bit_length = 0;
	while(operant_frac) {
		bit[ ++bit_length ] = operant_frac % 10;
		operant_frac /= 10;
	}

	
	for(int i = 0; i < operant_exp; i++) {
		for(int j = 1; j <= bit_length; j++)
			bit[j] <<= 1;
		for(int j = 1; j <= bit_length; j++) {
			bit[j+1] += (bit[j] / 10);
			bit[j] %= 10;
		}
		
		if(bit[ bit_length+1 ]) bit_length ++;
	}


	if(operant_exp >= 0) {
		for(int i = bit_length; i >= 1; i--) {
			putchar(bit[i] + '0');
			fflush(stdout);
		}
	} else {
		for(int i = bit_length + SHIFT_BIT; i > SHIFT_BIT; i --) {
			bit[i] = bit[i - SHIFT_BIT];
		}
		for(int i = 1; i <= bit_length; i++) bit[i] = 0;
		bit_length += SHIFT_BIT;

		operant_exp *= -1;
		for(int i = 1; i <=operant_exp; i++) {
			for(int j = bit_length; j >= 1; j--) {
				if(j != 1) {
					bit[j-1] += (bit[j] % 2) * 10;
				}
				bit[j] >>= 1;
			}
			if(bit_length && !bit[bit_length]) bit_length --;
		}

		for(int i = bit_length; i > SHIFT_BIT; i--) {
			putchar(bit[i] + '0');
			fflush(stdout);
		}
		if(bit_length <= SHIFT_BIT) {
			putchar('0');
			fflush(stdout);
		}
		putchar('.');
		fflush(stdout);
		for(int i = SHIFT_BIT; i > 0; i--) {
			putchar(bit[i] + '0');
			fflush(stdout);
		}
	}

	putchar('\n');
	fflush(stdout);

	for(int i = 0; i <= bit_length; i++) {
		bit[i] = 0;
	}

}

int main() {

	char c;
	int last_operator = 0;
	while(1) {
		c = getchar();
		if(c == EOF) break; 
		ungetc(c, stdin);
		int length = read_expression();
		struct double_t result = calc_expression(length);
		print_expression(result);
	}
	return 0;
}
