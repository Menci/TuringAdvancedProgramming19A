#include<stdio.h>
#include"struct.h"
#include"main.h"

int main()
{
	openfile();
	int n;
	scanf("%d", &n);

	Treap *my_treap = Treap_build(comp, sizeof(int));
	for (int i = 1; i <= n;i++)
		solve(my_treap);

	Treap_free(my_treap);
	check_memory();
	return 0;
}

static void solve(Treap* my_treap)
{
	int opt, x;
	int *p;
	scanf("%d%d", &opt, &x);
	++opt;
	switch(opt)
	{
		case 1:
			Treap_insert(my_treap, &x);
			break;
		case 2:
			Treap_delete(my_treap, &x);
			break;
		case 3:
			p = Treap_find_data(my_treap, x);
			printf("%d\n", p == NULL ? -1 : *p);
			break;
		case 4:
			printf("%d\n", Treap_find_rnk(my_treap, &x));
			break;
		case 5:
			p = Treap_find_pre(my_treap, &x);
			printf("%d\n", p == NULL ? -1 : *p);
			break;
		case 6:
			p = Treap_find_nxt(my_treap, &x);
			printf("%d\n", p == NULL ? -1 : *p);
			break;
	}
}

int comp(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}