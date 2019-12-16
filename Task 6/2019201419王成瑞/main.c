#include <stdio.h>

#include "list.h"
#include "parseargs.h"

int main(int argc, char **argv) {
    int size = -1, step = -1, dir = 0;
    parseArgs(argc, argv, &size, &step, &dir);
    list_t *list = newList(size);
    while(list -> size > 1) {
        moveForward(list, step, dir);
        printf("%d ", list -> cur -> id);
        deleteNode(list);
        list -> cur = dir ? list -> cur -> nxt : list -> cur -> pre;
    }
    printf("%d\n", list -> cur -> id);
    deleteList(list);
    return 0;
}
