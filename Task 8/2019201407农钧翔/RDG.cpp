#include <bits/stdc++.h>

using namespace std;

const int K = 21;
const int N = 1e6 + 5;

struct node {
    int lson, rson;
    char val[K];
} pool[N];

map <int, int> M;

int deg[N];
int son[N][2], p[N], id[N];

inline int _rand(int l, int r) {
    return rand() % (r - l + 1) + l;
}

int main() {
    srand(time(0));
    int n;
    cin >> n;
    for(int i = 2; i <= n; i++) {
        int x;
        while(deg[x = _rand(1, i - 1)] - 2 == 0) ;
        son[x][deg[x]++] = i;
        p[i] = x;
        id[i] = i;
    }
    id[1] = 1;
    random_shuffle(id + 1, id + n + 1);
    for(int j = 1; j <= n; j++) {
        int i = id[j];
        pool[i].lson = son[i][0] - 1;
        pool[i].rson = son[i][1] - 1;
        for(int k = 0; k < K; k++) pool[i].val[k] = rand();
    }
    FILE *fp = fopen("input.bin", "wb");
    fwrite(pool + 1, sizeof(node), n, fp);
    for(int i = 1; i <= min(n, 8); i++) {
        printf("%d -> %d | %d -> %d\n", i - 1, pool[i].lson, i - 1, pool[i].rson);
    }
    return 0;
}