#ifndef _EVERYTHING_INCLUDED
#define _EVERYTHING_INCLUDED

#include <stdlib.h>

struct _node;
struct _splay_tree;

typedef struct _node {
    int size, cnt, val;
    struct _node *son[2], *p;
} node;

typedef struct _splay_tree {
    struct _node *rf, *root, *s1, *s2;
} splay_tree;

static node _void_node  = {0, 0, -1, NULL, NULL};

#define _VAL_BELOW_MIN -1
#define _VAL_BEYOND_MAX 0x3f3f3f3f

#define sontype(x) ((x -> p -> son[1] == x) ? 1 : 0)
#define get_size(x) ((x == NULL) ? 0 : x -> size)
#define update(x) x -> size = get_size(x -> son[0]) + get_size(x -> son[1]) + x -> cnt

void rotate(node *x, splay_tree *tree);
void splay(node *x, node *dest, splay_tree *tree);

int get_rank(node *now, int val);
node *search_by_rank(node *now, int id);

void Insert(int val, splay_tree *tree);
void Delete(int val, splay_tree *tree);

node *get_prev(int val, splay_tree *tree);
node *get_next(int val, splay_tree *tree);

void init_tree(splay_tree *tree);

#endif

