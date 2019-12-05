#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "link.h"

void print_link(int siz, struct Link *Head) {
	for (int i = 1; i <= siz; ++i) {
		Head = Head->next;
		printf("%d ",Head->id);
	}
	printf("\n");
}
struct Link *clear(struct Link *p) {
	p->id = 0;
	p->next = p->last = p;
	return p;
}
void add(int key, struct Link *Now) {
	struct Link *tmp = malloc(sizeof(struct Link));
	tmp->id = key;
	tmp->next = tmp->last = NULL;
	
	Now->next = tmp;
	(Now->next)->last = Now;
}
void del(struct Link *p, struct Link *Head) {
	if ((p->next)->id == p->id) {
		free(p); clear(Head);
	}
	else {
		struct Link * Last, *Next;
		Last = p->last; Next = p->next;
		free(p);
		Last->next = Next;
		Next->last = Last;
	}
}
