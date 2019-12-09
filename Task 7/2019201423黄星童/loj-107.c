#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define size(node) ((node) == NULL ? 0 : (node) -> size)

typedef struct node {
    int size;
    int data;
    int key;
    struct node *lch, *rch;
}node;

typedef struct treap {
    node *root;
}treap;

typedef struct {
    node *first, *second;
}pair;

int Random() {
    static int seed = 19260817;
    return seed = 1ll * seed * 998244353ll % 1000000007ll;
}

node *NewNode(int x) {
    node *newNode = (node *)malloc(sizeof(node));
    newNode -> size = 1, newNode -> data = x;
    newNode -> key = Random();
    newNode -> lch = newNode -> rch = NULL;
    return newNode;
}

treap *CreateTreap() {
    treap *newTreap = (treap *)malloc(sizeof(treap));
    newTreap -> root = NULL;
    return newTreap;
}

void DFS(node *x) {
    if (x == NULL) return;

    DFS(x -> lch);
    DFS(x -> rch);

    free(x);
}

void DeleteTreap(treap *tree) {
    DFS(tree -> root);
    free(tree);
}

inline void PushUp(node *x) {
    x -> size = size(x -> lch) + size(x -> rch) + 1;
}

node *Merge(node *x, node *y) {
    if (x == NULL) return y;
    if (y == NULL) return x;

    if (x -> key < y -> key) {
        x -> rch = Merge(x -> rch, y);
        PushUp(x);
        return x;
    }
    else {
        y -> lch = Merge(x, y -> lch);
        PushUp(y);
        return y;
    }
}

pair Split(node *x, int k) {
    if (x == NULL) return (pair){NULL, NULL};

    if (k <= size(x -> lch)) {
        pair tmp = Split(x -> lch, k);
        x -> lch = tmp.second;
        PushUp(x);
        return (pair){tmp.first, x};
    }
    else {
        pair tmp = Split(x -> rch, k - 1 - size(x -> lch));
        x -> rch = tmp.first;
        PushUp(x);
        return (pair){x, tmp.second};
    }
}

int GetRank(node *x, int data) {
    if (x == NULL) return 0;

    if (data > x -> data)
        return size(x -> lch) + 1 + GetRank(x -> rch, data);
    else
        return GetRank(x -> lch, data);
}

void InsertNode(treap *tree, int x) {
    int k = GetRank(tree -> root, x);
    pair tmp = Split(tree -> root, k);
    tree -> root = Merge(Merge(tmp.first, NewNode(x)), tmp.second);
}

void DeleteNode(treap *tree, int x) {
    int k = GetRank(tree -> root, x);
    pair tmp1 = Split(tree -> root, k);
    pair tmp2 = Split(tmp1.second, 1);
    tree -> root = Merge(tmp1.first, tmp2.second);
    free(tmp2.first);
}

int GetKth(node *x, int k) {
    if (k <= size(x -> lch))
        return GetKth(x -> lch, k);
    if (k == size(x -> lch) + 1)
        return x -> data;
    return GetKth(x -> rch, k - size(x -> lch) - 1);
}

inline int Predecessor(node *x, int data) {
    int ans = -1;
    while (x != NULL) {
        if (x -> data < data) {
            ans = max(ans, x -> data);
            x = x -> rch;
        }
        else
            x = x -> lch;
    }
    return ans;
}

inline int Successor(node *x, int data) {
    int ans = INT_MAX;
    while (x != NULL) {
        if (x -> data > data) {
            ans = min(ans, x -> data);
            x = x -> lch;
        }
        else
            x = x -> rch;
    }
    return ans == INT_MAX ? -1 : ans;
}

int main() {
    int n;
    scanf("%d", &n);
    treap *tree = CreateTreap();
    while (n--) {
        int cmd, x;
        scanf("%d%d", &cmd, &x);
        switch (cmd) {
            case 0:
                InsertNode(tree, x);
                break;
            case 1:
                DeleteNode(tree, x);
                break;
            case 2:
                printf("%d\n", GetKth(tree -> root, x));
                break;
            case 3:
                printf("%d\n", GetRank(tree -> root, x));
                break;
            case 4:
                printf("%d\n", Predecessor(tree -> root, x));
                break;
            case 5:
                printf("%d\n", Successor(tree -> root, x));
                break;
        }
    }
    DeleteTreap(tree);
    return 0;
}