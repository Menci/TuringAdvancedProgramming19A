#include "everything.h"

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
    node *now = malloc(sizeof(node));
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
    tree -> rf = malloc(sizeof(node));
    tree -> s1 = tree -> root = malloc(sizeof(node));
    tree -> s2 = malloc(sizeof(node));
    tree -> rf -> val = _VAL_BELOW_MIN, tree -> rf -> son[1] = tree -> s1;
    tree -> s1 -> val = _VAL_BELOW_MIN, tree -> s1 -> son[1] = tree -> s2, tree -> s1 -> p = tree -> rf;
    tree -> s1 -> size = tree -> s1 -> cnt = 0;
    tree -> s2 -> val = _VAL_BEYOND_MAX, tree -> s2 -> p = tree -> s1;
    tree -> s2 -> size = tree -> s2 -> cnt = 0;
}