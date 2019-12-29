#include <stdio.h>
#define K 4
#define maxn 1000000

typedef struct node {
	int left, right;
	char data[K];
} node;

node tree[maxn];
