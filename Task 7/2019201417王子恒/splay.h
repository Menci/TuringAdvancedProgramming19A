#ifndef _INC_SPLAY
#define _INC_SPLAY

#include <stdlib.h>

void ins_node(int);
void del_node(int);
int find_kth(int);
int smaller_than_x(int);
int max_num_smaller_than_x(int);
int min_num_bigger_than_x(int);
void del_balanced_tree();

#endif
