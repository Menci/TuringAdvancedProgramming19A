#include<stdlib.h>
#include<stdio.h>
#define SIZE 1
#define CHAR_MAX 255

extern void _add(char*, int, int);
struct huffman_node{
	struct huffman_node *left, *right;
	int count;
	int ord;
	int code, len;
};

struct huffman_node* huffman_code(struct huffman_node *p, char *_out_base, char *_base, int _lenth)
{
	struct huffman_node *_root  = NULL;
	int i, j;
	int use = (SIZE << 8);
	for(i = 0; i < use * 2; i += 1){
		(p + i)-> count = 0;
		(p + i)-> left  = NULL;
		(p + i)-> right = NULL;
		(p + i)-> ord   = i;
	}
	for(i = 0; i < _lenth; i += 1)
	{
		unsigned char num = *(_base + i);
		(p + num)-> count += 1;
	}
	struct huffman_node *_first  = malloc(sizeof(struct huffman_node));
	struct huffman_node *_second = malloc(sizeof(struct huffman_node));
	while(1)
	{
		_first-> count  = 1008610086;
		_second-> count = 1008610086;
		for(i = 0; i < use * 2; i += 1)
		{
			if( (p + i)-> count == 0) continue;
			if( (p + i)-> count < _first-> count)
			{
				_second = _first;
				_first  = (p + i); 
			}
			else if( (p + i)-> count < _second-> count)
			{
				_second = (p + i);
			}
		}
		
		if(_second-> count < 1008610086)
		{
			(p + use)-> count = _first-> count + _second-> count;
			(p + use)-> left  = _first;
			(p + use)-> right = _second;
			use ++;
		}
		else
		{
			_root = _first;
			break;
		}
	}
	
	return _root;
}

int huffman_dfs(int tot, int pre, int dep, struct huffman_node *place, char *_out_base)
{
	if(place-> left == NULL)
	{
		_add(_out_base, tot ++, 1);
		_add(_out_base, tot ++, 1);
		int i;
		for(i = SIZE * 8 - 1; i >= 0; i --){
			_add(_out_base, tot ++, (place-> ord >> i & 1));
		}
		place-> code = pre;
		place-> len  = dep;
		return tot;
	}
	_add(_out_base, tot ++, 1);
	_add(_out_base, tot ++, 0);
	tot = huffman_dfs(tot, pre << 1, dep + 1, place-> left, _out_base);
	_add(_out_base, tot ++, 0);
	_add(_out_base, tot ++, 1);
	tot = huffman_dfs(tot, pre *2+1, dep + 1, place->right, _out_base);
	_add(_out_base, tot ++, 0);
	_add(_out_base, tot ++, 0);
	return tot;
}
