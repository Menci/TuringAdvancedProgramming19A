#include "basic.h"
#include <limits.h>

inline size_t calc_file_size(FILE *a)
{
	fseek(a, 0L, SEEK_END);
	size_t res = ftell(a);
	fseek(a, 0L, SEEK_SET);
	return res;
}

void fill(uchar *buffer, int bit, int val)
{
	int i = bit >> 3;
	bit &= 7;
	buffer[i] &= (UCHAR_MAX ^ (1 << bit));
	buffer[i] |= (val << bit);
}
