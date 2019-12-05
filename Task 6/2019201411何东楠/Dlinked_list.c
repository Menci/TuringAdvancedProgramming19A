#include <stdio.h>
#include <stdlib.h>
#include "Dlinked_list.h"

node* _delete(node *cur, int type)
{
	if (cur -> next == cur) {
		free(cur);
		return NULL;
	}

	(cur -> pre) -> next = cur -> next;
	(cur -> next) -> pre = cur -> pre;

	free(cur); //XXX without deleting the info in the adr of cur;

	return type ? cur -> next : cur -> pre; //XXX
}

node* _insert(node *lst, int curID)
{
	node* cur = (node *) malloc(sizeof(node));
	cur -> ID = curID;

	if (lst == NULL) {
		cur -> pre = cur;
		cur -> next = cur;
		return cur;
	}

	cur -> pre = lst;
	cur -> next = lst -> next;
	lst -> next = cur;

	return cur;
}

