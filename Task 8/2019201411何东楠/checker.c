#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_BYTE 4
#define K 8

int get_mem()
{
	return 8 + (K + MAX_BYTE - 1) / MAX_BYTE * MAX_BYTE;
}

int size_of_(FILE* fileHandle)
{
	int file_curpos = ftell(fileHandle);

	int fileSize = 0;
	fseek(fileHandle, 0, SEEK_END);
	fileSize = ftell(fileHandle);

	fseek(fileHandle, file_curpos, SEEK_SET);

	return fileSize;
}

typedef struct tree_node
{
	int lc, rc;
	char data[K];
}node;

void dfs(int cur, node *tree)
{
	printf("(");
	if (tree[cur].lc >= 0) dfs(tree[cur].lc, tree);
	if (tree[cur].rc >= 0) dfs(tree[cur].rc, tree);
	printf(")");
}

void dfsL(int cur, node *tree)
{
	for (int i = 0; i < K; ++i) {
		printf("%d\n", (unsigned char) tree[cur].data[i]);
	}
	if (tree[cur].lc >= 0) dfsL(tree[cur].lc, tree);
	if (tree[cur].rc >= 0) dfsL(tree[cur].rc, tree);
}

int main(int argc, char **argv)
{
	freopen("std_extract.out", "w", stdout);
	int fileSize = 0;
	int n = 0;
//	int K = atoi(argv[1]);
	int blockSize = get_mem(K);

	FILE* fileHandle;

	fileHandle = fopen("data.in", "rb");
	if (fileHandle) {
		fileSize = size_of_(fileHandle);
		n = fileSize / blockSize;
		assert(fileSize % blockSize == 0);
		node *tree = (node *) malloc((n + 1) * sizeof (node));
		fread(tree, sizeof (node), n, fileHandle);

		dfs(0, tree);
		printf("\n");
		dfsL(0, tree);
	}
	else {
		printf("ERROR : can't open file\n");
	}
	return 0;
}

