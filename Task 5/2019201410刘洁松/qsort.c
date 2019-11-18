#include <stdio.h>
#include <stdbool.h>

#define CUTOFF 8
#define STKSIZE 100

int my_int_compare(void *a, void *b);
void swap(char *a, char *b, size_t width);
void shortsort(char *lo, char *hi, size_t width, int (*cmp)(void* a, void* b));

bool sorted(char *lo, char *hi, size_t width, int (*cmp)(void *a, void *b)) {
	char *tmp = lo;
	for(tmp = lo + width; tmp <= hi; tmp += width) {
		if(cmp(tmp, tmp - width) < 0) return false;
	}
	return true;
}

bool reverse_sorted(char *lo, char *hi, size_t width, int (*cmp)(void *a, void *b)) {
	char *tmp = lo;
	for(tmp = lo + width; tmp <= hi; tmp += width) {
		if(cmp(tmp, tmp - width) > 0) return false;
	}
	return true;
}

void adjust(char *lo, char *hi, size_t width) {
	while(lo < hi) {
		swap(lo, hi, width);
		lo += width;
		hi -= width;
	}
}

void my_qsort(void *base, size_t size, size_t width, 
						  int(*cmp)(void *a, void *b)) {
	char *lo, *hi;
	char *loguy, *higuy;
	char *mid;
	int stkptr = 0;

	char *lostk[STKSIZE], *histk[STKSIZE];

	if(size < 2)
		return;

	lo = (char*) base;
	hi = (char*) base + (size - 1) * width;

	if(sorted(lo, hi, width, cmp)) {
		return;
	}
	if(reverse_sorted(lo, hi, width, cmp)) {
		adjust(lo, hi, width);
		return;
	}

recurse:

	size = (hi - lo) / width + 1;
	mid = lo + (size / 2) * width;

	if(size <= CUTOFF) {
		shortsort(lo, hi, width, cmp);
	} else {

		if((*cmp)(lo, mid) > 0) {
			swap(lo, mid, width);
		}
		if((*cmp)(lo, hi) > 0) {
			swap(lo, hi, width);
		}
		if((*cmp)(mid, hi) > 0) {
			swap(mid, hi, width);
		}

		loguy = lo;
		higuy = hi;

		for(;;) {
			if(loguy < mid) {
				do {
					loguy += width;
				} while(loguy < mid && (*cmp)(loguy, mid) <= 0);
			}

			if(loguy >= mid) {
				do {
					loguy += width;
				} while(loguy <= hi && (*cmp)(loguy, mid) <= 0);
			}

			if(higuy > mid) {
				do {
					higuy -= width;
				} while(higuy > mid && (*cmp)(higuy, mid) > 0);
			}

			if(loguy > higuy) break;

			swap(loguy, higuy, width);
			
			if(mid == higuy)
				mid = loguy;
		}

		higuy += width;
		
		if(higuy > mid) {
			do {
				higuy -= width;
			} while(higuy > mid && (*cmp)(higuy, mid) == 0);
		}

		if(higuy <= mid) {
			do {
				higuy -= width;
			} while(higuy >= lo && (*cmp)(higuy, mid) == 0);
		}

		/*
		if(lo < higuy)
			my_qsort(lo, (higuy - lo) / width + 1, width, cmp);

		if(loguy < hi)
			my_qsort(loguy, (hi - loguy) / width + 1, width, cmp);
		*/

		if(higuy - lo >= hi - loguy) {
			if(lo < higuy) {
				lostk[stkptr] = lo;
				histk[stkptr] = higuy;
				++ stkptr;
			}
			if(loguy < hi) {
				lo = loguy;
				goto recurse;
			}
		} else {
			if(loguy < hi) {
				lostk[stkptr] = loguy;
				histk[stkptr] = hi;
				++ stkptr;
			}
			if(lo < higuy) {
				hi = higuy;
				goto recurse;
			}
		}
	}

	-- stkptr;
	if(stkptr >= 0) {
		lo = lostk[stkptr];
		hi = histk[stkptr];
		goto recurse;
	} 
	else
		return;
}

void shortsort(char *lo, char *hi, size_t width, int (*cmp)(void *a, void *b)) {
	char *max, *tmp;
	while(lo < hi) {
		max = lo;
		for(tmp = lo + width; tmp <= hi; tmp += width) {
			if((*cmp)(tmp, max) > 0) {
				max = tmp;
			}
		}
		if(max != hi) {
			swap(max, hi, width);
		}
		hi -= width;
	}
}

int my_int_compare(void *a, void *b) {
	return *(int*)a - *(int*) b;
}

void swap(char *a, char *b, size_t width) {
	char tmp;
	if(a != b) {
		while(width --) {
			tmp = *a;
			*a++ = *b;
			*b++ = tmp;
		}
	}
}
