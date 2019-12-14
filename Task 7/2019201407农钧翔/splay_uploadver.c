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

void rotate(node *x, splay_tree *tree) {
    int type = sontype(x);
    node *p = x -> p, *g = p -> p, *b = x -> son[!type];
    x -> p = g, g -> son[sontype(p)] = x; if(tree -> root == p) tree -> root = x;
    p -> p = x, x -> son[!type] = p;
    if(b != NULL) b -> p = p; p -> son[type] = b;
    update(p), update(x);
}

void splay(node *x, node *dest, splay_tree *tree) {
    while(x -> p != dest) {
        if(x -> p -> p == dest) rotate(x, tree);
        else if(sontype(x) == sontype(x -> p)) rotate(x -> p, tree), rotate(x, tree);
        else rotate(x, tree), rotate(x, tree);
    }
}

int get_rank(node *now, int val) {
    if(now == NULL) return 0;
    if(now -> val > val) return get_rank(now -> son[0], val);
    if(now -> val < val) return get_rank(now -> son[1], val) + now -> cnt + get_size(now -> son[0]);
    return get_size(now -> son[0]);
}

node *search_by_rank(node *now, int rank) {
    if(rank <= get_size(now -> son[0])) return search_by_rank(now -> son[0], rank);
    if(rank > now -> size - get_size(now -> son[1])) return search_by_rank(now -> son[1], rank - (now -> size - get_size(now -> son[1])));
    return now;
}

void Insert(int val, splay_tree *tree) {
    int rank = get_rank(tree -> root, val);
    node *x = (rank == tree -> root -> size) ? tree -> s2 : search_by_rank(tree -> root, rank + 1);
    splay(x, tree -> rf, tree);
    if(x -> val == val) {
        x -> cnt ++;
        x -> size ++;
        return ;
    }
    if(rank != 0) splay(search_by_rank(tree -> root, rank), tree -> root, tree);
    node *now = (node *) malloc(sizeof(node));
    now -> size = now -> cnt = 1, now -> val = val;
    tree -> root -> son[0] -> son[1] = now, now -> p = tree -> root -> son[0];
    update(tree -> root -> son[0]), update(tree -> root);
}

void Delete(int val, splay_tree *tree) {
    int rank = get_rank(tree -> root, val);
    node *now = search_by_rank(tree -> root, rank + 1);
    if(now -> cnt > 1) {
        splay(now, tree -> rf, tree);
        now -> cnt --;
        now -> size --;
        return ;
    }
    node *x = (rank == 0) ? tree -> s1 : search_by_rank(tree -> root, rank);
    node *y = (rank + now -> cnt == tree -> root -> size) ? tree -> s2 : search_by_rank(tree -> root, rank + now -> cnt + 1);
    splay(x, tree -> rf, tree);
    splay(y, tree -> root, tree);
    y -> son[0] = NULL;
    free(now);
    update(y), update(x);
}

node *get_prev(int val, splay_tree *tree) {
    int rank = get_rank(tree -> root, val);
    return (rank == 0) ? &_void_node : search_by_rank(tree -> root, rank);
}

node *get_next(int val, splay_tree *tree) {
    int rank = get_rank(tree -> root, val);
    if(rank == tree -> root -> size) return &_void_node;
    node *now = search_by_rank(tree -> root, rank + 1);
    if(now -> val > val) return now;
    return (rank + now -> cnt == tree -> root -> size) ? &_void_node : search_by_rank(tree -> root, rank + now -> cnt + 1);
}

void init_tree(splay_tree *tree) {
    tree -> rf = (node *) malloc(sizeof(node));
    tree -> s1 = tree -> root = (node *) malloc(sizeof(node));
    tree -> s2 = (node *) malloc(sizeof(node));
    tree -> rf -> val = _VAL_BELOW_MIN, tree -> rf -> son[1] = tree -> s1;
    tree -> s1 -> val = _VAL_BELOW_MIN, tree -> s1 -> son[1] = tree -> s2, tree -> s1 -> p = tree -> rf;
    tree -> s1 -> size = tree -> s1 -> cnt = 0;
    tree -> s2 -> val = _VAL_BEYOND_MAX, tree -> s2 -> p = tree -> s1;
    tree -> s2 -> size = tree -> s2 -> cnt = 0;
}

#include <stdio.h>

int main() {
    int n, op, x;
    splay_tree *tree = malloc(sizeof(splay_tree));
    init_tree(tree);
    scanf("%d", &n);
    for(int i = 1; i <= n; i++) {
        scanf("%d%d", &op, &x);
        switch (op) {
        case 0:
            Insert(x, tree);
            break;
        case 1:
            Delete(x, tree);
            break;
        case 2:
            printf("%d\n", search_by_rank(tree -> root, x) -> val);
            break;
        case 3:
            printf("%d\n", get_rank(tree -> root, x));
            break;
        case 4:
            printf("%d\n", get_prev(x, tree) -> val);
            break;
        case 5:
            printf("%d\n", get_next(x, tree) -> val);
            break;
        }
    }
    return 0;
}