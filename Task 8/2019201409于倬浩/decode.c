#include "decode.h"
#include "tree.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
static int nelem;
static uint8_t* stack;
static node* tr;
int decodeDFS() {
    int curidx = nelem;
    for(int i = 0; i < K; ++i) tr[curidx].data[i] = (char)stack[i];
    uint8_t childstatus = stack[K];
    ++nelem, stack += K + 1;
    if(childstatus & 1) tr[curidx].left_child = decodeDFS();
    else tr[curidx].left_child = -1;
    if(childstatus & 2) tr[curidx].right_child = decodeDFS();
    else tr[curidx].right_child = -1;
    return curidx;
}
node* decodeArrayToStruct(void *s, size_t n) { 
    tr = malloc(n * sizeof(node));
    stack = (uint8_t *)s;
    node *begin = tr;
    decodeDFS();
    return begin;
}