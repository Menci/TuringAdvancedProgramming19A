#ifndef _PERSISTENT_TREAP_NODE_H_
#define _PERSISTENT_TREAP_NODE_H_
#include <malloc.h>
#include <stdint.h>
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
#endif