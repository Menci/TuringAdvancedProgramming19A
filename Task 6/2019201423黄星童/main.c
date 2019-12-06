#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

enum {
    COUNTERCLOCKWISE = -1, CLOCKWISE = 1
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        puts("Input Format Error");
        puts("Please read \"README.md\".");
        return 0;
    }

    int n, k, type;
    n = atoi(argv[1]);
    k = atoi(argv[2]);
    type = atoi(argv[3]);
    if (n <= 0 || k <= 0 || (type != -1 && type != 1)) {
        puts("Input Format Error");
        puts("Please read \"README.md\".");
        return 0;
    }

    linked_list *list = CreateList();
    for (int i = 1; i <= n; i++)
        InsertNode(list, list -> tail, i);

    linked_list_node *now = list -> head;
    while (n--) {
        for (int i = 0; i < k; i++) {
            if (type == CLOCKWISE)
                now = now -> next;
            else
                now = now -> prev;
        }
        printf("%d ", now -> data);

        linked_list_node *tmp = now;
        if (type == CLOCKWISE)
            now = now -> next;
        else
            now = now -> prev;
        DeleteNode(list, tmp);
    }
    puts("");
    DeleteList(list);
    return 0;
}