#include <stdio.h>
#include <stddef.h>
#include "linked_list.h"

extern struct linked_list;
typedef struct linked_list linked_list;

extern linked_list *delet(linked_list *);
extern linked_list *init_list(int, int);
extern linked_list *get_next(linked_list *);

void solve(int n, int k, int type)
{
	int i;
	linked_list *now = init_list(n, type);

	while(now != NULL)
	{
		for(i = 0; i < k; ++i)
			now = get_next(now);
		now = delet(now);
	}
	puts("");
}

