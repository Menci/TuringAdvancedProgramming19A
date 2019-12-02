#include <stdio.h>
#include "josephus.h"
#include "my_linked_list.h"
node *kickPlayer(node *pos, int k, int dir) {
	while (k) {
		--k;
		if (dir == 1)
			pos = pos->nxt;
		else
			pos = pos->pre;
	}
	return pos;
}
void runGame(int n, int m, int dir) {
	node *head, *last;
	initializeHead(&head);
	last = head;
	for (int i = 2; i <= n; ++i) {
		node *new = newNode(i);
		insertNode(last, new);
		last = new;
	}
	last = head;
	for (int i = 1; i <= n; ++i) {
		last = kickPlayer(last, m, dir);
		printf("%d ", last->val);
		if (dir == 1) {
			last = last->nxt;
			eraseNode(last->pre);
		} else {
			last = last->pre;
			eraseNode(last->nxt);
		}
	}
}