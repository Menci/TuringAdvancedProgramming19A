#include <stdio.h>
#include <malloc.h>
#include "splay.h"

int main()
{
	int n,i,o,x;
	struct splay *r = (struct splay*)malloc(sizeof(struct splay));
	init(r);
	scanf("%d", &n);
	for(i=0;i<n;i++)
	{
		scanf("%d%d",&o,&x);
		switch(o)
		{
			case 0:
				insert(r, x);
				break;
			case 1:
				del(r, x);
				break;
			case 2:
				printf("%d\n", getkth(r, x));
				break;
			case 3:
				printf("%d\n", getrnk(r, x));
				break;
			case 4:
				printf("%d\n", getrmax(r, x));
				break;
			case 5:
				printf("%d\n", getrmin(r, x));
				break;
		}
	}
	while(r -> root != NULL)
		del(r, r -> root -> num);
	free(r);
	return 0; 
}
