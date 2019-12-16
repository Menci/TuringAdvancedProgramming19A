#ifndef EVERYTHING_INCLUDED
#define EVERYTHING_INCLUDED

#define _SUCC 0
#define _ERR_SYNTAX 1
#define _ERR_OPEN_FAILED 2
#define _ERR_MALLOC_FAILED 4
#define _ERR_DATASIZE 8

#define FILE_NAME_MAX 32
#define NODE_MAX 1048576
#define mod 998244353

#define upper_div(x, y) (((x) + (y) - 1) / (y))

static const int big_num[4] = {19260817, 1000000007, 100000003, 10000009};

int zip_tree(int data_size, char *from_file_name, char *to_file_name, int *hash_val);
int extract_tree(char *file_name, int *hash_val);
int hash(char *str, int len);

#endif