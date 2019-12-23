#include "tree.h"
#include <assert.h>

void LDR(int x, node *tree, FILE *output) {
    if (x == -1) return;

    LDR(tree[x].lch, tree, output);
    fwrite(tree[x].ch, K, 1, output);
    LDR(tree[x].rch, tree, output);
}

void DataDFS(int x, node *tree, FILE *output) {
    if (x == -1) return;

    fwrite(tree[x].ch, K, 1, output);
    DataDFS(tree[x].lch, tree, output);
    DataDFS(tree[x].rch, tree, output);
}

void ChDFS(int x, node *tree, FILE *output, char *info, int *now) {
    if (x == -1) return;

    if (tree[x].lch != -1)
        *info |= (1 << ((*now) - 1));
    if (tree[x].rch != -1)
        *info |= (1 << ((*now) - 2));
    *now = (*now) - 2;

    if (!(*now)) {
        fwrite(info, 1, 1, output);
        *info = 0;
        *now = 8;
    }

    ChDFS(tree[x].lch, tree, output, info, now);
    ChDFS(tree[x].rch, tree, output, info, now);
}

void BuildTree(int *tot, node *tree, char *info, FILE *input) {
    int now = *tot;
    fseek(input, sizeof(int) + now * K, SEEK_SET);
    fread(tree[now].ch, K, 1, input);

    tree[*tot].lch = tree[*tot].rch = -1;
    if (info[now] & 2) {
        *tot = (*tot) + 1;
        tree[now].lch = *tot;
        BuildTree(tot, tree, info, input);
    }
    if (info[now] & 1) {
        *tot = (*tot) + 1;
        tree[now].rch = *tot;
        BuildTree(tot, tree, info, input);
    }
}