#ifndef HULFFMAN
#define HULFFMAN

typedef struct code_table
{
	int w;
	short ls, rs;
	uchar val;
}code_table;

void dfs_hulffman(int *, int *, int, code_table *, int);
int dfs_tree(node *, int, int *, uchar *, int);
void hulffman_encode(node *, int, size_t, FILE *);

#endif
