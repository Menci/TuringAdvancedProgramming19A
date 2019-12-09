#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "function.h"
#include <malloc.h>
#define inf 1e9 + 7

int main () {
/*
#ifndef ONLINE_JUDGE
	freopen ("bona.in", "r", stdin);
	freopen ("bona.out", "w", stdout);
#endif
*/
	int n;
	scanf ("%d", &n);
	Head = NULL;

	while (n --) {
		int op, x;
		scanf ("%d%d", &op, &x);
		if (op == 0) Insert (x, Head, NULL);
		else if (op == 1) Delete (x, Head);
		else if (op == 2) printf ("%d\n", Kth_number (Head, x) );
		else if (op == 3) printf ("%d\n", Lessthanx (Head, x) );
		else if (op == 4) printf ("%d\n", Find_pre(Head, x, -1) );
		else if (op == 5) {
			int ans = Find_next (Head, x, inf);
			if (ans == inf) ans = -1;
			printf ("%d\n", ans);
		}
	}
	Free (Head);

	return 0;
}
