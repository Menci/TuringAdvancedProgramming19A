#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxlen 100
#define maxn 30000

FILE *fp;

static int n, l, root, cnt, nxt[maxn][2], val[maxn], len;
static char c;

void init() {
    root = cnt = 1;
    memset(nxt,  0, sizeof(nxt));
    memset(val, -1, sizeof(val));
}

void insert(int key) {
    int now = root, x;
    char ch;
    while (1) {
        fread(&ch, sizeof(char), 1, fp);
        if (ch == ' ') break;
        x = ch - '0';
        if (nxt[now][x] == 0)
            nxt[now][x] = ++cnt;
        now = nxt[now][x];
    }
    val[now] = key;
}

int r;

int main () {
    fp = fopen("hfm.out", "rb");
    freopen ("me.out", "w", stdout);

    int n = 257, sum = 0;
    init();
    for (int i = 0; i <= n; ++i) insert(i);
    fread(&len, sizeof(int), 1, fp);
    r = root;
    for (int i = 0; i < len;) {
        sum++;
        char x;
        fread(&x, sizeof(char), 1, fp);
        for (int j = 7; j >= 0 && i < len; --j, ++i) {
            if (x & (1 << j)) r = nxt[r][1];
            else r = nxt[r][0];
            if (r == 0)return 0;
            if (val[r] >= 0) {
                printf ("%d ", val[r]);
                r = root;
            }
        }
    }
    return 0;
}