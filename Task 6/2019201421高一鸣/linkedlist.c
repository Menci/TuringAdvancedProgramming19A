#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
void* newNode(size_t size);


static struct linkedList *add(struct linkedList *pos,struct Data *data);
static struct linkedList *delete(struct linkedList *pos);

struct linkedList *newLinkedList(struct Data *data) {
	struct linkedList* cur=newNode(sizeof(struct linkedList));
	cur->nxt=cur,cur->pre=cur,cur->data=data,cur->add=add,cur->delete=delete;
	return cur;
}

static struct linkedList *add(struct linkedList *pos,struct Data *data) {
	struct linkedList* cur=newLinkedList(data);
	pos->nxt->pre=cur,cur->nxt=pos->nxt;
	pos->nxt=cur,cur->pre=pos;
	return cur;
}
static struct linkedList *delete(struct linkedList *pos) {
	if(pos->nxt==pos) return NULL;
	pos->nxt->pre=pos->pre;
	pos->pre->nxt=pos->nxt;
	return pos->nxt;
}