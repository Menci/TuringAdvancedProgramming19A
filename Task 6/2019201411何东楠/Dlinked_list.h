#ifndef Dlinked_list
#define Dlinked_list 

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int ID;
	struct node *pre, *next;
}node;

node* _delete(node *cur, int type);
node* _insert(node *lst, int curID);

#endif

