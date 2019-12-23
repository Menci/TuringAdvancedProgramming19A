#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

void dfs(Node *tree, int u, uint8_t* shape, FILE* out) {
    static int cnt = 0;
    fwrite(tree[u].data, sizeof(char), K, out);
    shape[cnt++] = ((tree[u].left_child != EMPTY_SON) << 1) | (tree[u].right_child != EMPTY_SON);
    if(tree[u].left_child != EMPTY_SON)
        dfs(tree, tree[u].left_child, shape, out);
    if(tree[u].right_child != EMPTY_SON)
        dfs(tree, tree[u].right_child, shape, out);
}

int main() {
    FILE *in = fopen("input.bin", "rb");
    FILE *out = fopen("zipped.bin", "wb");
    
    fseek(in, 0, SEEK_END);
    int N = ftell(in) / sizeof(Node);
    fseek(in, 0, SEEK_SET);
    fwrite(&N, sizeof(int), 1, out);

    Node *tree = (Node*)malloc(sizeof(Node) * N);
    fread(tree, sizeof(Node), N, in);
    
    uint8_t *shape = (uint8_t*)malloc(sizeof(uint8_t) * N);
    dfs(tree, 0, shape, out);

    for(int i = 0, j = 4; i < N; i = j, j += 4) {
        uint8_t tmp = 0;
        for(int k = i; k < j; k++)
            tmp = (tmp << 2) | (k < N ? shape[k] : 0);
        fwrite(&tmp, sizeof(uint8_t), 1, out);
    }

    free(tree);
    free(shape);
    return 0;
}
