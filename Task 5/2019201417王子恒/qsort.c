#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define val(x) ((char*) base + (x) * size)
const int sort_threshold = 16;

uint32_t SA = 23333, SB = 66666, SC = 233333;
uint32_t rnd(int l, int r) {
	SA ^= SA << 16; SA ^= SA >> 5; SA ^= SA << 1;
	uint32_t t = SA; SA = SB;
	SB = SC; SC ^= t ^ SA;
	return SC % (r - l + 1) + l;
}
void swap(void *a, void *b, size_t size) {
	int i, x = size >> 3, y = size % 8;
	for(i = 0; i < x; ++i) {
		long long t = *((long long*)a + i);
		*((long long*)a + i) = *((long long*)b + i);
		*((long long*)b + i) = t;
	}
	x <<= 3;
	for(i = 0; i < y; ++i) {
		char t = *((char*)a + x + i);
		*((char*)a + x + i) = *((char*)b + x + i);
		*((char*)b + x + i) = t;
	}
	return; 
}
void copy(void *a, void *b, size_t size) {
	int i, x = size >> 3, y = size % 8;
	for(i = 0; i < x; ++i)
		*((long long*)a + i) = *((long long*)b + i);
	x <<= 3;
	for(i = 0; i < y; ++i)
		*((char*)a + x + i) = *((char*)b + x + i);
	return; 
}

void build_heap(void *base, int l, int n, int p, size_t size, int (*compar)(const void *, const void *)) {
	void *tmp = (void*) malloc(size);
	copy(tmp, val(p + l - 1), size);
	int i;
	for(i = p << 1; i <= n; i <<= 1) {
		if(i < n && compar(val(i + l - 1), val(i + l)) < 0) ++i;
		if(compar(tmp, val(i + l - 1)) < 0) {
			copy(val(p + l - 1), val(i + l - 1), size);
			p = i;
		} else break;
	}
	copy(val(p + l - 1), tmp, size);
	free(tmp);
	return;
}
void partial_sort(void *base, int l, int r, size_t size, int (*compar)(const void *, const void *)) {
	int i, len = r - l + 1;
	for(i = len >> 1; i >= 1; --i)
		build_heap(base, l, len, i, size, compar);
	for(i = len; i > 1; --i) {
		swap(val(1 + l - 1), val(i + l - 1), size);
		build_heap(base, l, i - 1, 1, size, compar);
	}
	return;
}

void insertion_sort(void *base, int l, int r, size_t size, int (*compar)(const void *, const void *)) {
	if(l >= r) return;
	int i, j;
	void *tmp = (void*) malloc(size);
	for(i = l + 1; i <= r; ++i) {
		copy(tmp, val(i), size);
		for(j = i; j > l && compar(tmp, val(j - 1)) < 0; --j) copy(val(j), val(j - 1), size);
		copy(val(j), tmp, size);
	}
	free(tmp);
	return;
}

void introsort_loop(void *base, int l, int r, size_t size, int (*compar)(const void *, const void *), int dep) {
	if(l >= r) return;
	while(r - l > sort_threshold) {
		if(!dep) {
			partial_sort(base, l, r, size, compar);
			return;
		}
		--dep;
		swap(val(l), val(rnd(l, r)), size);
		int p = l , i = l + 1, j = r, q = r + 1;
		do {
			while(i <= r) {
				int op = compar(val(i), val(l));
				if(op > 0) break;
				if(op == 0 && (++p) != i) swap(val(p), val(i), size);
				++i;
			}
			while(j > l) {
				int op = compar(val(l), val(j));
				if(op > 0) break;
				if(op == 0 && (--q) != j) swap(val(q), val(j), size);
				--j;
			}
			if(i <= j) {
				if(i != j) swap(val(i), val(j), size);
				++i; --j;
			}
		} while(i <= j);
		while(p >= l)
			swap(val(--i), val(p--), size);
		while(q <= r)
			swap(val(++j), val(q++), size);
		introsort_loop(base, j + 1, r, size, compar, dep);
		r = i - 1;
	}
	insertion_sort(base, l, r, size, compar);
	return;
}

int bubble_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
	int i, flag = 0;
	for(i = 1; i < nmemb; ++i) {
		int op = compar(val(i), val(i - 1));
		if(op > 0) {
			if(!flag) flag = 1;
			else if(flag == 2) return 0;
		}
		if(op < 0) {
			if(!flag) flag = 2;
			else if(flag == 1) return 0;
		}
	}
	return (flag > 1) ? 2 : 1;
}

void my_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
	int op = bubble_sort(base, nmemb, size, compar);
	if(op == 1) return;
	if(op == 2) {
		int i, j;
		for(i = 0, j = nmemb - 1; i < j; ++i, --j)
			swap(val(i), val(j), size);
		return;	
	}
	introsort_loop(base, 0, nmemb - 1, size, compar, log2(nmemb) * 2);
	return;
}
