#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

const int threshold = 16;

void my_sort(void *, size_t, size_t, int (*)(const void *, const void *));
void my_introsort_loop(void *, size_t, size_t, int (*)(const void *, const void *), int);
void* my_median(void *, void *, void *, int (*)(const void *,const void *));
void* my_partition(void *, size_t, size_t, int(*)(const void *, const void *));
void my_heap_sort(void *, size_t, size_t, int(*)(const void *, const void *));
void my_booble_sort(void *, size_t, size_t, int(*)(const void *, const void *));

void my_sort(void *A, size_t n, size_t s, int (*cmp)(const void *, const void *))
{
	void *lt = A, *rt = A + (n - 1) * s, *tmp = malloc(s);
	int i;
	while(lt < rt)
	{
		if(cmp(lt, rt) > 0)
		{
			memcpy(tmp, lt, s);
			memcpy(lt, rt, s);
			memcpy(rt, tmp, s);
		}
		lt += s;
		rt -= s;
	}
	
	bool flag = true;
	for(i = 0; i + 1 < n; ++i)
		if(cmp(A + i * s, A + (i + 1) * s) > 0)
		{
			flag = false;
			memcpy(tmp, A + i * s, s);
			memcpy(A + i * s, A + (i + 1) * s, s);
			memcpy(A + (i + 1) * s, tmp, s); 
		}
	if(flag) return ;
	
	free(tmp);
	
	my_introsort_loop(A, n, s, cmp, 30);
}

void my_introsort_loop(void *A, size_t n, size_t s, int (*cmp)(const void *, const void *), int depth_limit)
{
	int i;
	void *tmp = malloc(s), *lt, *rt;
	
	while(n > threshold)
	{
		if(depth_limit == 0)
		{
			my_heap_sort(A, n, s, cmp);
			return ;
		}
		
		--depth_limit;
			
		lt = my_median(A, A + (n / 2) * s, A + (n - 1) * s, cmp);
		rt = A;
		memcpy(tmp, lt, s);
		memcpy(lt, rt, s);
		memcpy(rt, tmp, s);
		
		void *cut = my_partition(A, n, s, cmp);
		
		my_introsort_loop(cut, n - (cut - A) / s, s, cmp, depth_limit);
		n=(cut - A) / s;
	}
	my_booble_sort(A, n, s, cmp); 
	
	free(tmp);
}

void* my_partition(void *A, size_t n, size_t s, int(*cmp)(const void *, const void *))
{ 
	void *left = A + s, *right = A + (n - 1) * s, *tmp = malloc(s);
	
	while(left <= right)
	{
		while(cmp(left, A) < 0)
			left += s;
		while(cmp(right, A) > 0)
			right -= s;
		if(left < right)
		{
			memcpy(tmp, left, s);
			memcpy(left, right, s);
			memcpy(right, tmp, s);
			left += s;
			right -= s;
		}
		else
			left += s;
	}
	
	memcpy(tmp, A, s);
	memcpy(A, right, s);
	memcpy(right, tmp, s);
	
	free(tmp);
	return right;
}

void* my_median(void *A, void *B, void *C, int (*cmp)(const void *,const void *))
{
	if (cmp(A, B) < 0)
		if (cmp(B, C) < 0)
			return B;
		else if (cmp(A, C) < 0)
			return C;
		else
			return A;
	else if (cmp(A, C) < 0)
		return A;
	else if (cmp(B, C) < 0)
		return C;
	else 
		return B;
}

void my_heap_sort(void *A, size_t n, size_t s, int(*cmp)(const void *, const void *))
{
	int i, j;
	int left, right, index, maxIndex;
	void *tmp = malloc(s);
	
	void *lt = A, *rt = A + (n - 1) * s;
	while(lt < rt)
	{
		if(cmp(lt, rt) > 0)
		{
			memcpy(tmp, lt, s);
			memcpy(lt, rt, s);
			memcpy(rt, tmp, s);
		}
		lt += s;
		rt -= s;
	}
	
	bool flag = true;
	for(i = 0; i + 1 < n; ++i)
		if(cmp(A + i * s, A + (i + 1) * s) > 0)
		{
			flag = false;
			memcpy(tmp, A + i * s, s);
			memcpy(A + i * s, A + (i + 1) * s, s);
			memcpy(A + (i + 1) * s, tmp, s); 
		}
	if(flag)
	{
		free(tmp);
		return ;
	}
	
	for(i = n / 2 - 1; i >= 0; --i)
	{
		index = i;
		while(1)
		{
			left = 2 * index + 1;
			right = 2 * index + 2;
		
			maxIndex = index;
			if(left < n && cmp(A + left * s, A + maxIndex * s) > 0) maxIndex = left;
			if(right < n && cmp(A + right * s, A + maxIndex * s) > 0) maxIndex = right;
			
			if(maxIndex == index) break;
			
			memcpy(tmp, A + index * s, s);
			memcpy(A + index * s, A + maxIndex * s, s);
			memcpy(A + maxIndex * s, tmp, s);
			
			index = maxIndex;
		}
	}
	
	for(i = n - 1; i >= 1; --i)
	{
		memcpy(tmp, A, s);
		memcpy(A, A + i * s, s);
		memcpy(A + i * s, tmp, s);
		
		index = 0;
		while(1)
		{
			left = 2 * index + 1;
			right = 2 * index + 2;
		
			maxIndex = index;
			if(left < i && cmp(A + left * s, A + maxIndex * s) > 0) maxIndex = left;
			if(right < i && cmp(A + right * s, A + maxIndex * s) > 0) maxIndex = right;
			
			if(maxIndex == index) break;
	
			memcpy(tmp, A + index * s, s);
			memcpy(A + index * s, A + maxIndex * s, s);
			memcpy(A + maxIndex * s, tmp, s);
			
			index = maxIndex;
		}
	}
	
	free(tmp);
}

void my_booble_sort(void *A, size_t n, size_t s, int(*cmp)(const void *, const void *))
{
	int i, j;
	void *tmp = malloc(s);
	
	void *lt = A, *rt = A + (n - 1) * s;
	while(lt < rt)
	{
		if(cmp(lt, rt) > 0)
		{
			memcpy(tmp, lt, s);
			memcpy(lt, rt, s);
			memcpy(rt, tmp, s);
		}
		lt += s;
		rt -= s;
	}
	
	bool flag = true;
	for(j = n; j >= 2 ; --j)
	{
		flag = true;
		for(i = 0; i + 1 < j; ++i)
			if(cmp(A + i * s, A + (i + 1) * s) > 0)
			{
				flag = false;
				memcpy(tmp, A + i * s, s);
				memcpy(A + i * s, A + (i + 1) * s, s);
				memcpy(A + (i + 1) * s, tmp, s); 
			}
		if(flag)
		{
			free(tmp);
			return ;
		}
	}
	
	free(tmp);
}
