#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define SD 8
#define ES 11
#define FS 52
#define getint(x,y) (((x)>>(y))&1)

struct td
{
	unsigned char c[SD];
};

int isin(struct td x)
{
	int i=0;
	for(i=0;i<FS;i++)
		if(get(x,i))
			return 0;
	for(i=FS;i<FS+ES;i++)
		if(get(x,i)==0)
			return 0;
	return 1;
}

int isna(struct td x)
{
	int i;
	for(i=FS;i<FS+ES;i++)
		if(get(x,i)==0)
			return 0;
	for(i=0;i<FS;i++)
		if(get(x,i))
			return 1;
	return 0;
}

int iszero(struct td x)
{
	int i;
	for(i=0;i<ES+FS;i++)
		if(get(x,i))
			return 0;
	return 1;
}

struct td getinf(int M)
{
	int i;
	struct td res;
	memset(&res,0,sizeof(res));
	for(i=FS;i<=ES+FS;i++)
		if(i<ES+FS||M)
			res.c[i>>3]+=1<<(i&7);
	return res;
}

struct td getnan(int M)
{
	struct td res=getinf(M);
	res.c[0]|=1;
	return res;
}

struct td getzero(int M)
{
	struct td res;
	memset(&res,0,sizeof(res));
	set(&res,ES+FS,M);
	return res;
}

int get(struct td x,int y)
{
	return (x.c[y>>3]>>(y&7))&1;
}

void set(struct td *x,int y,int z)
{
	if(z)
		x->c[y>>3]|=(z<<(y&7));
	else if(get(*x,y)!=z)
		x->c[y>>3]^=(1<<(y&7));
}

struct td multi(struct td x,struct td y)
{
	long long Mx,My,Ex=0,Ey=0,i;
	__int128 Fx=0,Fy=0;
	
	for(i=0;i<FS;i++)
	{
		Fx|=(__int128)get(x,i)<<i;
		Fy|=(__int128)get(y,i)<<i;
	}
	for(i=0;i<ES;i++)
	{
		Ex|=get(x,i+FS)<<i;
		Ey|=get(y,i+FS)<<i;
	}
	Mx=get(x,ES+FS);
	My=get(y,ES+FS);
	long long Blas=(1<<(ES-1))-1;
	if(Ex==0) Fx<<=1; else Fx+=(__int128)1<<FS;
	if(Ey==0) Fy<<=1; else Fy+=(__int128)1<<FS;
	Ex-=Blas;Ey-=Blas;
		
	long long Mz=Mx^My,Ez=Ex+Ey;
	__int128 Fz=Fx*Fy;
	
	if(isna(x)) return x;
	if(isna(y)) return y;
	if(isin(x))
		if(iszero(y))
			return getnan(Mz);
		else return getinf(Mz);
	if(isin(y))
		if(iszero(x))
			return getnan(Mz);
		else return getinf(Mz);
	
//	print(Fx,16);print(Fy,16);print(Fz,16);
	int w=0;__int128 t=Fz;
	while(t>0)
	{
		t>>=1;
		w++;
	}
	Ez-=FS;
	int rd=1,ird=0;
	if(w>FS+1)
	{
		for(i=0;i<w-FS-2;i++)
		{
			if(Fz&1) rd=0;
			Fz>>=1; 
		}
		if(Fz&1)
		{
			Fz++;
			ird=1;
			if(rd&&(Fz&2))
			{
				Fz--;
				ird=0;
			}
		}
		if(Fz&1) rd=0;
		Fz>>=1;
		Ez+=w-FS-1;
//		printf("%d %d\n",rd,(Fz&1));
	}
	if(w<=FS)
	{
		Fz<<=(FS-w+1);
		Ez-=FS-w+1;
	}
//	print(Fz,16);
//	printf("%d\n",Ez);
	
	if(Ez>Blas) return getinf(Mz);
	if(Ez<=-Blas)
	{
		Fz-=ird;
		for(i=0;i<-Blas-Ez;i++)
		{
			if(Fz&1) rd=0;
			Fz>>=1; 
		}
		if(Fz&1)
		{
			Fz++;
			if(rd&&(Fz&2))
				Fz--;
		}
		Fz>>=1;
		Ez=-Blas; 
	}
	Ez+=Blas;
//	print(Fz,16);
	
	struct td res;
	memset(&res,0,sizeof(res));
	
	for(i=0;i<FS;i++)
		set(&res,i,getint(Fz,i));
	for(i=0;i<ES;i++)
		set(&res,i+FS,getint(Ez,i));
	set(&res,ES+FS,Mz);
	return res; 
}

struct td add(struct td x,struct td y)
{
	long long Mx,My,Ex=0,Ey=0,i;
	__int128 Fx=0,Fy=0;
	
	for(i=0;i<FS;i++)
	{
		Fx|=(__int128)get(x,i)<<i;
		Fy|=(__int128)get(y,i)<<i;
	}
	for(i=0;i<ES;i++)
	{
		Ex|=get(x,i+FS)<<i;
		Ey|=get(y,i+FS)<<i;
	}
	Mx=get(x,ES+FS);
	My=get(y,ES+FS);
	long long Blas=(1<<(ES-1))-1;
	if(Ex==0) Fx<<=1; else Fx+=(__int128)1<<FS;
	if(Ey==0) Fy<<=1; else Fy+=(__int128)1<<FS;
	Ex-=Blas;Ey-=Blas;
	
	if(Ex>Ey) return add(y,x);
	
	if(Mx) Fx=-Fx;
	if(My) Fy=-Fy;
	Fx<<=16;Fy<<=16;
	long long Mz=0,Ez=Ey-16;
	__int128 Fz;
//	print(Fx,16);
	int rd=1;
	for(i=0;i<Ey-Ex;i++)
	{
		if(Fx&1) rd=0;
		Fx>>=1;
	}
	Fx<<=1;Fy<<=1;
	Fx+=1^rd;
	Fz=Fx+Fy;
	Ez--;
	if(Fz<0)
	{
		Mz=1;
		Fz=-Fz;
	}
	
	if(isna(x)) return x;
	if(isna(y)) return y;
	if(isin(x))
	{
		if(isin(y)&&(Mx^My))
			return getnan(0);
		return x;
	}
	if(isin(y))
	{
		if(isin(x)&&(Mx^My))
			return getnan(0);
		return y;
	}
	
//	print(Fx,16);print(Fy,16);print(Fz,16);
	int w=0;__int128 t=Fz;
	while(t>0)
	{
		t>>=1;
		w++;
	}
	int ird=0;
	if(w>FS+1)
	{
		for(i=0;i<w-FS-2;i++)
		{
			if(Fz&1) rd=0;
			Fz>>=1; 
		}
		if(Fz&1)
		{
			Fz++;
			ird=1;
			if(rd&&(Fz&2))
			{
				Fz--;
				ird=0;
			}
		}
		if(Fz&1) rd=0;
		Fz>>=1;
		Ez+=w-FS-1;
//		printf("%d %d\n",rd,(Fz&1));
	}
	if(w<=FS)
	{
		Fz<<=(FS-w+1);
		Ez-=FS-w+1;
	}
//	print(Fz,16);
	
	if(Ez>Blas) return getinf(Mz);
	if(Ez<=-Blas)
	{
		Fz-=ird;
		for(i=0;i<-Blas-Ez;i++)
		{
			if(Fz&1) rd=0;
			Fz>>=1; 
		}
		if(Fz&1)
		{
			Fz++;
			if(rd&&(Fz&2))
				Fz--;
		}
		Fz>>=1;
		Ez=-Blas; 
	}
	Ez+=Blas;
	
	struct td res;
	memset(&res,0,sizeof(res));
	
	for(i=0;i<FS;i++)
		set(&res,i,getint(Fz,i));
	for(i=0;i<ES;i++)
		set(&res,i+FS,getint(Ez,i));
	set(&res,ES+FS,Mz);
	return res; 
}

struct td sub(struct td x,struct td y)
{
	set(&y,ES+FS,get(y,ES+FS)^1);
	return add(x,y);
}

struct td divid(struct td x,struct td y)
{
	long long Mx,My,Ex=0,Ey=0,i;
	__int128 Fx=0,Fy=0;
	
	for(i=0;i<FS;i++)
	{
		Fx|=(__int128)get(x,i)<<i;
		Fy|=(__int128)get(y,i)<<i;
	}
	for(i=0;i<ES;i++)
	{
		Ex|=get(x,i+FS)<<i;
		Ey|=get(y,i+FS)<<i;
	}
	Mx=get(x,ES+FS);
	My=get(y,ES+FS);
	long long Blas=(1<<(ES-1))-1;
	if(Ex==0) Fx<<=1; else Fx+=(__int128)1<<FS;
	if(Ey==0) Fy<<=1; else Fy+=(__int128)1<<FS;
	Ex-=Blas;Ey-=Blas;
		
	long long Mz=Mx^My,Ez=Ex-Ey-16;
	Fx<<=FS+16;
	__int128 Fz=Fx/Fy;
	
	if(isna(x)) return getnan(Mz);
	if(isna(y)) return getnan(Mz);
	if(iszero(y))
	{
		if(iszero(x))
			return getnan(Mz);
		return getinf(Mz);
	}
	if(isin(y))
	{
		if(isin(x))
			return getnan(Mz);
		return getzero(Mz);
	}
	if(isin(x))
		return getinf(Mz); 
	
//	print(Fx,16);print(Fy,16);print(Fz,16);
	int w=0;__int128 t=Fz;
	while(t>0)
	{
		t>>=1;
		w++;
	}
	int rd=1,ird=0;
	if(w>FS+1)
	{
		for(i=0;i<w-FS-2;i++)
		{
			if(Fz&1) rd=0;
			Fz>>=1; 
		}
		if(Fz&1)
		{
			Fz++;
			ird=1;
			if(rd&&(Fz&2)&&(Fz%Fy==0))
			{
				Fz--;
				ird=0;
			}
		}
		if(Fz&1) rd=0;
		Fz>>=1;
		Ez+=w-FS-1;
//		printf("%d %d\n",rd,(Fz&1));
	}
	if(w<=FS)
	{
		Fz<<=(FS-w+1);
		Ez-=FS-w+1;
	}
//	print(Fz,16);
	
	if(Ez>Blas) return getinf(Mz);
	if(Ez<=-Blas)
	{
		Fz-=ird;
		for(i=0;i<-Blas-Ez;i++)
		{
			if(Fz&1) rd=0;
			Fz>>=1; 
		}
		if(Fz&1)
		{
			Fz++;
			if(rd&&(Fz&2))
				Fz--;
		}
		Fz>>=1;
		Ez=-Blas; 
	}
	Ez+=Blas;
	
	struct td res;
	memset(&res,0,sizeof(res));
	
	for(i=0;i<FS;i++)
		set(&res,i,getint(Fz,i));
	for(i=0;i<ES;i++)
		set(&res,i+FS,getint(Ez,i));
	set(&res,ES+FS,Mz);
	return res; 
}

double rnd()
{
	double x=(rand()*100000.0+(double)rand())-(rand()*100000.0+(double)rand());
	double y=rand()*100000.0+(double)rand()-(rand()*100000.0+(double)rand());
	return x/y;
}

void print(unsigned char* c,int t)
{
	int i,j;
	for(i=0;i<t;i++,printf("|"))
		for(j=0;j<8;j++)
			printf("%d",(*(c+i))>>j&1);
	printf("\n");
}

uint64_t solve(uint64_t a, uint64_t b, char op) 
{
    struct td x=*(struct td*)&a,y=*(struct td*)&b,res;
    
    if (op == '+') res=add(x,y);
    if(op=='-') res=sub(x,y);
    if(op=='*') res=multi(x,y);
    if(op=='/') res=divid(x,y);
    
    return *(uint64_t*)&res;
}

int main()
{ 
	int i; 
	srand(time(0));
	struct td x,y,s;
	double dx,dy,ds;
	long long t;
	for(i=0;;i++)
	{
	//	if(i%10000==0) printf("%d\n",i);
		t=0xbe3798318c20002c;
		dx=*(double*)&t;
		t=0x3f95a9ab0573d9b3;
		dy=*(double*)&t;
		x=*(struct td*)&dx;
		y=*(struct td*)&dy;
		s=add(x,y);
		ds=dx+dy;
		if(*(double*)&s!=ds)
		{
			printf("%d\n",i);
			printf("%.30lf %.30lf\n",dx,dy);
			printf("%.30lf %.30lf\n",x,y);
			print(&x,8);print(&y,8);
			print(&s,8);print(&ds,8);
			printf("My Answer:\t%.30lf\n",s);
			printf("Right Answer:\t%.30lf\n",ds);
			system("pause");
		}
	}
	scanf("%lf%lf",&x,&y);
	printf("My Answer:\t%.30lf\n",multi(x,y));
	printf("Right Answer:\t%.30lf\n",multy(dx,dy));
    return 0;
}
