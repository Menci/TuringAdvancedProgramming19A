#include "my_double.c"

#define MAX_NUM_OF_CHARS 10000000
#define MAX_BITS_OF_ANS 1000
#define DECIMAL_BITS_OF_FRAC 50

enum STATE
{
	NUM,OPT
};

struct ANS
{
	int b[MAX_BITS_OF_ANS];
}ans;

int len_of_expression;
char expression[MAX_NUM_OF_CHARS];
bool is_opt[MAX_NUM_OF_CHARS];

int top;
char stk[MAX_NUM_OF_CHARS];

//reverse polish notation
int len;
my_double RPN[MAX_NUM_OF_CHARS];

int aim;

void solve();
void terminate();

void input_and_evaluate();
my_double calculate();
void output_my_double(my_double);

bool is_open_paren(char);
bool is_close_paren(char);
bool is_parenthesis(char);
int priority(char);

my_double input_num();

void push_opt_into_RPN(char);
void push_num_into_RPN(my_double num);

my_double calc(my_double, my_double, my_double);

int main()
{
//	freopen("data.in", "r", stdin);
	register int i;
	register char ch;
	while ((ch = getchar()) != EOF)
	{
		i = 0;
		ungetc(ch, stdin);
		while((ch = getchar()) != '\n' && ch != '\r')
			if(ch != ' ')
				expression[++i] = ch;
		len_of_expression = strlen(expression + 1);
		solve();
		terminate();
	}
}

void solve()
{
	input_and_evaluate(),
	output_my_double(calculate());
}

inline void terminate()
{
	len = 0;
	register int i;
	for(i = 1; i <= len_of_expression; ++i)
		expression[i] = '\0', is_opt[i] = 0;
}

void input_and_evaluate()
{
	aim = 0;
	register int pos = 0;
	enum STATE state = NUM;
	while(pos < len_of_expression)
	{
		aim = pos + 1;
		if(is_parenthesis(expression[aim]))
		{
			if(is_open_paren(expression[aim]))
				stk[++top] = expression[aim];

			if(is_close_paren(expression[aim]))
			{
				while (!is_open_paren(stk[top]))
					push_opt_into_RPN(stk[top--]);
				top--;
			}
		}
		else
		{
			switch(state)
			{
				case NUM:
					push_num_into_RPN(input_num());
					break;

				case OPT:
					if(priority(expression[aim]) > priority(stk[top]))
						stk[++top] = expression[aim];
					else
					{
						while (top && priority(stk[top]) >= priority(expression[aim]))
							push_opt_into_RPN(stk[top--]);
						push_opt_into_RPN(expression[aim]);
					}
					break;
			}
			state ^= 1;
		}
		pos = aim;
	}

	while(top)
		push_opt_into_RPN(stk[top--]);
}

my_double calculate()
{
	top = 0;
	register int i;
	for(i = 1; i <= len; ++i)
	{
		if(is_opt[i])
			RPN[top-1] = calc(RPN[top - 1], RPN[top], RPN[i]),
			--top;
		else
			RPN[++top] = RPN[i];
	}
	top = 0;
	return RPN[1];
}

inline void output_my_double(my_double num)
{
//	printf("%.50lf\n", decode_into_double(num));
//	fflush(stdout);
	if(is_nan(num))
	{
		printf("nan\n");
		fflush(stdout);
		return;
	}

	if(calc_sign_bit(num))
	{
		putchar('-');
		fflush(stdout);
	}

	if(is_infinity(num))
	{
		printf("inf\n");
		fflush(stdout);
		return;
	}

	if(is_zero(num))
	{
		putchar('0');
		fflush(stdout);
		putchar('\n');
		fflush(stdout);
		return;
	}

	int E = calc_E(num) - BIT_OF_M;
	__int128 M = calc_M(num);
	ans.b[0] = 0;
	while(M)
		ans.b[++ans.b[0]] = M % 10,
		M /= 10;
	register int i, j;
	for(i = 1; i <= E; ++i)
	{
		for(j = 1; j <= ans.b[0]; ++j)
			ans.b[j] <<= 1;
		for(j = 1; j <= ans.b[0]; ++j)
		{
			ans.b[j + 1] += ans.b[j] / 10;
			ans.b[j] %= 10;
		}
		if(ans.b[ans.b[0] + 1])
			++ans.b[0];
	}

	if(E >= 0)
		for(i = ans.b[0]; i; --i)
		{
			putchar(ans.b[i] + '0');
			fflush(stdout);
		}
	else
	{
		for(i = ans.b[0] + DECIMAL_BITS_OF_FRAC; i > DECIMAL_BITS_OF_FRAC; --i)
			ans.b[i] = ans.b[i - DECIMAL_BITS_OF_FRAC];
		ans.b[0] += DECIMAL_BITS_OF_FRAC;
		for(i = 1; i <= DECIMAL_BITS_OF_FRAC; ++i)
			ans.b[i] = 0;

		for(i = 0; i > E; --i)
		{
			for(j = 1; j <= ans.b[0]; ++j)
			{
				if(j != 1)
					ans.b[j - 1] += ans.b[j] % 2 * 5;
				ans.b[j] >>= 1;
			}
			if(ans.b[0] - 1 && !ans.b[ans.b[0]])
				--ans.b[0];
		}

		for(i = ans.b[0]; i > DECIMAL_BITS_OF_FRAC; --i)
		{
			putchar(ans.b[i] + '0');
			fflush(stdout);
		}
		if(ans.b[0] <= DECIMAL_BITS_OF_FRAC)
		{
			putchar('0');
			fflush(stdout);
		}
		putchar('.');
		fflush(stdout);
		for(i = DECIMAL_BITS_OF_FRAC; i; --i)
		{
			putchar(ans.b[i] + '0');
			fflush(stdout);
		}
	}
	putchar('\n');
	fflush(stdout);

	while(ans.b[0])
		ans.b[ans.b[0]--] = 0;
}

inline bool is_open_paren(char ch)
{
	return ch == '(';
}

inline bool is_close_paren(char ch)
{
	return ch == ')';
}

inline bool is_parenthesis(char ch)
{
	return is_open_paren(ch) || is_close_paren(ch);
}

inline int priority(char ch)
{
	int res;
	switch(ch)
	{
		case '*':
		case '/':
			res = 2;
			break;
		case '+':
		case '-':
			res = 1;
			break;
		default:
			res = 0;
	}
	return res;
}

inline my_double input_num()
{
	my_double res = gen_zero();
	my_double bas = encode_uint_into_my_double(10);
	my_double pw = encode_uint_into_my_double(1);
	bool sign_flag = 0, frac_flag = 0;
	if(priority(expression[aim]))
	{
		if(expression[aim] == '-')
			sign_flag = 1;
		++aim;
	}

	while(isdigit(expression[aim]) || expression[aim] == '.')
	{
		if(expression[aim] == '.')
			frac_flag = 1,
			pw = calc_divide(pw, bas);
		else if(!frac_flag)
			res = calc_add(calc_multiply(res, bas), \
			encode_uint_into_my_double(expression[aim] - '0'));
		else
			res = calc_add(res, \
			calc_multiply(encode_uint_into_my_double(expression[aim] - '0'), \
			pw)),
			pw = calc_divide(pw, bas);
		++aim;
	}
	--aim;

	double tmp = decode_into_double(res);
	if(sign_flag)
		set_sign(&res, 1);
	return res;
}

inline void push_opt_into_RPN(char ch)
{
	is_opt[++len] = 1;
	switch(ch)
	{
		case '+':
			RPN[len] = gen_my_double(0 , 0, 1);
			break;
		case '-':
			RPN[len] = gen_my_double(0 , 0, 2);
			break;
		case '*':
			RPN[len] = gen_my_double(0 , 0, 3);
			break;
		case '/':
			RPN[len] = gen_my_double(0 , 0, 4);
			break;
	}
}

inline void push_num_into_RPN(my_double num)
{
	RPN[++len] = num;
}

inline my_double calc(my_double a, my_double b, my_double opt)
{
	my_double res;
	switch(calc_fraction(opt))
	{
		case 1:
			res = calc_add(a,b);
			break;
		case 2:
			res = calc_subtract(a,b);
			break;
		case 3:
			res = calc_multiply(a,b);
			break;
		case 4:
			res = calc_divide(a,b);
			break;
	}
	return res;
}

