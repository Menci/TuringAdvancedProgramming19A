#include<stdlib.h>
#include<stdio.h>
#define SIZE 1

extern void _add(char*, int, int);

struct huffman_node{
	struct huffman_node *left, *right;
	int count;
	int ord;
	int code, len;
};

int compress(int lenth, struct huffman_node* _huff_base, char* _data_base, char* _out_base, int tot)
{
	int i, j;
	for(i = 0; i < tot; i ++){
		unsigned char data = *(_data_base + i);
		struct huffman_node node = *(_huff_base + data);
		for(j = node.len - 1; j >= 0; j --){
			_add(_out_base, lenth ++, node.code >> j & 1);
		}
	}
	return lenth;
}
