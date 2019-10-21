#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>

#define uchar unsigned char
#define uint unsigned int
#define ull uint64_t
#define u128 __int128_t

#define eps 1e-7
#define ofs 54

#define doubles struct mydouble
#define swap(a, b) (a) ^= (b), (b) ^= (a), (a) ^= (b); 

const uint bias = 1023; 

struct mydouble
{
	uchar c[8]; 
}; 

doubles init()
{
	doubles x; 
	int i; 
	for(i = 0; i < 8; i ++ )
		x.c[i] = 0; 
	return x; 
}

void printb(ull x,int len)
{
	int i;
	for(i = len-1; ~i; i--)
		printf("%d", (x>>i)&1);
	puts("");
}

void printc(uchar x)
{
	int i;
	for(i = 7; ~i; i--)
		printf("%d", (x>>i)&1);
}

void print(doubles x)
{
	int i;
	for(i=7;~i;i--)
		printc(x.c[i]);
	printf("\n");
}

void print2 (doubles v)
{
    double *output_double = &v;
    printf ("%.9f\n", *output_double);
}

void dswap(doubles  * a, doubles  * b)
{
	doubles c =  * a; 
	* a =  * b; 
	* b = c; 
}

int sign(doubles d)
{
	return (d.c[7] >> 7) & 1; 
}

uint cexpb(doubles d)
{
	uint x = 0; 
	int i, bit, id; 
	for(i = 1; i <= 11; i ++ )
	{
		bit = 7 - i%8; 
		id = 7 - i/8; 
		if(d.c[id]&1 << bit)
			x += (1 << (11 - i)); 
	}
	return x; 
}

uint cexps(doubles d)
{
	int x = cexpb(d); 
	if(x == 0)return 1; 
	return x; 
}

ull cfrac(doubles d)
{
	ull x = 0; int i; 
	for(i = 1; i <= 52; i ++ )
	{
		int bit = i + 11; 
		int id = 7 - bit%8; 
		if(d.c[7 - bit/8]&(1 << id))
			x += (1ull << (52 - i)); 
	}
	return x; 
}

int is_inf(doubles x)
{
	return (cexps(x) == ((1 << 11) - 1) && (!cfrac(x)))?1:0; 
}

int is_nan(doubles x)
{
	return (cexps(x) == ((1 << 11) - 1) && cfrac(x))?1:0; 
}

int is_zero(doubles x)
{
	return (!cexpb(x) && !cfrac(x)); 
}

int is_denormalize(doubles d)
{
	return cexpb(d) == 0; 
}

doubles integrate(int sign, int exp, ull frac)
{
	doubles x = init(); int i, bit, id; 
	if(sign)x.c[7] |= 1 << 7; 
	for(i = 11; i >= 1; exp >>= 1, i -- )
		if(exp&1)
			x.c[7 - i/8] |= (1 << (7 - i%8)); 
	for(i = 0; i<6; i ++ , frac >>= 8)
		x.c[i] |= frac&((1 << 8) - 1); 
	for(i = 0; i<4; i ++ , frac >>= 1)
		x.c[6] |= (frac&1) << i; 
	return x; 
}

int ctop(u128 a)
{
	int i; 
	for(i = 127; i >= 0; i -- )
		if((a >> i)&1)
			return i; 
}

u128 rounding(u128 a, int of)
{
	if(of == 0)
		return a; 
	if(of>126)
		return 0; 

	if(of == 1)
	{
		if(!(a&1))
			return a >> 1; 
		if((a >> 1)&1)
			return (a >> 1) + 1; 
	}
	
	if(!(a >> (of - 1)&1))
		return a >> of; 
	if(a&((((u128)1) << (of - 1)) - 1))
		return (a >> of) + 1; 
	else
		return (a >> of) + (a >> of&((u128)1)); 
}

void update(uint  * rexp, ull  * rfrac, u128 ufrac, int bit)
{
	int tp = ctop(ufrac); 
	if(bit + tp - 52>1023)
	{
		* rexp = (1 << 11) - 1; 
		* rfrac = 0; 
		return; 
	}

	if(tp<52)
	{
		ufrac <<= (52 - tp); 
		bit -= (52 - tp); 
	}

	if(bit + tp - 52< -1022)
	{
		tp = 52; 
		ufrac = rounding(ufrac, -1022 - bit); 
		bit = -bias; 
	}
	
	if(tp>52)
	{
		ufrac = rounding(ufrac, tp - 52); 
		bit += tp - 52; 
	}

	* rexp = bit + bias; 
	* rfrac = ((ull)ufrac)&((1ull << 52) - 1); 
}

doubles divide(doubles a, doubles b)
{
	if(is_nan(a))return a; 
	if(is_nan(b))return b; 
	int f1 = is_zero(a), f2 = is_zero(b); 
	if(f1 && f2)
		return integrate(1, (1 << 11) - 1, 1); 
	if(f1)
		return integrate(sign(a)^sign(b), 0, 0); 
	if(f2)
		return integrate(sign(a)^sign(b), (1 << 11) - 1, 0); 
	f1 = is_inf(a), f2 = is_inf(b); 
	if(f1 && f2)
		return integrate(1, (1 << 11) - 1, 1); 
	if(f1)
		return integrate(sign(a)^sign(b), (1 << 11) - 1, 0); 
	if(f2)
		return integrate(sign(a)^sign(b), 0, 0); 

	uint exp1 = cexps(a), exp2 = cexps(b), rexp; 
	u128 ufrac, frac1 = cfrac(a), frac2 = cfrac(b); 
	ull rfrac = 0; 
	
	if(!is_denormalize(a))
		frac1 += ((u128)1) << 52; 
	if(!is_denormalize(b))
		frac2 += ((u128)1) << 52; 

	rexp = exp1 - exp2; 
	
	frac1 <<= 52; 

	int tp = ctop(frac1), std = 119; 
	if(tp<std)	
		frac1 <<= (std - tp), rexp -= std - tp, 
	ufrac = frac1/frac2; 
	if(ufrac * frac2<frac1)ufrac |= 1; 

	update(&rexp, &rfrac, ufrac, rexp); 	
	return integrate(sign(a)^sign(b), rexp, rfrac); 
}

doubles mult(doubles a, doubles b)
{
	if(is_nan(a))return a; 
	if(is_nan(b))return b; 
	if(is_inf(a) && is_inf(b))
		return integrate(sign(a)^sign(b), cexps(a), cfrac(a)); 
	if((is_inf(a) && is_zero(b)) || (is_inf(b) && is_zero(a)))
	{
		if(is_inf(a))
		{
			a.c[0] ++ ; 
			return a; 
		}
		else
		{
			b.c[0] ++ ; 
			return b; 
		}
	}
	if(is_inf(a))return a; 
	if(is_inf(b))return b; 

	u128 ufrac = 0, frac1 = (u128)cfrac(a), frac2 = (u128)cfrac(b); 
	ull rfrac = 0; 

	if(!is_denormalize(a))
		frac1 += (1ull << 52); 
	if(!is_denormalize(b))
		frac2 += (1ull << 52); 

	uint rexp = cexps(a) + cexps(b) - 52; 
	ufrac = frac1 * frac2; 

	update(&rexp, &rfrac, ufrac, rexp - bias * 2); 	
	return integrate(sign(a)^sign(b), rexp, rfrac); 
}

doubles plus(doubles a, doubles b)
{
	if(is_zero(a))return b; 
	if(is_zero(b))return a; 
	if(is_nan(a))return a; 
	if(is_nan(b))return b; 
	if(is_inf(a) && is_inf(b))
	{
		if(sign(a) == sign(b))return a; 
		else return integrate(1, (1 << 11) - 1, (1ull << 52) - 1); 
	}
	if(is_inf(a))return a; 
	if(is_inf(b))return b; 

	int sgn1 = sign(a), sgn2 = sign(b); 
	int exp1 = cexps(a), exp2 = cexps(b), rexp = 0; 
	u128 frac1 = (u128)cfrac(a), frac2 = (u128)cfrac(b), ufrac = 0; 
	ull rfrac = 0; 

	if(!is_denormalize(a))
		frac1 += (1ull << 52); 
	if(!is_denormalize(b))
		frac2 += (1ull << 52); 
	
	if(exp2>exp1 || (exp2 == exp1 && frac2>frac1))
	{
		swap(sgn1, sgn2); 
		swap(exp1, exp2); 
		swap(frac1, frac2); 
		dswap(&a, &b); 
	}
	
	if(exp1 - exp2>60)
		return integrate(sgn1, exp1, frac1); 
	
	frac1 <<= ofs; frac2 <<= ofs; 
	frac2 >>= exp1 - exp2; 
	if(!(sgn1^sgn2))
		ufrac = frac1 + frac2; 
	else
		ufrac = frac1 - frac2; 

	update(&rexp, &rfrac, ufrac, exp1 - ofs - bias); 
	return integrate(sgn1, rexp, rfrac); 
}

doubles minus(doubles a, doubles b)
{
	b.c[7] ^= 1 << 7; 
	return plus(a, b); 
}

doubles convert(ull a)
{
	int i, j; 
	doubles b; b = init(); 
	for(i = 0; i <= 7; i ++ )
		for(j = 0; j <= 7; j ++ , a >>= 1)
			if(a&1ull)
				b.c[i] |= 1 << j; 
	return b; 
}

ull convertl(doubles a)
{
	ull b = 0; int i, j; 
	for(i = 7; i >= 0; i -- )
		for(j = 7; j >= 0; j -- )
			if((a.c[i] >> j)&1)
				b |= 1ull << (i * 8 + j); 
	return b; 
}

doubles calc(doubles a, doubles b, char op) 
{
    doubles c; 

	if(op == '+')
		c = plus(a, b); 
	else if(op == '-')
		c = minus(a, b); 
	else if(op == '*')
		c = mult(a, b); 
	else if(op == '/')
		c = divide(a, b); 
   
	return c; 
}
