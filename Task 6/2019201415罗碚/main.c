#include <stdio.h>

extern struct DCList* DCLinit(int);
extern struct DCList* DCLnext(struct DCList*);
extern int DCLord(struct DCList*);
extern void DCLinsert(struct DCList *, int);
extern struct DCList* DCLdelete(struct DCList *);

int main(int argc, char* argv[])
{
	if(argc != 3 && argc != 5)
	{
		printf("Right input: %s n k [-c x](x means clockwise or anticlockwise, 1(default) means clockwise needle, 0 means anticlockwise)\n", argv[0]);
		return 0;
	}
	
	int n = -1, k = -1, xt = 1, i;
	sscanf(argv[1], "%d", &n);
	sscanf(argv[2], "%d", &k);
	if(argc == 5)
		sscanf(argv[4], "%d", &xt);
	
	if(n < 0 || k < 0 || (xt != 0 && xt != 1))
	{
		printf("Right input: %s n k [-c x](x means clockwise or anticlockwise, 1(default) means clockwise needle, 0 means anticlockwise)\n", argv[0]);
		return 0;
	}
	
	struct DCList* x = DCLinit(1);
	for(i = n; i > 1; i--)
		DCLinsert(x, i);
	
	while(x)
	{
		for(i = 0; i < k; i++)
			x = DCLnext(x);
		if(xt)
			printf("%d\n", DCLord(x));
		else
			printf("%d\n", DCLord(x) == 1? 1: n + 2 - DCLord(x));
		x = DCLdelete(x);
	}
	
	return 0;
}