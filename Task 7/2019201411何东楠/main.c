#include <stdio.h>
#include "splay.h"

int main()
{
#ifndef ONLINE_JUDGE
	freopen("input.in", "r", stdin);
	freopen("output.out", "w", stdout);
#endif

	int n; scanf("%d", &n);
	for (int i = 1; i <= n; ++i) {
		int type, x;
		node *ret;

		scanf("%d%d", &type, &x);

		switch(type) {
			case 0:
				insert(x);
				break;
			case 1:
				erase(x);
				break;
			case 2:
				printf("%d\n", find_kth(get_root(), x));
				break;
			case 3:
				printf("%d\n", get_rank(x));
				break;
			case 4:
			case 5:
				ret = neighbor_ele(x, type & 1); 
				printf("%d\n", ret == NULL ? -1 : ret -> val);
				break;
		}

	}

	cut_tree(get_root());

	return 0;
}

