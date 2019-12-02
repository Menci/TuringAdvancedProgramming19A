#include<stdio.h>

extern struct _dll_node *_dll_build(int);
extern struct _dll_node *_dll_find(struct _dll_node *, int);
extern struct _dll_node *_reverse_dll_find(struct _dll_node *, int);
extern void _dll_delete(struct _dll_node *);
extern int _dll_getv(struct _dll_node *);

void solve(int n,int m,int ord)
{
	int i;
	struct _dll_node *now;
	
	for(i = 1; i <= n; i ++)
	{
		now = _dll_build(i);
	}
	
	for(i = 1; i <= n; i ++)
	{
		if(i == 1){
			if(ord == 1) now = _dll_find(now, m);
			else now = _reverse_dll_find(now, m);
		}
		else{
			if(ord == 1) now = _dll_find(now, m + 1);
			else now = _reverse_dll_find(now, m + 1);
		}
		printf("%d\n", _dll_getv(now));
		
		_dll_delete(now);
	}
	
	return;
}


int main()
{
	int n,m,ord;
	
	scanf("%d%d%d", &n, &m, &ord);
	
	solve(n,m,ord);
	
	return 0;
}
