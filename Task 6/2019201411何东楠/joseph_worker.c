#include <stdio.h>
#include <stdlib.h>
#include "Dlinked_list.h"
#include "joseph_worker.h"

node* init(int n)
{
	node *cur = NULL, *head;
	for (int i = 1; i <= n; ++i) {
		cur = _insert(cur, i);
		if (i == 1) head = cur;
	}
	cur -> next = head;
	head -> pre = cur;

	return head;
}

node* work(int m, int type, node *head)
{
	while (1) {
		for (int step = 1; step <= m; ++step) {
			if (type) {
				head = head -> next;
			}
			else {
				head = head -> pre;
			}
		}
		printf("%d ", head -> ID);
		head = _delete(head, type);
		if (head == NULL) break;
	}

	printf("\n");
}

