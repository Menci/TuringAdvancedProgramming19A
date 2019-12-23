#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_BYTE 4

int get_mem(int K)
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

typedef struct node
{
	int lc, rc;
	char *data;
}node;

node *init(int n, int K, FILE* fileHandle)
{
	node* tree = (node *) malloc(n * sizeof (node));

	for (int i = 0; i < n; ++i) {
		fread(&tree[i].lc, sizeof (int), 1, fileHandle);
		fread(&tree[i].rc, sizeof (int), 1, fileHandle);
		tree[i].data = (char *) malloc((K + 1) * sizeof (char));
		fread(tree[i].data, sizeof (char), K, fileHandle);
	}

	return tree;
}

static int dfn_clock;
static int *p;

void compress(node *tree, int cur, int *data)
{
	p[dfn_clock/2] = cur;
	data[dfn_clock++] = tree[cur].lc >= 0;
	data[dfn_clock++] = tree[cur].rc >= 0;
	if (tree[cur].lc >= 0) {
		compress(tree, tree[cur].lc, data);
	}
	if (tree[cur].rc >= 0) {
		compress(tree, tree[cur].rc, data);
	}
}

void write_to_file(node *tree, int *data, int n, int K)
{
	FILE *fileHandle = fopen("data_zip.bin", "wb");

	fwrite(&n, sizeof (int), 1, fileHandle);
	fwrite(&K, sizeof (int), 1, fileHandle);

	unsigned int *data_zip = (unsigned int *) malloc(n * sizeof (unsigned int));
	int cur = -1;
	for (int i = 0; i < n; ++i) {
		if (i % 16 == 0) cur ++;
		int bit = (i & 15) * 2;
		data_zip[cur] |= data[i << 1] << bit;
		data_zip[cur] |= data[i << 1 | 1] << (bit + 1);
	}
	fwrite(data_zip, sizeof (unsigned int), cur + 1, fileHandle);

	for (int i = 0; i < n; ++i) {
		fwrite(tree[p[i]].data, sizeof (char), K, fileHandle);
	}

	fclose(fileHandle);

	free(data);
	free(data_zip);
}

int main(int argc, char **argv)
{
	int fileSize = 0;
	int n = 0;
//	int K = atoi(argv[1]);
	int K = 64;
	int blockSize = get_mem(K);

	FILE* fileHandle;

	fileHandle = fopen("3.in", "rb");
	if (fileHandle) {
		fileSize = size_of_(fileHandle);
		n = fileSize / blockSize;
		assert(fileSize % blockSize == 0);

		node *tree = init(n, K, fileHandle);
		fclose(fileHandle);

		int *data = (int *) malloc(2 * n * sizeof (int));
		p = (int *) malloc(n * sizeof (int));
		compress(tree, 0, data);

		write_to_file(tree, data, n, K);

		for (int i = 0; i < n; ++i) free(tree[i].data);
		free(tree);
		free(p);
	}
	else {
		printf("ERROR : can't open file\n");
	}
	return 0;
}

