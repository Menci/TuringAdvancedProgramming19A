#ifndef BASIC
#define BASIC

#ifndef K
#define K 0
#endif

#include <stdio.h>
#define uchar unsigned char

typedef struct node
{
	int ls, rs;
	uchar w[K];
}node;

size_t calc_file_size(FILE *);
void fill(uchar *, int, int);

#endif
