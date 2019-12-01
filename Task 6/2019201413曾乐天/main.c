#include <stdio.h>

extern void link(struct list*, struct list*);
extern struct list* move(struct list*, int);
extern struct list* newlist(int);
extern int value(struct list*);
extern void del(struct list*);

int read(char *s)
{
	int x = 0;char c = *s;
	while(c < '0' || '9' < c)c = *++s;
	while('0' <= c && c <= '9')x = x * 10 + (c ^ 48), c = *++s;
	return x;
}

int main(int argc,char *argv[])
{
	int type, n, k, i;
	
	if(argc == 1)
		scanf("%d%d%d",&n,&k,&type);
	else if(3 <= argc && argc <= 4)
	{
		n = read(argv[1]);
		k = read(argv[2]);
		type = (argc == 4) ? read(argv[3]) : 0;
	}
	else
	{
		fprintf(stderr,"Error: argument count not match\n");
		return -1;
	}

	//type = 0:clockwise, type = 1:counerclockwise
	struct list *start = newlist(1), *cur = start;
	for(i = 2;i <= n; i++)
	{
		link(cur,newlist(i));
		cur = move(cur, 0);
	}
	link(cur,start);
	
	cur = start;
	for(;n>1;n--)
	{
		for(i = 1; i <= k; i++)
			cur = move(cur, type);
		printf("%d ", value(cur));
		
		cur = move(cur, type);
		del(move(cur, type ^ 1));
	}
	printf("%d\n",value(cur));

	return 0;
}
