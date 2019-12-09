#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "link.h"

#define N 1000

enum KIND{RIGHT = 0, LEFT = 1};

static int ans[N];
struct Link *mov(struct Link *p, int k) {
	if (k == RIGHT) return p->next;
	else if (k == LEFT) return p->last;
}
void init(int n, struct Link *Head) {
	struct Link *Now = NULL;
	Now = clear(Head);
	
	for (int i = 1; i <= n; ++i) {
		add(i, Now);
		Now = Now->next;
	}
	Now->next = Head->next;
	(Head->next)->last = Now;
}
void solve(int n, int m, int k, struct Link *Head) {
	struct Link *Now = Head->next;

	for (int step = 1; step <= n; ++step) {
		for (int i = 1; i <= m; ++i)
			Now = mov(Now, k);
		ans[step] = Now->id;
		if (k == RIGHT) {
			Now = Now->next;
			del(Now->last, Head);
		}
		else if (k == LEFT) {
			Now = Now->last;
			del(Now->next, Head);
		}
	}
	for (int i = 1; i <= n; ++i) printf("%d ", ans[i]);
	printf("\n");
}
int main (int argc, char *argv[]) {
	int n, m, k;
	struct Link *Head = malloc(sizeof(struct Link));
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	k = atoi(argv[3]);
	init(n, Head);
	solve(n, m, k, Head);
	free(Head);
	return 0;
}
