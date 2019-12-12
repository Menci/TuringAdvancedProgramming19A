#ifndef _METAKAR1_TREAP_H
#define _METAKAR1_TREAP_H

#include <stdlib.h>

typedef struct node {
    int size;
    int data;
    int key;
    struct node *lch, *rch;
}node;

typedef struct treap {
    node *root;
}treap;

typedef struct {
    node *first, *second;
}pair;

node *NewNode(int);
treap *CreateTreap();
void DeleteTreap(treap *);
node *Merge(node *, node *);
pair Split(node *, int);
int GetRank(node *, int);
void InsertNode(treap *, int);
void DeleteNode(treap *, int);
int GetKth(node *, int);
int Predecessor(node *, int);
int Successor(node *, int);

#endif