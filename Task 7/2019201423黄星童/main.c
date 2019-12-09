#include <stdio.h>
#include "treap.h"

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