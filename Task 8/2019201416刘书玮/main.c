#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"main.h"

int main()
{
	int n = 240000, k = 8;
	struct tree *_tree_base = malloc((k + 8) * n);
	input(_tree_base, n, k);
	char* _out_base  = malloc((k + 8) * n + 1024);
	char *_data_base = malloc(k * n);
	int lenth = 0;
	int i, j;
	lenth = bracket_dfs(_tree_base, 0, _out_base, _data_base, 0, 0, k).first;
	struct huffman_node *_huff_base = malloc(32768);
	struct huffman_node *root = huffman_code(_huff_base, _out_base, _data_base, k * n);
	fprintf(stderr,"%d\n",lenth);
	lenth = huffman_dfs(lenth, 0, 0, root, _out_base);
	fprintf(stderr,"%d\n",lenth);
	lenth = compress(lenth, _huff_base, _data_base, _out_base, k*n);
	fprintf(stderr,"%d\n",lenth);
	output(_out_base, lenth);
	
	free(_out_base);
	free(_data_base);
	free(_huff_base);
	return 0;
}
