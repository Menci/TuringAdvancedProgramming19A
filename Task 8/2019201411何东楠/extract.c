
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_BYTE 4
#define BLK_SZ 16

int dfs(int cur, int *treeStr)
{
	printf("(");
	int dfnL = cur + 1;
	if (treeStr[cur + 0]) dfnL = dfs(dfnL + 1, treeStr);
	if (treeStr[cur + 1]) dfnL = dfs(dfnL + 1, treeStr);
	printf(")");
	return dfnL;
}

typedef struct node
{
	int id;
	struct node *lc, *rc;
}node;

int dfs_build(node *u, int cur, int *treeStr)
{
	static int leafOrder;
	int dfnL = cur + 1;
	if (treeStr[cur + 0]) {
		u -> lc = (node *) malloc(sizeof (node));
		u -> lc -> lc = NULL;
		u -> lc -> rc = NULL;
		u -> lc -> id = -1;
		dfnL = dfs_build(u -> lc, dfnL + 1, treeStr);
	}
	if (treeStr[cur + 1]) {
		u -> rc = (node *) malloc(sizeof (node));
		u -> rc -> lc = NULL;
		u -> rc -> rc = NULL;
		u -> rc -> id = -1;
		dfnL = dfs_build(u -> rc, dfnL + 1, treeStr);
	}
	else {
		u -> id = leafOrder++;
	}
	return dfnL;

}

int main(int argc, char **argv)
{
	freopen("my_extract.out", "w", stdout);

	int n = 0;
	int K = 0;
	short p[256 + 5];

	FILE* fileHandle;

	fileHandle = fopen("data_zip.bin", "rb");
	if (fileHandle) {
		fread(&K, sizeof (int), 1, fileHandle);
		fread(&n, sizeof (int), 1, fileHandle);

		int total_cnt = n * K;

		int *treeStr = (int *) malloc(n * 2 * sizeof (int));
		unsigned int *treeZip = (unsigned int *) malloc(((n + (BLK_SZ - 1)) / BLK_SZ + 1) * sizeof (unsigned int));
		fread(treeZip, sizeof (unsigned int), (n + (BLK_SZ - 1)) / BLK_SZ, fileHandle);

		for (int i = 0; i < n; ++i) {
			treeStr[i << 1] = (treeZip[i / BLK_SZ] >> ((i & (BLK_SZ - 1)) * 2)) & 1;
			treeStr[i << 1 | 1] = (treeZip[i / BLK_SZ] >> ((i & (BLK_SZ - 1)) * 2 + 1)) & 1;
		}

		dfs(0, treeStr); printf("\n");

		fread(&n, sizeof (int), 1, fileHandle);
		fread(treeZip, sizeof (unsigned int), (n + (BLK_SZ - 1)) / BLK_SZ, fileHandle);
		for (int i = 0; i < n; ++i) {
			treeStr[i << 1] = (treeZip[i / BLK_SZ] >> ((i & (BLK_SZ - 1)) * 2)) & 1;
			treeStr[i << 1 | 1] = (treeZip[i / BLK_SZ] >> ((i & (BLK_SZ - 1)) * 2 + 1)) & 1;
		}

		node *root = (node *) malloc(sizeof (node));
		dfs_build(root, 0, treeStr);

		fread(&p, sizeof (short), 256, fileHandle);

		node* cur = root;
		int bit = 0;
		unsigned char buffer;
		fread(&buffer, sizeof (unsigned char), 1, fileHandle);
		int cnt = 0;
		while (total_cnt) {
			bit ++;
			if (bit > 8) {
				bit = 1;
				fread(&buffer, sizeof (unsigned char), 1, fileHandle);
			}
			if (buffer & (1 << (bit - 1))) {
				cur = cur -> rc;
//				if (cnt <= 3) printf("1\n"); 
			}
			else {
				cur = cur -> lc;
//				if (cnt <= 3) printf("0\n"); 
			}

			if (cur -> id >= 0) {
				cnt ++;
				printf("%d\n", p[cur -> id]), cur = root, total_cnt --;
			}
		}

		fclose(fileHandle);

		free(treeStr);
	}
	else {
		printf("ERROR : can't open file\n");
	}
	return 0;
}

#undef BLK_SZ
#undef MAX_BYTE

