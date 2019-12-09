#include <stdlib.h>
#include <stdint.h>

#include "list.h"

list_t* newList(int size) {
    list_t *res = (list_t*)malloc(sizeof(list_t));
    res -> size = size;
    res -> node = (listNode*)malloc(sizeof(listNode) * (size_t)size);
    res -> cur = res -> node;
    for(int i = 0; i < size; i++) {
        res -> node[i].id = i + 1;
        res -> node[i].pre = res -> node + ((i - 1 + size) % size);
        res -> node[i].nxt = res -> node + ((i + 1) % size);
    }
    return res;
}

void moveForward(list_t *list, int step, int dir) {
    for(int i = 0; i < step; i++) {
        list -> cur = dir ? list -> cur -> nxt : list -> cur -> pre;
    }
}

void deleteNode(list_t *list) {
    list -> size -= 1;
    list -> cur -> pre -> nxt = list -> cur -> nxt;
    list -> cur -> nxt -> pre = list -> cur -> pre;
}

void deleteList(list_t *list) {
    free(list -> node);
    free(list);
}