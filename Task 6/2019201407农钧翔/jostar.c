#include <stdio.h>
#include "everything.h"

void work(int n, int m, int type) {
    init_link(n, type);
    node *now = head;
    int cnt = 1;
    while(size != 1) {
        if(cnt == m) {
            cnt = 1;
            now = delete_node(now);
        }
        else {
            cnt++;
            now = now -> next;
        }
    }
    printf("%d\n", head -> val);
}