#include <stdio.h>

extern struct DCList* DCLinit(int);
extern struct DCList* DCLnext(struct DCList*);
extern int DCLord(struct DCList*);
extern void DCLinsert(struct DCList *, int);
extern struct DCList* DCLdelete(struct DCList *);

int main()
{
	int n, k, i;
	scanf("%d%d", &n, &k);
	
	struct DCList* x = DCLinit(1);
	while(n > 1)
		DCLinsert(x, n--);
	
	while(x)
	{
		for(i = 0; i < k; i++)
			x = DCLnext(x);
		printf("%d\n", DCLord(x));
		x = DCLdelete(x);
	}
	
	return 0;
}
