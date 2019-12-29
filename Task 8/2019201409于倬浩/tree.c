#include "tree.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
static node *root1, *root2;
void checkDFS(int a, int b) {
    for(int i = 0; i < K; ++i) assert(root1[a].data[i] == root2[b].data[i]);
    if(root1[a].left_child != -1) {
        assert(root2[b].left_child != -1);
        checkDFS(root1[a].left_child, root2[b].left_child);
    }
    else assert(root2[b].left_child == -1);
    if(root1[a].right_child != -1) {
        assert(root2[b].right_child != -1);
        checkDFS(root1[a].right_child, root2[b].right_child);
    }
    else assert(root2[b].right_child == -1);
}
void checkStructure(node *rt1, node *rt2) {
    root1 = rt1;
    root2 = rt2;
    checkDFS(0, 0);
    return;
}