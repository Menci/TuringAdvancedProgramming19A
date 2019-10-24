#include "double.c"

#define SIZES 100000
#define SIZE 10000
#define node struct nod
#define bignum struct bnu

struct nod
{
	int ty; 
	doubles d; 
}; 

int las, stktop, optop, dtop; 
node stk[SIZE]; 
char s[SIZES], op[SIZE]; 
doubles dstk[SIZE]; 

char nu[SIZES], bi[SIZES]; 


int is_digit(char c)
{
	return '0' <= c && c <= '9'; 
}

int is_operator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/'; 
}

int is_bracket(char c)
{
	return c == '(' || c == ')';  
}

int is_priority(char c)
{
	return c == '*' || c == '/'; 
}

bool cmp(doubles a, doubles b)
{
	int i; 
	for(i = 7; i >= 0; i--)
		if(a.c[i] != b.c[i])
			return 0; 
	return 1; 
}

doubles qpow(doubles a, int x)
{
	doubles ret = integrate(0, bias, 0); 
	while(x)
	{
		if(x&1)ret = mult(ret, a); 
		a = mult(a, a); x >>= 1; 
	}
	return ret; 
}

doubles read(char * s)
{
	int pos = 0, len = 0, exp10 = 0, i; 
	while(is_digit(s[pos]))
		nu[++len] = s[pos] - '0', pos++; 
	if(s[pos] == '.')
	{
		pos++; 
		while(is_digit(s[pos]))
		{
			nu[++len] = s[pos] - '0'; 
			pos++; exp10++; 
		}
	}
	for(i = 1; i + i <= len; i++)
		swap(nu[i], nu[len - i + 1]); 
	while(len && !nu[len])len--; 
	
	int blen = 0, cur; 
	while(len)
	{
		bi[++blen] = (nu[1]&1); 
		cur = 0; 
		for(i = len; i >= 1; i--)
		{
			cur = cur * 10 + nu[i]; 
			if(cur<2)nu[i] = 0; 
			else
			{	
				nu[i] = cur/2; 
				cur = cur - nu[i] * 2; 
			}
		}
		while(len && nu[len] == 0)
			len--; 
	}

	int exp = bias, bs = 1, rexp; 

	while(blen - bs + 1>65)
		bs++, exp++; 
	
	u128 frac = 0; ull rfrac = 0; 
	for(i = blen; i >= bs; i--)
		frac = (frac<<1)|bi[i]; 
	frac <<= 52; 

	update(&rexp, &rfrac, frac, exp - bias); 
	doubles res = integrate(0, rexp, rfrac); 
	res = divide(res, qpow(integrate(0, bias + 3, (1ll<<50)), exp10)); 
	return res; 
}

void printd(doubles a)
{
	ull frac = cfrac(a); 
	int i, sgn = sign(a), exp = cexps(a) - bias - 52; 
	if(!is_denormalize(a))
		frac += (1ull<<52); 

	memset(nu, 0, sizeof(nu)); 
	int len = 0, dpos = 1; 
	while(frac)
		nu[++len] = frac%10, frac /= 10; 

	while(exp>0)
	{
		for(i = len; i; i--)
		{
			nu[i] *= 2; 
			if(nu[i]>9)
			{
				nu[i + 1] += nu[i]/10; 
				nu[i] %= 10; 
			}
		}
		if(nu[len + 1])len++; 
		exp--; 
	}
	
	nu[0] = 0; 
	while(exp<0)
	{
		if(nu[1]&1)
		{
			len++; 
			for(i = len; i >= 1; i--)
				nu[i] = nu[i - 1]; 
			nu[1] = 0; dpos++; 
		}

		int cur = 0; 
		for(i = len; i >= 1; i--)
		{
			cur = cur * 10 + nu[i]; 
			if(cur<2)nu[i] = 0; 
			else
			{	
				nu[i] = cur/2; 
				cur = cur - nu[i] * 2; 
			}
		}
		while(len && nu[len] == 0)
			len--; 
		exp++; 
	}

	if(sgn)printf("-"); 
	if(dpos == 1)dpos--; 
	if(dpos>len)
	{
		printf("0."); 
		while((--dpos)>len)
			printf("0"); 
		for(i = len; i; i--)
			printf("%d", nu[i]); 
	}
	else
	{
		for(i = len; i; i--)
		{
			printf("%d", nu[i]); 
			if(i == dpos)
				printf("."); 
		}
	}
	printf("\n"); 
}

node makenode(char c)
{
	node ret; 
	ret.ty = (int)c; 
	return ret; 
}

void convert_to_rpn()
{
	int cond = 0, ptr = 1; 
	stktop = optop = 0; 
	while(ptr <= las)	
	{
		if(is_digit(s[ptr]))
		{
			stk[++stktop].d = read(s + ptr); 
			stk[stktop].ty = 0; 

			while(is_digit(s[ptr]))ptr++; 
			if(s[ptr] == '.')++ptr; 
			while(is_digit(s[ptr]))ptr++; 
		}
		else if(is_operator(s[ptr]))
		{
			re:
			if(!optop)
				op[++optop] = s[ptr]; 
			else if(is_priority(s[ptr]) && !is_priority(op[optop]))
				op[++optop] = s[ptr]; 
			else if(op[optop] == '(')
				op[++optop] = s[ptr]; 
			else
			{
				stk[++stktop] = makenode(op[optop--]); 
				goto re; 
			}

			ptr++; 
		}
		else if(is_bracket(s[ptr]))
		{
			if(s[ptr] == '(')
				op[++optop] = s[ptr]; 
			else
			{
				while(!is_bracket(op[optop]))
					stk[++stktop] = makenode(op[optop--]); 
				optop--; 
			}
			ptr++; 
		}
		else ptr++; 
	}

	while(optop)
		stk[++stktop] = makenode(op[optop--]); 
}

void calculate()
{
	int i; 
	for(i = 1; i <= stktop; i++)
	{
		if(stk[i].ty == 0)
			dstk[++dtop] = stk[i].d; 
		else if(is_operator((char)stk[i].ty))
		{
			doubles c = calc(dstk[dtop - 1], dstk[dtop], (char)stk[i].ty); 
			dstk[--dtop] = c; 
		}
	}
}

void work()
{
	char ch; 
	
	while((ch = getchar()) != EOF)
	{
		las = 0; 
		while(ch != '\n')
			s[++las] = ch, ch = getchar(); 
		
		convert_to_rpn(); 
		calculate(); 
		printd(dstk[dtop]); 
		fflush(stdout); 
	}
}

int main()
{
	if(0)
	{
		freopen("in.txt", "r", stdin); 
		freopen("out.txt", "w", stdout); 
	}

	work(); 

	return 0; 
}
