#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

uint8_t getShape(uint8_t *data, int id) {
    int cur = id & 3;
    return (data[id >> 2] >> (2 * (3 - cur))) & 3;
}

int dfs(Node* tree, char *data, uint8_t *shape) {
    static int cnt = 0;
    int cur_id = cnt++;
    Node *cur = tree + cur_id;
    memcpy(cur -> data, data + cur_id * K, sizeof(char[K]));
    uint8_t sons = getShape(shape, cur_id);
    if(sons >> 1) {
        cur -> left_child = dfs(tree, data, shape);
    } else {
        cur -> left_child = EMPTY_SON;
    }
    if(sons & 1) {
        cur -> right_child = dfs(tree, data, shape);
    } else {
        cur -> right_child = EMPTY_SON;
    }
    return cur_id;
}

int main() {
    FILE *in = fopen("zipped.bin", "rb");
    FILE *out = fopen("unzipped.out", "w");
    
    int N;
    fread(&N, sizeof(int), 1, in);

    char *data = (char*)malloc(sizeof(char) * N * K); 
    fread(data, sizeof(char), N * K, in);

    int lth = (N + 3) / 4;
    uint8_t *shape = (uint8_t*)malloc(sizeof(uint8_t) * lth);
    fread(shape, sizeof(uint8_t), lth, in);

    Node *tree = (Node*)malloc(sizeof(Node) * N);
    dfs(tree, data, shape);

    fprintf(out, "%d\n", N);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < K; j++) {
            fprintf(out, "%02X ", (unsigned char)tree[i].data[j]);
        }
        fputc('\n', out);
    }
    for(int i = 0; i < N; i++) {
        fprintf(out, "%d %d\n", tree[i].left_child, tree[i].right_child);
    }

    free(data);
    free(shape);
    free(tree);
    return 0;
}
