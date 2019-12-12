#include <stdio.h>
#include "RB_Tree.h"

int main () {
	freopen ("testdata.in", "r", stdin);
	freopen ("ans.out", "w", stdout);
	int n, k, x, i;
	struct RB_Tree *tmp;
	scanf ("%d", &n);
	for (i = 1; i <= n; ++i) {
		scanf ("%d%d", &k, &x);
		if (k == 0) {
			Insert(root, x);
		} else if (k == 1) {
			Delete(root, x);
		} else if (k == 2) {
			printf ("%d\n", Get_Id(root, x));
		} else if (k == 3) {
			printf ("%d\n", Get_Rank(root, x) - 1);
		} else if (k == 4) {
			tmp = Get_Last(root, x);
			if (tmp == NULL) puts("-1");
			else printf ("%d\n", tmp->val);
		} else if (k == 5) {
			tmp = Get_Next(root, x);
			if (tmp == NULL) puts("-1");
			else printf ("%d\n", tmp->val);
		}
	}
	Clean();
	return 0;
}
