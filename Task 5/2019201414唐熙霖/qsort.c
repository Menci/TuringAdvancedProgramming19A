#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define SPACE_LEFT_FOR_EXCHANGE 2000000
#define THRESHOLD_FOR_INSERT_SORT 16
#define SIZE_OF_LONG_LONG 8

size_t n = 0, siz = 0;
int (*cmp)(const void *, const void *);
unsigned char space[SPACE_LEFT_FOR_EXCHANGE];

void my_swap(void *, void *);
void my_memcpy(void *, void *, size_t);

bool check(void *);
size_t my_rand();
void my_random_shuffle(void *);

void insert_sort(void *, size_t);
void my_qsort(void *, size_t);
void solve(void *, size_t, size_t, int(*)(const void *, const void *));

void my_memcpy(void *dest, void *src, size_t size)
{
	if(size == 1)
	{
		*(char *)dest = *(char *)src;
		return ;
	}
	int i;
	for(i = 0; i < size; i += SIZE_OF_LONG_LONG)
		*(long long *)(dest + i) = *(long long *)(src + i);
}

inline void my_swap(void *a, void *b)
{
	my_memcpy(space, a, siz),
	my_memcpy(a, b, siz),
	my_memcpy(b, space, siz);
}

bool check(void *bas)
{
	int i;
	for(i = 0; (i << 1) < n; ++i)
		if(cmp(bas + siz*i, bas + (n-i-1)*siz) > 0)
			my_swap(bas + siz*i, bas + (n-i-1)*siz);

	for(i = 1; i < n; ++i)
		if(cmp(bas + siz*i, bas + (i-1)*siz) < 0)
			return 0;
	return 1;
}

inline size_t my_rand()
{
	return (size_t)(rand()<<15|rand()) % n;
}

void my_random_shuffle(void *bas)
{
	int i;
	srand(time(NULL));
	for(i = 0; i < n; i += 5)
		my_swap(bas + i*siz, bas + my_rand()*siz);
}

inline void insert_sort(void *bas, size_t nmemb)
{
	if(nmemb == 0)
		return;
	int i, j;
	for(i = 1; i < nmemb; ++i)
		for(j = 0; j < i; ++j)
			if(cmp(bas + i*siz, bas + j*siz) <= 0)
			{
				my_memcpy(space, bas + i*siz, siz),
				memmove(bas + (j+1)*siz, bas + j*siz, siz*(i-j)),
				my_memcpy(bas + j*siz, space, siz);
				break;
			}
}

void my_qsort(void *bas, size_t nmemb) 
{
	if(nmemb <= THRESHOLD_FOR_INSERT_SORT)
		return (void) insert_sort(bas, nmemb);

	void *i = bas, *j = bas + (nmemb-1)*siz;
	my_memcpy(space, i, siz);
	
	int cnt = 1, tmp;

	while(i < j)
	{
		while(i < j && (tmp = cmp(j, space)) > 0)
			j -= siz;
		if(tmp == 0)
			++cnt;
		if(i < j)
			my_memcpy(i, j, siz),
			i += siz;

		while(i < j && (tmp = cmp(i, space)) <=0)
			cnt += (tmp == 0),	
			i += siz;
		if(i < j)
			my_memcpy(j, i, siz),
			j -= siz;
	}

	if(cnt == nmemb)
		return;

	my_memcpy(i, space, siz);

	my_qsort(bas, (i-bas)/siz);
	my_qsort(i + siz, nmemb - (i-bas)/siz - 1);
}

void solve(void *bas, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
	n = nmemb,
	siz = size,
	cmp = compar;

	if(check(bas))
		return ;
	
	my_random_shuffle(bas);
	my_qsort(bas, n);
	
	return;
}

