#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tree.h"
static node *s;
int rnd(int l, int r) {
    return rand() % (r - l + 1) + l;
}
static int tot;
void buildtree(int x, int size) {
    if(size <= 1) {
        s[x].left_child = s[x].right_child = -1;
        return;
    }
    int sl = rnd(0, size - 1), sr = size - 1 - sl;
    if(sl) {
        ++tot;
        s[x].left_child = tot;
        buildtree(tot, sl);
    }
    else s[x].left_child = -1;
    if(sr) {
        ++tot;
        s[x].right_child = tot;
        buildtree(tot, sr);
    }
    else s[x].right_child = -1;
}
int main() {
    srand(233333333);
    // printf("%d %d\n", K, sizeof(node));
    FILE *fp = fopen("input.bin", "w");
    int n = 10;
    s = (node *)malloc(sizeof(node) * n);
    memset(s, 0, sizeof(node) * n);
    for(int i = 0; i < n; ++i) 
        for(int j = 0; j < K; ++j)
            s[i].data[j] = 0xaa;
    buildtree(0, n);
    // for(int i = 0; i < n; ++i) printf("%d %d %d\n", i, s[i].left_child, s[i].right_child);
    fwrite((void *)s, sizeof(node), n, fp);
    free((void *)s);
    fclose(fp);
}