// Scapegoat_tree.h

#ifndef SPG
#define SPG

#include <stdio.h>
#include <malloc.h>


#define NUM_OF_SON 2
#define MAX_STK 100100

#define inf 0X3f3f3f3f
#define alpha 0.75

typedef double db;

typedef struct node
{
	int val, sz;
	struct node *fa, *son[NUM_OF_SON];
}node;

typedef struct scapegoat_tree
{
	int tot;
	node *root;
}spg;

typedef struct stack_for_spg
{
	int tot;
	node *el[MAX_STK];
}spgstk;

spgstk *new_stk();

node *new_node();
spg *initial();
void termination(spg *, spgstk *);

int balance(node *);
void seek(spgstk *, node *);
node *build(spgstk *stk, int, int);
void rebuild(spg *, spgstk *, node *);
void insert(spg *, spgstk *, int);
void del(spg *, node *);
node *get_pos(spg *, int);

int query_rank(spg *, int);
int query_kth(spg *, int);
int query_pre(spg *, int);
int query_sub(spg *, int);

#endif

