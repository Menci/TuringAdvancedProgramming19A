#include <stdlib.h>

struct node {
	int index;
	struct node *pre, *next;
} *head;

typedef struct node person;

person *delete_node(person *operant);

void init(int total_candidate, int order);
