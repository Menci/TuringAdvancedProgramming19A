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

int main(int argc, char **argv)
{
	freopen("my_extract.out", "w", stdout);

	int n = 0;
	int K = 0;

	FILE* fileHandle;

	fileHandle = fopen("data_zip.bin", "rb");
	if (fileHandle) {
		fread(&K, sizeof (int), 1, fileHandle);
		fread(&n, sizeof (int), 1, fileHandle);

		int *treeStr = (int *) malloc(n * 2 * sizeof (int));
		unsigned int *treeZip = (unsigned int *) malloc(((n + (BLK_SZ - 1)) / BLK_SZ + 1) * sizeof (unsigned int));
		fread(treeZip, sizeof (unsigned int), (n + (BLK_SZ - 1)) / BLK_SZ, fileHandle);

		for (int i = 0; i < n; ++i) {
			treeStr[i << 1] = (treeZip[i / BLK_SZ] >> ((i & (BLK_SZ - 1)) * 2)) & 1;
			treeStr[i << 1 | 1] = (treeZip[i / BLK_SZ] >> ((i & (BLK_SZ - 1)) * 2 + 1)) & 1;
		}

		char *data = (char *) malloc(n * K * sizeof (char));
		fread(data, sizeof (char), n * K, fileHandle);

		fclose(fileHandle);

		dfs(0, treeStr); printf("\n");

		free(treeStr);
		free(data);
	}
	else {
		printf("ERROR : can't open file\n");
	}
	return 0;
}

#undef BLK_SZ
#undef MAX_BYTE

