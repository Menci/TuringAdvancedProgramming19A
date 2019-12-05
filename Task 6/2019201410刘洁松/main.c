#include <stdio.h>
#include "josephus.h"

int main(int argc, char **argv) {

	int total_candidate, interval, order;

	if(argc != 4) {
		printf("invalid input!\n");
		return 0;
	}

	total_candidate = atoi(argv[1]);
	interval = atoi(argv[2]);
	order = atoi(argv[3]);

	init(total_candidate, order);

	person *current = head;
	while(total_candidate --) {
		for(int i = 0; i < interval; i++) {
			current = current -> next;
		}
		printf("%d ", current -> index);
		current = delete_node(current);
	}
	puts("");
	return 0;
}
