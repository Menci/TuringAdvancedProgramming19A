

#include "basic.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

int get_root(node *tree, size_t n)
{
	bool *book = (bool *)calloc(n, sizeof(bool));
	int i;
	for (i = 0; i < n; ++i)
	{
		if (~tree[i].ls)
			book[tree[i].ls] = 1;
		if (~tree[i].rs)
			book[tree[i].rs] = 1;
	}
	for (i = 0; i < n; ++i)
		if(!book[i])
		{
			free(book);
			return i;
		}
	return 0;
}

int dfs_brckt(node *tree, int u, uchar *buffer, int bit)
{
	fill(buffer, bit, 0), ++bit;

	if (~u)
	{
		bit = dfs_brckt(tree, tree[u].ls, buffer, bit);
		bit = dfs_brckt(tree, tree[u].rs, buffer, bit);
	}

	fill(buffer, bit, 1);
	return bit + 1;
}

void get_brckt_seq(node *tree, int root, size_t n, FILE *output)
{
	uchar *buffer = (uchar *)malloc(sizeof(int)*n);
	int end = dfs_brckt(tree, root, buffer, 0) - 1;
	
	fwrite(&end, sizeof(int), 1, output);
	fwrite(buffer, sizeof(uchar), (end >> 3) + 1, output);
//	printf("SDFDSFSDFSD");
	free(buffer);
}

void decode_brckt_seq(int n, char *seq, FILE *output)
{
	int block_num = (n>>3) + 1;
	int *stk = (int *)malloc(sizeof(int) * n);
	bool *book = (bool *)calloc(n, sizeof(bool));
	node *tree =  (node *)malloc(sizeof(node) * n);
	int top = 0, now = 0;

	int i;
	for (i = 0; i < n; ++i)
	{
		int opt = read(seq, i);
		if (opt == 0)
			stk[++top] = now++;
		else
		{
			top--;
			if (top)
				book[stk[top]] ? (tree[stk[top]].rs = stk[top+1]) : \
				(tree[stk[top]].ls = stk[top-1]);
		}
	}

	fwrite(stk, sizeof(uchar), n, output);
}

void get_weight(node *tree, int u, FILE *output)
{
	fwrite(tree[u].w, sizeof(uchar), K, output);
	int v = tree[u].ls;
	if (~v)
		get_weight(tree, v, output);
	if (~(v = tree[u].rs))
		get_weight(tree, v, output);
}
