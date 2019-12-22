#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "basic.h"
#include "hulffman.h"
#include "bracket_sequence.h"

bool check_tree(node *tree, int u, node *new_tree, int v)
{
	if (u == -1)
	{
		if (v == -1)
			return 1;
		return 0;
	}
	int i;
	bool flag = 0;
	for (i = 0; i < K; ++i)
		if(tree[u].w[i] != new_tree[v].w[i])
			return 1;
	return check_tree(tree, tree[u].ls, new_tree, new_tree[v].ls) \
		| check_tree(tree, tree[u].rs, new_tree, new_tree[v].rs);
}

bool check(node *tree, node *new_tree, size_t n)
{
	int root = get_root(tree, n);
	int new_root = get_root(new_tree, n);
	return check_tree(tree, root, new_tree, new_root);
}

int main(int argc, char *argv[])
{
	FILE *file1 = fopen(argv[1], "rb");
	FILE *file2 = fopen("zipped.out", "rb");

	size_t siz = calc_file_size(file1);
	size_t n = siz / (sizeof(node));

	node *tree = (node *)malloc(siz), *new_tree;new_tree = tree;
	fread(tree, sizeof(node), n, file1);

	if (check(tree, new_tree, n))
	{
		puts("Accepted");
		printf("%lf\n", 1.0*calc_file_size(file2) / (1.0*calc_file_size(file1)));
	}
	else 
		puts("Lossy");
	
	return 0;
}
