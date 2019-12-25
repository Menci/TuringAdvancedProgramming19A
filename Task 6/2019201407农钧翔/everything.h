#include <stdlib.h>

typedef struct _node {
    int val;
    struct _node *next, *prev;
} node;

node *head;
int size;

void init_link(int n, int type);

node *delete_node(node *now);
