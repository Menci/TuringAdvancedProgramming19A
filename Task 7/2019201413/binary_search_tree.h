#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef struct xxx
{
	int x, cnt, siz;
	struct xxx *fa, *ch[2];
}node;

node* insert(int x);
node* dec(int x);
int findkth(int k);
int query(int x, int ty);
int findrk(int x);
void freeall(node *x);

int read();
int writes(int x);

#endif
