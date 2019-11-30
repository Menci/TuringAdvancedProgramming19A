#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

void* temp[100010];

void qsort_sort (void *Aarray, size_t Capacity, size_t Width, int (*fcmp) (const void*, const void*) ) {

	if (Capacity <= 1) return;

	int mid = Capacity >> 1;
	if (mid && mid != 1) {
		memcpy (temp, Aarray + (mid - 1) * Width, Width);
		memcpy (Aarray + (mid-1) * Width, Aarray, Width);
		memcpy (Aarray, temp, Width);
	}

	memcpy (temp, Aarray, Width);
	int pos_left = 1, pos_right = Capacity;
	
	while (pos_left < pos_right) {

		while (pos_left < pos_right && fcmp (Aarray + (pos_right-1) * Width, temp) >= 0) -- pos_right;
		if (pos_left < pos_right) { 
			memcpy (Aarray + (pos_left-1) * Width, Aarray + (pos_right-1) * Width, Width);
			++ pos_left;
		}

		while (pos_left < pos_right && fcmp (Aarray + (pos_left-1) * Width, temp) < 0) ++ pos_left;
		if (pos_left < pos_right) {
			memcpy (Aarray + (pos_right-1) * Width, Aarray + (pos_left-1) * Width, Width);
			-- pos_right;
		}

	} memcpy (Aarray + (pos_left-1) * Width, temp, Width);
	
	while (pos_left > 1 && fcmp (Aarray + (pos_left - 2) * Width, Aarray + (pos_left - 1) * Width) == 0) -- pos_left;
	while (pos_right < Capacity && fcmp (Aarray + (pos_right - 1) * Width, Aarray + (pos_right) * Width) == 0) ++ pos_right;

	if (pos_left > 0) qsort_sort(Aarray, pos_left - 1, Width, fcmp);
	if (pos_right < Capacity) qsort_sort(Aarray + (pos_right) * Width, Capacity - pos_right, Width, fcmp);
}

void Qsort (void *Aarray, size_t Capacity, size_t Width, int (*fcmp) (const void*, const void*) ) {
	for (int i = 0; i < Capacity - 1; ++i) {
		if (fcmp (Aarray + i * Width, Aarray + (i+1) * Width) > 0) {
			qsort_sort (Aarray, Capacity, Width, fcmp);
			return ;
		}
	}
}

int main () {
	return 0;
}
