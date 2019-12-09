#include <stdio.h>
#include "josephus.h"

int index_node(int total_candidate, int order, int number) {
	if(order) {
		return number;
	}
	return total_candidate + 2 - number;
}

void init(int total_candidate, int order) {
	head = malloc(sizeof(person));
	head -> index = 1;

	person *lst = head, *current;

	for(int i = 1; i < total_candidate; i++) {
		current = (person*)malloc(sizeof(person));
		current -> index = index_node(total_candidate, order, i + 1);

		lst -> next = current;
		current -> pre = lst;
		
		lst = current;
	}

	lst -> next = head;
	head -> pre = lst;
}

person *delete_node(person *operant) {

	if(operant -> next == operant) {
		free(operant);
		return NULL;
	}

	operant -> pre -> next = operant -> next;
	operant -> next -> pre = operant -> pre;
	person *ret = operant -> next;
	free(operant);
	return ret;
}

