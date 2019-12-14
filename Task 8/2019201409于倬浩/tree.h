#ifndef _TREE_H_
#define _TREE_H_
#define K 64
typedef struct tree_node_t {
    int left_child, right_child;
    char data[K];
} node;
int* getAncestor();
int checkStructure(node *, node *, size_t);
#endif