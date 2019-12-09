#include"struct.h"
#include<stdio.h>
#include<stdlib.h>

void link_node(LN *pre, LN *nxt)
{
    pre->nxt = nxt;
    nxt->pre = pre;
}

void del_node(LN *now)
{
    link_node(now->pre, now->nxt);
    free(now);
}

LN* goto_node_wise(LN *now, int gap)
{
    while(gap--)
        now = now->nxt;
    return now;
}

LN* goto_node_anti(LN *now, int gap)
{
    while(gap--)
        now = now->pre;
    return now;
}

void print_node(LN *now)
{
    printf("%d\n", now->data);
}