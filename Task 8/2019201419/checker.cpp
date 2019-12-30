#include <bits/stdc++.h>

#include "tree.h"

using namespace std;

void dfs(int u, int* ord, int* dfn, Node* tree) {
    static int cnt = 0;
    ord[u] = cnt, dfn[cnt] = u, cnt++;
    for(int i = 0; i < K; i++) {
        printf("%02X ", (unsigned char)tree[u].data[i]);
    }
    putchar('\n');
    if(tree[u].left_child != EMPTY_SON) {
        dfs(tree[u].left_child, ord, dfn, tree);
    }
    if(tree[u].right_child != EMPTY_SON) {
        dfs(tree[u].right_child, ord, dfn, tree);
    }
}

int main() {
    ios::sync_with_stdio(false);
    FILE *in = fopen("input.bin", "rb");
    freopen("oringin.out", "w", stdout);
    
    fseek(in, 0, SEEK_END);
    int N = ftell(in) / sizeof(Node);
    fseek(in, 0, SEEK_SET);

    Node *tree = new Node[N];
    fread(tree, sizeof(Node), N, in);

    int *ord = new int[N + 1], *dfn = new int[N];
    ++ord;
    ord[-1] = -1;
    printf("%d\n", N);
    dfs(0, ord, dfn, tree);

    for(int i = 0; i < N; i++) {
        int u = dfn[i];
        printf("%d %d\n", ord[tree[u].left_child], ord[tree[u].right_child]);
    }
    return 0;
}
