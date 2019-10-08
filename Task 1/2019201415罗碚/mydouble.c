#include <stdint.h>
#define SD 8
#define ES 11
#define FS 52

struct td
{
	unsigned char c[SD];
}in[2],na,zero[2];

const long long Blas = (1ll << (ES - 1)) - 1;

inline int isin(struct td *x)
{
	long long t = *(long long*) x;
	if(((t >> FS) & ((1ll << ES) - 1)) != ((1ll << ES) - 1)) return 0;
	if((t & ((1ll << FS) - 1)) != 0) return 0;
	return 1;
}

inline int isna(struct td *x)
{
	long long t = *(long long*) x;
	if(((t >> FS) & ((1ll << ES) - 1)) != ((1ll << ES) - 1)) return 0;
	if((t & ((1ll << FS) - 1)) != 0) return 1;
	return 0;
}

inline int iszero(struct td *x)
{
	long long t = *(long long*) x;
	if((t << 1) != 0) return 0;
	return 1;
}

void totinit()
{
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
}

inline void init(struct td *x, __int128 *Fx, long long *Ex, long long *Mx)
{
	long long Tx = *(long long*) x;
	
	*Mx = (Tx >> ES + FS) & 1;
	*Ex = (Tx >> FS) & ((1ll << ES) - 1);
	*Fx = Tx & ((1ll << FS) - 1);
	
	if(*Ex == 0) *Fx <<= 1; else *Fx += (__int128)1 << FS;
	*Ex -= Blas;
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
	
	if(Ez > Blas)
	{
		Fz = 0;
		Ez = 1ll << ES - 1;
	}
	if(Ez <= - Blas)
	{
		Fz -= ird;
		if((Fz & (((__int128)1 << ( - Blas - Ez >= 128 ? 127 : - Blas - Ez)) - 1)) != 0) rd = 0;
		( - Blas - Ez >= 128) ? (Fz = 0) : (Fz >>= - Blas - Ez);
		if(Fz & 1)
		{
			Fz ++ ;
			if(rd && (Fz & 2) && flag)
				Fz -- ;
		}
		Fz >>= 1;
		Ez = - Blas; 
	}
	Ez += Blas;
	
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

uint64_t solve(uint64_t a, uint64_t b, char op) 
{
	totinit();
    struct td x = *(struct td*) &a, y=*(struct td*) &b, res;
    
    if (op == '+') res = add(x,y);
    if(op=='-') res = sub(x,y);
    if(op=='*') res = multi(x,y);
    if(op=='/') res = divid(x,y);
    
    return *(uint64_t*) &res;
}
