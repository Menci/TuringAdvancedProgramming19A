#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define SD 8
#define ES 11
#define FS 52

struct td
{
	unsigned char c[SD];
}in[2], na, zero[2], number[11];

const long long Bias = (1ll << (ES - 1)) - 1;

int isin(struct td *x)
{
	long long t = *(long long*) x;
	if(((t >> FS) & ((1ll << ES) - 1)) != ((1ll << ES) - 1)) return 0;
	if((t & ((1ll << FS) - 1)) != 0) return 0;
	return 1;
}

int isna(struct td *x)
{
	long long t = *(long long*) x;
	if(((t >> FS) & ((1ll << ES) - 1)) != ((1ll << ES) - 1)) return 0;
	if((t & ((1ll << FS) - 1)) != 0) return 1;
	return 0;
}

int iszero(struct td *x)
{
	long long t = *(long long*) x;
	if((t << 1) != 0) return 0;
	return 1;
}

void init(struct td *x, __int128 *Fx, long long *Ex, long long *Mx)
{
	long long Tx = *(long long*) x;
	
	*Mx = (Tx >> ES + FS) & 1;
	*Ex = (Tx >> FS) & ((1ll << ES) - 1);
	*Fx = Tx & ((1ll << FS) - 1);
	
	if(*Ex == 0) *Fx <<= 1; else *Fx += (__int128)1 << FS;
	*Ex -= Bias;
}

void rounding(__int128 *Ft, long long *Et, long long *Mt, int flag)
{
	__int128 Fz = *Ft;
	long long Ez = *Et, Mz = *Mt;
	
	int w = 0;
	__int128 t = Fz;
	while(t > 0)
	{
		t >>= 1;
		w ++ ;
	}
	int rd = 1 , ird = 0;
	if(w > FS + 1)
	{
		if((Fz & (((__int128)1 << (w - FS - 2 >= 128 ? 127 : w - FS - 2)) - 1)) != 0) rd = 0;
		(w - FS - 2 >= 128) ? (Fz = 0) : (Fz >>= w - FS - 2);
		if(Fz & 1)
		{
			Fz ++ ;
			ird = 1;
			if(rd && (Fz & 2) && flag)
			{
				Fz -- ;
				ird = 0;
			}
		}
		if(Fz & 1) rd = 0;
		Fz >>= 1;
		Ez += w - FS - 1;
	}
	if(w <= FS)
	{
		Fz <<= (FS - w + 1);
		Ez -= FS - w + 1;
	}
	
	if(Ez > Bias)
	{
		Fz = 0;
		Ez = 1ll << ES - 1;
	}
	if(Ez <= - Bias)
	{
		Fz -= ird;
		if((Fz & (((__int128)1 << ( - Bias - Ez >= 128 ? 127 : - Bias - Ez)) - 1)) != 0) rd = 0;
		( - Bias - Ez >= 128) ? (Fz = 0) : (Fz >>= - Bias - Ez);
		if(Fz & 1)
		{
			Fz ++ ;
			if(rd && (Fz & 2) && flag)
				Fz -- ;
		}
		Fz >>= 1;
		Ez = - Bias; 
	}
	Ez += Bias;
	
	*Ft = Fz;
	*Et = Ez;
	*Mt = Mz; 
}

struct td getres(__int128 Fz, long long Ez, long long Mz)
{
	long long res;
	res = Mz & 1;
	res <<= ES;
	res |= Ez & ((1ll << ES) - 1);
	res <<= FS;
	res |= Fz & ((1ll << FS) - 1);
	return *(struct td*) &res; 
}

struct td add(struct td x, struct td y)
{
	long long Mx, My, Ex, Ey;
	__int128 Fx, Fy;
	
	init(&x, &Fx, &Ex, &Mx);
	init(&y, &Fy, &Ey, &My);
	
	if(Ex > Ey) return add(y,x);
	
	if(Mx) Fx = - Fx;
	if(My) Fy = - Fy;
	Fx <<= 16;Fy <<= 16;
	long long Mz = 0,Ez = Ey - 16;
	__int128 Fz;
	int rd = 1;
	if((Fx & (((__int128)1 << (Ey-Ex >= 128 ? 127 : Ey - Ex)) - 1)) != 0) rd = 0;
	(Ey-Ex >= 128) ? (Fx = 0) : (Fx >>= Ey - Ex); 
	Fx <<= 1;
	Fy <<= 1;
	Fx += 1 ^ rd;
	Fz = Fx + Fy;
	Ez -- ;
	if(Fz < 0)
	{
		Mz = 1;
		Fz = - Fz;
	}
	
	if(isna(&x)) return x;
	if(isna(&y)) return y;
	if(isin(&x))
	{
		if(isin(&y) && (Mx ^ My))
			return na;
		return x;
	}
	if(isin(&y))
	{
		if(isin(&x) && (Mx ^ My))
			return na;
		return y;
	}
	
	rounding(&Fz, &Ez, &Mz, 1);

	return getres(Fz, Ez, Mz);
}

struct td sub(struct td x, struct td y)
{
	y.c[7] ^= 128;
	return add(x , y);
}

struct td multi(struct td x, struct td y)
{
	long long Mx, My, Ex, Ey;
	__int128 Fx, Fy;
	
	init(&x, &Fx, &Ex, &Mx);
	init(&y, &Fy, &Ey, &My);
	
	long long Mz = Mx ^ My , Ez = Ex + Ey - FS;
	__int128 Fz = Fx * Fy;
	
	if(isna(&x)) return x;
	if(isna(&y)) return y;
	if(isin(&x))
		if(iszero(&y))
			return na;
		else
			return in[Mz & 1];
	if(isin(&y))
		if(iszero(&x))
			return na;
		else
			return in[Mz & 1];
	
	rounding(&Fz, &Ez, &Mz, 1);

	return getres(Fz,Ez,Mz);
}

struct td divid(struct td x, struct td y)
{
	long long Mx, My, Ex, Ey;
	__int128 Fx, Fy;
	
	init(&x, &Fx, &Ex, &Mx);
	init(&y, &Fy, &Ey, &My);
		
	long long Mz = Mx ^ My , Ez = Ex - Ey - 16;
	Fx <<= FS + 16;
	__int128 Fz = Fx / Fy;
	
	if(isna(&x)) return na;
	if(isna(&y)) return na;
	if(iszero(&y))
		if(iszero(&x))
			return na;
		else
			return in[Mz & 1];
	if(isin(&y))
		if(isin(&x))
			return na;
		else
			return zero[Mz & 1];
	if(isin(&x))
		return in[Mz & 1];
	
	rounding(&Fz,&Ez,&Mz,Fx%Fy==0);
	
	return getres(Fz,Ez,Mz); 
}

struct td calc(struct td a, struct td b, char op) 
{
	if(op == '+') return add(a,b);
    if(op == '-') return sub(a,b);
    if(op == '*') return multi(a,b);
    if(op == '/') return divid(a,b);
}

void totinit()
{
	int i;
	long long res = 0;
	zero[0] = *(struct td*) &res;
	res = 1ll << ES + FS;
	zero[1] = *(struct td*) &res;
	res = (((1ll << ES) - 1ll) << FS);
	in[0] = *(struct td*) &res;
	res = (1ll << ES + FS) + (((1ll << ES) - 1ll) << FS);
	in[1] = *(struct td*) &res;
	res ++ ;
	na = *(struct td*) &res;
	number[0] = zero[0];
	res = 0x3ff0000000000000ll;
	number[1] = *(struct td*) &res;
	for(i = 2; i <= 10; i ++)
		number[i] = add(number[i - 1], number[1]);
}

struct td getnum(char *s, int x)
{
	struct td res = zero[0];
	int neg = 0 , t = 0;
	
	if(s[x] == '+' || s[x] == '-')
		if(s[x ++] == '-') neg = 1;
	while(s[x] >= '0' && s[x] <= '9')
		res = add(multi(res, number[10]), number[s[x ++] - '0']);
	if(s[x] == '.')
	{
		x ++;
		while(s[x] >= '0' && s[x] <= '9')
		{
			res = add(multi(res, number[10]), number[s[x ++] - '0']);
			t ++;
		}
	}
	while(t --)
		res = divid(res, number[10]);
	
	if(neg) res.c[7] ^= 128;
	
	return res;
}

int gj[20200], gjs, gjx;

void gjmul(int x)
{
	int i;
	for(i = 0; i < gjs; i ++)
	{
		gj[i] = gj[i] * x;
		if(i)
		{
			gj[i] += gj[i - 1] / 10;
			gj[i - 1] %= 10;
		}
	}
	while(gj[gjs - 1] >= 10)
	{
		gj[gjs] = gj[gjs - 1] / 10;
		gj[gjs - 1] %= 10;
		gjs ++;
	}
}

void putnum(struct td x)
{
	__int128 Fx;
	long long Ex, Mx, i;
	init(&x, &Fx, &Ex, &Mx);
	if(Mx) printf("-");
	
	gjs = gjx = 0;
	memset(gj, 0, sizeof(gj));
	while(Fx > 0)
	{
		gj[gjs ++] = Fx % 10;
		Fx /= 10;
	}
	
	for(i = 0; i < FS; i ++, gjx ++)
		gjmul(5);

	if(Ex > 0)
		for(i = 0; i < Ex; i++)
			gjmul(2);
	else
		for(i = 0; i < -Ex; i ++, gjx ++)
			gjmul(5);
	
	if(gjs <= gjx)
	{
		printf("0.");
		for(i = gjs; i < gjx ; i ++)
			printf("0");
		for(i = gjs - 1; i >= 0; i --)
			printf("%d", gj[i]);
		printf("\n");
	}
	else
	{
		for(i = gjs - 1; i >= gjx; i --)
			printf("%d", gj[i]);
		if(gjx)
		{
			printf(".");
			for(i = gjx - 1; i >= 0; i --)
				printf("%d", gj[i]);
		}
		printf("\n");
	}
	fflush(stdout);
}

struct w
{
	int x;
	char op;
	struct td num;
};

struct w getwop(char op)
{
	struct w res;
	res.x = 0;
	res.op = op;
	return res;
}

struct w getwnum(struct td num)
{
	struct w res;
	res.x = 1;
	res.num = num;
	return res;
}

void dell(char *s)
{
	int i = 0, j = 0, l = strlen(s);
	while(j < l)
	{
		while(j < l && s[j] == ' ') j++;
		if(j == l) break;
		s[i ++] = s[j ++];
	}
	s[i] = 0;
}

char st[100100];

void convert(char *s, struct w *ex, int *lex)
{
	int len = strlen(s), i = 0 , l = 0, isnum = 1, siz = 0;
	
	while(i < len)
	{
		if(s[i] == '(')
		{
			st[siz ++] = s[i ++]; 
			isnum = 1;
			continue;
		}
		
		if(s[i] == ')')
		{
			siz --; 
			while(st[siz] != '(')
				ex[l ++] = getwop(st[siz --]);
			i ++;
			isnum = 0;
		}
		
		if(isnum)
		{
			ex[l ++] = getwnum(getnum(s, i));
			isnum = 0;
			if(s[i] == '+' || s[i] == '-') i++;
			while((s[i] == '.') || (s[i] >= '0' && s[i] <= '9')) i++;
			isnum = 0;
			continue;
		}
		
		if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
		{
			while(1)
			{
				if(siz == 0 || st[siz - 1] == '(' || ((st[siz - 1] == '+' || st[siz - 1] == '-') && (s[i] == '*' || s[i] == '/')))
					break;
				ex[l ++] = getwop(st[-- siz]);
			}
			st[siz ++] = s[i ++];
			isnum = 1;
			continue ; 
		}
	}
	while(siz > 0)
		ex[l ++] = getwop(st[-- siz]);
	
	*lex = l;
}

struct td stn[1000100];

struct td getans(struct w *ex, int l)
{
	int siz = 0, i;
	for(i = 0; i < l; i ++)
		if(ex[i].x)
			stn[siz ++] = ex[i].num;
		else
		{
			stn[siz - 2] = calc(stn[siz - 2], stn[siz - 1], ex[i].op);
			siz --;
		}
	
	return stn[0];
}

char s[5000000],c;
struct w ex[1000100];

int main()
{
	totinit();
	fflush(stdout);	
	
	int i, l;
	while(1)
	{
		l = 0;
		for(i = 0;(c = getchar()) != '\n'; i ++)
		{
			if(c == EOF)
				return 0;
			s[l] = c;
			l ++;
		}
		s[l] = 0;
		dell(s);
		
		convert(s, ex, &l);
		
		struct td res = getans(ex ,l);
		putnum(res);
	} 
	fflush(stdout);
	return 0;
}
