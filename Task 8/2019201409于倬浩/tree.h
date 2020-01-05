#ifndef _TREE_H_
#define _TREE_H_
#include <stdint.h>
#include <stdlib.h>
typedef struct tree_node_t {
    int left_child, right_child;
    char data[K];
} node;
void checkDFS(int,int);
void checkStructure(node *, node *);
#endif