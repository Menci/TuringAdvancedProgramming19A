#include<stdlib.h>

struct DCList
{
	int ord;
	struct DCList *pre, *next;
};

struct DCList* DCLinit(int);
struct DCList* DCLnext(struct DCList*);
int DCLord(struct DCList*);
void DCLinsert(struct DCList *, int);
struct DCList* DCLdelete(struct DCList *);

struct DCList* DCLinit(int ord)
{
	int i;
	struct DCList* x = (struct DCList*) malloc(sizeof(struct DCList));
	x -> ord = ord;
	x -> pre = x;
	x -> next = x;
	
	return x;
}

struct DCList* DCLnext(struct DCList *x)
{
	return x -> next;
}

int DCLord(struct DCList *x)
{
	return x -> ord;
}

void DCLinsert(struct DCList *x, int ord)
{
	struct DCList* y =  (struct DCList*) malloc(sizeof(struct DCList));
	y -> ord = ord;
	
	y -> next = x -> next;
	x -> next -> pre = y;
	
	y -> pre = x;
	x -> next = y;
}

struct DCList* DCLdelete(struct DCList *x)
{
	if(x -> next == x)
	{
		free(x); 
		return NULL;
	}
	
	x -> pre -> next = x -> next;
	x -> next -> pre = x -> pre;
	
	free(x);
	return x -> next;
}
