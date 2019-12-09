#include <limits.h>
#include <stdio.h>

#include "splay.h"

int main() {
	int n;
	scanf("%d", &n);
	splayNode *root = newNode(-1);
	root -> siz = root -> cnt = 1;
	insert(&root, root, INT_MAX);
	while(n--) {
		int op, x, ans;
		scanf("%d%d", &op, &x);
		switch(op) {
			case 0:
				insert(&root, root, x);
				break;
			case 1:
				erase(&root, x);
				break;
			case 2:
				printf("%d\n", getKth(root, x + 1));
				break;
			case 3:
				printf("%d\n", getRank(root, x));
				break;
			default:
				ans = getNumber(&root, x, op - 4);
				if(ans == INT_MAX) ans = -1;
				printf("%d\n", ans);
		}
	}
	clear(root);
	return 0;
}
