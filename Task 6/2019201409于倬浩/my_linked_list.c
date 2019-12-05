#include "my_linked_list.h"
#include <stdlib.h>
node *newNode(int val) {
	node *ret = (node *)malloc(sizeof(node));
	ret->pre = ret->nxt = NULL;
	ret->val = val;
	return ret;
}
void deleteNode(node *x) {
	if (x != NULL) free(x);
}
void eraseNode(node *x) {
	if (x == NULL) return;
	node *pre = x->pre, *nxt = x->nxt;
	if (pre != NULL) pre->nxt = nxt;
	if (nxt != NULL) nxt->pre = pre;
	deleteNode(x);
}
void insertNode(node *pre, node *new) {
	if (pre == NULL) return;
	node *nxt = pre->nxt;
	pre->nxt = new;
	new->pre = pre;
	new->nxt = nxt;
	if (nxt != NULL) nxt->pre = new;
}
void initializeHead(node **head) {
	*head = newNode(1);
	(*head)->pre = (*head)->nxt = (*head);
}