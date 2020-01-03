#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxn 80000000
#define inf 10000
#define total 300
#define maxlen 100

enum KID {LEFT, RIGHT};

static int len, sum, n, tail, tmp[total], num[maxn];
static char c[maxn], h[total][maxlen], l[maxlen], e = ' ';

struct Tree{
    int siz, key;
    int son[2];
}t[total * 2];

int search() {
    int x = 0, y;
    for (int i = 1; i <= tail; ++i) {
        if (x == 0 || t[tmp[x]].siz > t[tmp[i]].siz) x = i;
    }
    y = tmp[x];
    for (int i = x; i < tail; ++i) tmp[i] = tmp[i + 1];
    tail--;
    return y;
}

int merge(int x, int y) {
    ++sum;
    t[sum].siz = t[x].siz + t[y].siz;
    t[sum].son[LEFT] = y;
    t[sum].son[RIGHT] = x;
    return sum;
}

void dfs(int ro, int step) {
    // fprintf (stderr, "dfs: %d %d %d %d\n", ro, t[ro].son[LEFT], t[ro].son[RIGHT], t[ro].key);
    if (t[ro].son[LEFT] == -1 && t[ro].son[RIGHT] == -1) {
        for (int i = 0; i < step; ++i)
            h[t[ro].key][i] = l[i];
        h[t[ro].key][step] = '\0';
        return;
    }
    l[step] = '0'; dfs(t[ro].son[ LEFT], step + 1);
    l[step] = '1'; dfs(t[ro].son[RIGHT], step + 1);
}

int main () {
    freopen ("data.in", "r", stdin);
    FILE *fp = fopen("hfm.out", "wb");

    int x;
    n = sum = 257;

    for (int i = 0; i <= n ; ++i) {
        t[i].siz = 0; t[i].key = i;
        t[i].son[0] = t[i].son[1] = -1;
    }
    for (int x; scanf ("%d", &x) != EOF;) {
        t[x].siz++;
        num[++len] = x;
        // fprintf (stderr, "%d %d\n", i, x);
    }
    for (int i = 0; i <= n; ++i)
        if (t[i].siz) tmp[++tail] = i;
    
    for (int i = 1, p = tail; i < p; ++i) {
        int t1, t2;
        t1 = search(); t2 = search();
        tmp[++tail] = merge(t1, t2);
    }

    dfs(tmp[tail], 0);
    tail = 0;
    for (int i = 0, x = 0; i <= n; ++i) {
        fwrite (h[i], sizeof(char), strlen(h[i]), fp);
        fwrite (&e, sizeof(char), 1, fp);
    }
    
    for (int i = 1; i <= len; ++i) 
        for (int j = 0; j < strlen(h[num[i]]); ++j)
            c[tail++] = h[num[i]][j];
    fwrite (&tail, sizeof(int), 1, fp);
    int sum = 0;
    for (int i = 0; i < tail; i += 8) {
        char x = 0;
        if (i + 8 <= tail) {
            for (int j = i; j < i + 8; ++j)
                x = (x << 1) + c[j] - '0';
        }
        else {
            for (int j = i; j < tail; ++j)
                x = (x << 1) + c[j] - '0';
            x = x << (8 - tail + i);
        }
        fwrite(&x, sizeof(char), 1, fp);
        sum++;
    }
    return 0;
}