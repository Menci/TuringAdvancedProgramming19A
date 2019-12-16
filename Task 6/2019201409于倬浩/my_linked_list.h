#ifndef _MY_LINKED_LIST_H_
#define _MY_LINKED_LIST_H_
#include <stdlib.h>

typedef struct linked_list_node_t {
	struct linked_list_node_t *pre, *nxt;
	int val;
} node;

node *newNode(int);
void deleteNode(node *);
void eraseNode(node *);
void insertNode(node *, node *);
void initializeHead();
#endif