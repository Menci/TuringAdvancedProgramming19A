#include "persistent_treap_node.h"
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