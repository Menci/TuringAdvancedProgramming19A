#ifndef _STRUCT_H
#define _STRUCT_H

typedef int (*cmp_pointer)(const void *, const void *);

struct Tree_node{
	struct Tree_node *son[2];
	void *data;
	int cnt, siz, rnd;
}*root;
typedef struct Tree_node t_node;

struct Treap{
	t_node *root;
	cmp_pointer comp;
	int width;
};
typedef struct Treap Treap;

#endif