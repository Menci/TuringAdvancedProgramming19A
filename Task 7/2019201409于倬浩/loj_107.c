#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct persistent_treap_node_t {
    struct persistent_treap_node_t *l, *r;
    int v, sz;
} node;
typedef struct treap_node_pair_t {
    node *a, *b;
} pii;
int xorShift();
node *newNode(int);
void deleteNode(node *);
void initialize();
void pushup(node *);
pii split(node *, int);
node *merge(node *, node *);
node *null, *root;
int xorShift() {
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123;
    uint32_t t;
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    w = w ^ (w >> 19) ^ (t ^ (t >> 8));
    return w & 0x7fffffff;
}
node *newNode(int v) {
    node *ret = (node *)malloc(sizeof(node));
    ret->l = ret->r = null;
    ret->v = v, ret->sz = 1;
    return ret;
}
void deleteNode(node *x) {
    if (x != null) free(x);
}
void initialize() {
    null = (node *)malloc(sizeof(node));
    null->l = null->r = null;
    null->v = null->sz = 0;
    root = null;
}
void pushup(node *x) {
    if (x == null) return;
    x->sz = x->l->sz + x->r->sz + 1;
}
pii split(node *rt, int k) {
    if (rt == null || k == 0) return (pii){null, rt};
    if (k == rt->sz) return (pii){rt, null};
    pii ret;
    if (k >= rt->l->sz + 1) {
        ret = split(rt->r, k - rt->l->sz - 1);
        rt->r = ret.a, pushup(rt);
        ret.a = rt;
    } else {
        ret = split(rt->l, k);
        rt->l = ret.b, pushup(rt);
        ret.b = rt;
    }
    return ret;
}
node *merge(node *a, node *b) {
    if (a == null) return b;
    if (b == null) return a;
    if (xorShift() % (a->sz + b->sz) < a->sz) {
        a->r = merge(a->r, b);
        pushup(a);
        return a;
    } else {
        b->l = merge(a, b->l);
        pushup(b);
        return b;
    }
}
node *kthElement(int k) {
    node *rt = root;
    int lsize;
    while (rt != null) {
        lsize = rt->l->sz + 1;
        if (lsize == k) return rt;
        if (k > lsize)
            k -= lsize, rt = rt->r;
        else
            rt = rt->l;
    }
    return rt;
}
int rankElement(int x) {
    int ret = 0;
    node *rt = root;
    while (rt != null) {
        if (x > rt->v)
            ret += rt->l->sz + 1, rt = rt->r;
        else
            rt = rt->l;
    }
    return ret;
}
void insert(int x) {
    pii t = split(root, rankElement(x));
    root = merge(t.a, merge(newNode(x), t.b));
}
void erase(int x) {
    pii t1 = split(root, rankElement(x)), t2 = split(t1.b, 1);
    root = merge(t1.a, t2.b);
    deleteNode(t2.a);
}
int precessor(int x) {
    int rnk = rankElement(x);
    if (rnk == 0) return -1;
    node *t = kthElement(rnk);
    return t->v;
}
int successor(int x) {
    int rnk = rankElement(x + 1);
    if (rnk == root->sz) return -1;
    node *t = kthElement(rnk + 1);
    return t->v;
}
int main() {
    initialize();
    int op, x, n;
    scanf("%d", &n);
    while (n--) {
        scanf("%d%d", &op, &x);
        if (op == 0)
            insert(x);
        else if (op == 1)
            erase(x);
        else if (op == 2)
            printf("%d\n", kthElement(x)->v);
        else if (op == 3)
            printf("%d\n", rankElement(x));
        else if (op == 4)
            printf("%d\n", precessor(x));
        else
            printf("%d\n", successor(x));
    }
    return 0;
}