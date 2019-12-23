#include<stdio.h>
#define K 4
struct huffman_node{
	struct huffman_node *left, *right;
	int count;
	int ord;
	int code, len;
};
struct pair{
	int first;
	int second;
};
extern int get_val(char *, int);
struct pair huffman_decode(char *_base, struct huffman_node *root, struct pair now)
{
	int p  = now.second;
	int f1 = get_val(_base, now.first);
	int f2 = get_val(_base, now.first + 1);
	if(!f1 && !f2){
		now.first += 2;
		now.second ++;
		(root + p)-> count = now.first;
		now.first += 8;
		return now;
	}
	if(f1 && !f2){
		now.first += 2;
		now.second ++;
		(root + p)-> left = root + now.second;
		now = huffman_decode(_base, root, now);
	}
	
	f1 = get_val(_base, now.first);
	f2 = get_val(_base, now.first + 1);
	if(!f1 && f2){
		now.first += 2;
		now.second ++;
		(root + p)-> right = root + now.second;
		now = huffman_decode(_base, root, now);
	}
	
	f1 = get_val(_base, now.first);
	f2 = get_val(_base, now.first + 1);
	if(f1  && f2){
		now.first += 2;
		now.second ++;
		return now;
	}
}

