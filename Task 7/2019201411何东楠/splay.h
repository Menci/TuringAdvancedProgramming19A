#ifndef SPLAY_H
#define SPLAY_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef struct node_of_splay
{
	int cnt;
	int val;
	int size;
	struct node_of_splay *ch[2], *fa;
}node;

node* get_root();

void initSet(node *cur);

void maintain(node *cur);

void rotate(node *cur, int type);

void splay(node *cur, node *anc);

node* find(int _val);

void insert(int _val);

node* neighbor_ele(int _val, int type);

void erase(int _val);

int find_kth(node *cur, int k);

int get_rank(int _val);

void cut_tree(node *cur);

#endif

