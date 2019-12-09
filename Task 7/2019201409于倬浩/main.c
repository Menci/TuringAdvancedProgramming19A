#include <stdio.h>
#include "persistent_treap.h"
int main() {
	extern node *root, *null;
	initialize();
	int op, x, n;
	scanf("%d", &n);
	while (n--) {
		scanf("%d%d", &op, &x);
		if (op == 0)
			insert(x);
		else if (op == 1)
			erase(x);
		else if (op == 2)
			printf("%d\n", kthElement(x)->v);
		else if (op == 3)
			printf("%d\n", rankElement(x));
		else if (op == 4)
			printf("%d\n", precessor(x));
		else
			printf("%d\n", successor(x));
	}
	cleanup();
	return 0;
}