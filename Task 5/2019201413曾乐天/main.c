#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef long long ll;
const int MAX_DEPTH = 8;
typedef unsigned char uchar;

void mswap(uchar *a, uchar *b, size_t siz)
{
	uchar t;
	while(siz--)
	{
		t = *a;
		*a++ = *b;
		*b++ = t;
	}
}

void msort(void *st, void *ed, size_t siz, int (*cmp)(const void *, const void *))
{
	int len = (ed - st) / siz + 1, flag = 0;
	void *lp = st, *rp = ed, *mid = st + len / 2 * siz, *i;
	
	if(ed <= st)
		return;
	if(len <= MAX_DEPTH)
	{
		for(flag = 1; st != ed && flag; ed -= siz)
		{
			flag = 0;
			for(i = st; i != ed; i += siz)
				if((*cmp)(i, i + siz) > 0)
				{
					mswap(i, i + siz, siz);
					flag = 1;
				}
		}
		return;
	}
	
	if((*cmp)(st, mid) > 0)
		mswap(st, mid, siz);
	if((*cmp)(st, ed) > 0)
		mswap(st, ed, siz);
	if((*cmp)(mid, ed) > 0)
		mswap(mid, ed, siz);

	while(1)
	{
		if(mid > lp)
			do
				lp += siz;
			while(lp < mid && (*cmp)(lp, mid) <= 0);

		if(mid <= lp)
			do
				lp += siz;
			while(lp <= ed && (*cmp)(lp, mid) <= 0);

		do
			rp -= siz;
		while(rp > mid && (*cmp)(rp, mid) > 0);

		if(rp < lp)
			break;
		mswap(lp, rp, siz);
		if(mid == rp)
			mid = lp;
	}

	rp += siz;
	if(mid < rp)
		do
			rp -= siz;
		while(mid < rp && (*cmp)(rp, mid) == 0);
	if(mid >= rp)
		do
			rp -= siz;
		while(rp > st && (*cmp)(rp, mid) == 0);

	msort(st, rp, siz, cmp);
	msort(lp, ed, siz, cmp);
}

void mqsort(void *a, size_t len, size_t siz, int (*cmp)(const void *, const void *))
{
	srand(time(0));
	msort(a, a + siz * (len - 1), siz, cmp);
}

int cmp ( const void *a , const void *b ) 
{ 
	if(*(ll *)a < *(ll *)b)return -1;
	if(*(ll *)a > *(ll *)b)return 1;
	return 0;
} 

ll a[2000009];

int main()
{
	if(fopen("in.txt", "r"))
	{
		freopen("in.txt", "r", stdin);
		freopen("out.txt", "w", stdout);
	}

	int n, i;
	scanf("%d", &n);
	for(i = 1; i <= n; i++)
		scanf("%lld",&a[i]);

	mqsort(a+1, n, sizeof(a[1]), cmp);
	printf("%d\n", n);
	for(i = 1; i <= n; i++)
		printf("%lld ", a[i]);
	puts("");

	return 0;
}
