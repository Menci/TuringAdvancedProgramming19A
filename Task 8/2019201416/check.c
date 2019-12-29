#include<stdlib.h>
#include<stdio.h>
#include"check.h"

int main()
{
	int n = 240000, k = 8;
	int size = n * (k + 8);
	char *_base     = malloc(size);
	char *_out_base = malloc(size);
	struct tree *root = malloc(size);
	struct tree *_ret = malloc(size);
	int tot;
	tot  = deinput(_base, size);
	int used = (tot - 2) * 8 + (*_base);
	int lenth = 0;
	
	if(!(*_base) ) used += 8;
	_base += 1;
	
	struct pair now;
	now.first  = lenth;
	now.second =0;
	
	lenth = debracket(_base, root, now).first;
	
	deoutput((char*)root, n * (k + 8));
	if(lenth > size){
		fprintf(stderr, "Test Wrong!\n");
		return 1;
	}
	
	struct huffman_node *_huff_base = malloc(32768);
	char *_code_base = malloc(32768);
	now.first  = lenth;
	now.second = 0;
	lenth = huffman_decode(_base, _huff_base, now).first;
	
	while(used < lenth){
		now.first  = lenth;
		now.second = 0;
		lenth = huffman_dfs(_base, _out_base, _huff_base, now).first;
	}
	tree_dfs(_out_base, root, 0, 0);
	
	
	if(lenth > size * 8){
		fprintf(stderr, "Test Wrong!\n");
		return 1;
	}
	
	printf("Correct!\n");
	printf("Your compression rate is %.2lf%%!\n", 100.0 * tot / size);
	deoutput((char*)root, size);
	return 0;
}
