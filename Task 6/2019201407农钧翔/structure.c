#include "everything.h"
#include <stdio.h>

void init_link(int n, int type) {
    size = n;
    head = malloc(sizeof(node));
    head -> val = 1;
    node *tmp, *tail = head;
    for(int i = 2; i <= n; i++) {
        tmp = malloc(sizeof(node));
        if(type == 0) tmp -> val = i;
        else tmp -> val = n + 2 - i;
        tail -> next = tmp;
        tmp -> prev = tail;
        tail = tmp;
    }
    tail -> next = head;
    head -> prev = tail;
}

node *delete_node(node *now) {
    if(size == 1) return NULL;
    size--;
    if(head == now) head = now -> next;
    now -> next -> prev = now -> prev;
    now -> prev -> next = now -> next;
    node *res = now -> next;
    printf("%d ", now -> val);
    free(now);
    return res;
}