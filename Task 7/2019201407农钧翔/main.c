#include <stdio.h>
#include "everything.h"

int main() {
    int n, op, x;
    splay_tree *tree = malloc(sizeof(splay_tree));
    init_tree(tree);
    scanf("%d", &n);
    for(int i = 1; i <= n; i++) {
        scanf("%d%d", &op, &x);
        switch (op) {
        case 0:
            Insert(x, tree);
            break;
        case 1:
            Delete(x, tree);
            break;
        case 2:
            printf("%d\n", search_by_rank(tree -> root, x) -> val);
            break;
        case 3:
            printf("%d\n", get_rank(tree -> root, x));
            break;
        case 4:
            printf("%d\n", get_prev(x, tree) -> val);
            break;
        case 5:
            printf("%d\n", get_next(x, tree) -> val);
            break;
        }
    }
    return 0;
}