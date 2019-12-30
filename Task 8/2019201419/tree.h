#ifndef _TREE_H_

#define _TREE_H_

#ifndef K
#   define K 10
#endif

#define EMPTY_SON   (-1)

typedef struct {
    int left_child, right_child;
    char data[K];
} Node;

#endif