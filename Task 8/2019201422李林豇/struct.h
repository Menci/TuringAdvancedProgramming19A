#ifndef _STRUCT_H
#define _STRUCT_H

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>

#define N 1000000
#define M 100000000
#define byte_size 8
#define byte_max 255
#define lowbit(x) ((x)^(-(x)))

struct Tree_node{
	int lc,rc;
	char data[_K]; 
}t_node[N+10];
typedef struct Tree_node T_node;

struct Trie_node{
	int pre;
	int num;					
	unsigned char c;						
}trie_node[N*10+10];
typedef struct Trie_node TR_node;

typedef int (*cmp_pointer)(const void *, const void *);

struct Treap_node{
	struct Treap_node *son[2];
	void *data;
	int cnt, siz, rnd;
}*root;
typedef struct Treap_node tp_node;

struct Treap{
	tp_node *root;
	cmp_pointer comp;
	int width;
};
typedef struct Treap Treap;

FILE *input, *output;

#endif