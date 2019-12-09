#ifndef _STRUCT_H
#define _STRUCT_H

#include<stdlib.h>

struct List_node{
    int data;
    struct List_node *pre, *nxt;
};
typedef struct List_node LN;

LN* new_node(void);

#endif


