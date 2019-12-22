#ifndef BRACKET
#define BRACKET

#include "basic.h"

int get_root(node *, size_t);
int dfs_brckt(node *, int, uchar *, int);
void get_brckt_seq(node *, int, size_t, FILE *);
void get_weight(node *, int, FILE *);

#endif
