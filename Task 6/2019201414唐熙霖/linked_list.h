#include <stdio.h>
#include <stdlib.h>

typedef struct linked_list
{
	int w;
	struct linked_list *next, *pre;
}linked_list;

linked_list *delet(linked_list *);
linked_list *init_list(int ,int);
linked_list *get_next(linked_list *);

linked_list *delet(linked_list *a)
{
	printf("%d ", a -> w);
	if(a -> next == a)
		return free(a), NULL;

	a -> pre -> next = a -> next;
	a -> next -> pre = a -> pre;
	linked_list *res = a -> next;
	return free(a), res;
}

linked_list *init_list(int n, int k)
{
	int i;
	linked_list *now, *last, *fir = (linked_list *)malloc(sizeof(linked_list));
	fir -> w = 1;
	last = fir;

	switch(k)
	{
		case 0:
			for(i = 2; i <= n; ++i)
			{
				now  = (linked_list *)malloc(sizeof(linked_list));
				now -> w = i;
				now -> pre = last,
				last -> next = now;
				last = now;
			}
			now -> next = fir;
			fir -> pre = now;
			break;

		case 1:
			for(i = 2; i <= n; ++i)
			{
				now  = (linked_list *)malloc(sizeof(linked_list));
				now -> w = i;
				now -> next = last,
				last -> pre = now;
				last = now;
			}
			now -> pre = fir;
			fir -> next = now;
			break;
	}
	return fir;
}

linked_list *get_next(linked_list *a)
{
	return a -> next;
}
