#include<stdio.h>
#include<stdlib.h>

struct node
{
	struct node *head,*nxt;
	int data;
};



struct node A;
struct node *last , *now ,*Null = &A;

void CreatNew(int data)
{
	struct node *X = malloc(sizeof(struct node));
	X->head = last;
	last->nxt = X;
	X->nxt = Null;
	X->data = data;
	last = X;
}


void init()
{
	Null->head = Null->nxt = Null;
	Null->data = 0 ;
	last  = Null;
	now = Null;
}

void Move()
{
	extern int type , k;
	int step = k;
	if( type == 0 )
		while( step-- ) now = now->nxt;
	else
		while( step-- ) now = now->head;	
	printf( "%d\n" , now->data );
	now->nxt->head = now->head;
	now->head->nxt = now->nxt;
	last = now;
	now = now->nxt;
	free(last);
}
void Prt()
{
	printf( "%d\n" , now->data );
	free(now);
}
void Endlink()
{
	now = now->nxt;
	last->nxt = now;
	now->head = last;
	
}
