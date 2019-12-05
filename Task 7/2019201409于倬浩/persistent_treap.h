#ifndef _PERSISTENT_TREAP_H_
#define _PERSISTENT_TREAP_H_
#include "persistent_treap_node.h"
node* kthElement(int);
int rankElement(int);
void insert(int);
void erase(int);
int precessor(int);
int successor(int);
void clearTree(node*);
void cleanup();
#endif