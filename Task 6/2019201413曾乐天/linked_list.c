#include <stdlib.h>

typedef struct list
{
	int key;
	struct list *Last, *Next;
}list;

void link(list *a, list *b)
{
	a->Next = b;
	b->Last = a;
}

void del(list *a)
{
	a->Last->Next = a->Next;
	a->Next->Last = a->Last;
	free(a);
}

list* newlist(int key)
{
	list *ret = malloc(sizeof(list));
	ret->Next = ret->Last = NULL;
	ret->key = key;
}

list* move(list* cur, int type)
{
	if(type == 0)
		return cur->Next;
	return cur->Last;
}

int value(list *a)
{
	return a->key;
}
