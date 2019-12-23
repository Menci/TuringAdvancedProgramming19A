#include "encode.h"
#include "tree.h"
#include <stdint.h>
#include <stdlib.h>
static uint8_t* stack;
static node* tr;
static int remainSpace;
void encodeDFS(int x) {
    if(x == -1) return;
    for(int i = 0; i < K; ++i) stack[i] = tr[x].data[i];
    stack[K] = 0;
    if(tr[x].left_child != -1) stack[K] |= 1;
    if(tr[x].right_child != -1) stack[K] |= 2;
    if(remainSpace) stack += K + 1, remainSpace -= K + 1;
    if(tr[x].left_child != -1) encodeDFS(tr[x].left_child);
    if(tr[x].right_child != -1) encodeDFS(tr[x].right_child);
}
void* encodeStructToArray(node *s, size_t *size, size_t n) {
    stack = malloc(2 * (K + 1) * n);
    memset(stack, 0, (K + 1) * n);
    tr = s;
    void *begin = (void *)stack;
    *size = remainSpace = (K + 1) * n;
    encodeDFS(0);
    return begin;
}