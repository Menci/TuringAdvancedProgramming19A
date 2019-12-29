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
	unsigned char *data;
}node;

node *init(int n, int K, FILE* fileHandle)
{
	node* tree = (node *) malloc(n * sizeof (node));//

	for (int i = 0; i < n; ++i) {
		fread(&tree[i].lc, sizeof (int), 1, fileHandle);
		fread(&tree[i].rc, sizeof (int), 1, fileHandle);
		tree[i].data = (unsigned char *) malloc((K + 1) * sizeof (unsigned char));
		fread(tree[i].data, sizeof (unsigned char), K, fileHandle);
	}

	return tree;
}

static int *p;

void compress(node *tree, int cur, int *data)
{
	static int dfn_clock;
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

#define SIGMA 256

typedef struct huffmanTree
{
	int id;
	struct huffmanTree *fa, *lc, *rc;
}htnode;

typedef struct Dlinklist
{
	int key;
	htnode *p;
	struct Dlinklist *pre, *nxt;
}DLL;

DLL* insert(DLL *pre, int key, int id)
{
	DLL *cur = (DLL *) malloc(sizeof (DLL));

	cur -> key = 0;
	cur -> pre = cur -> nxt = NULL;

	cur -> p = (htnode *) malloc(sizeof (htnode));
	cur -> p -> lc = NULL;
	cur -> p -> rc = NULL;
	cur -> p -> id = id;

	cur -> key = key;
	cur -> pre = pre;
	if (pre) pre -> nxt = cur;

	return cur;
}

static char str[SIGMA];
static char code[SIGMA][SIGMA];

void compress_dfs(htnode *cur, int *data, int dep, short *ord)
{
	static int dfn_clock;
	static int leafOrder;

	if (!cur -> lc && !cur -> rc) {
		for (int i = 0; i < dep; ++i) code[cur -> id][i] = str[i];
//		printf("%d : %s\n", cur -> id, code[cur -> id]);
		ord[leafOrder ++] = cur -> id;
	}
	data[dfn_clock++] = cur -> lc != NULL;
	data[dfn_clock++] = cur -> rc != NULL;
	if (cur -> lc) {
		str[dep] = '0';
		compress_dfs(cur -> lc, data, dep + 1, ord);
		str[dep] = 0;
	}
	if (cur -> rc) {
		str[dep] = '1';
		compress_dfs(cur -> rc, data, dep + 1, ord);
		str[dep] = 0;
	}
}

DLL* disconnect(DLL *cur, DLL *head)
{
	if (cur -> pre) {
		cur -> pre -> nxt = cur -> nxt;
	}
	if (cur -> nxt) {
		cur -> nxt -> pre = cur -> pre;
	}
	if (head == cur) return cur -> nxt;
	else return head;
	cur -> pre = cur -> nxt = NULL;
}

void tree_writer(int *data, int n, FILE* fileHandle)
{
	fwrite(&n, sizeof (int), 1, fileHandle);

	unsigned int *data_zip = (unsigned int *) malloc(n * sizeof (unsigned int));
	int cur = -1;
	for (int i = 0; i < n; ++i) {
		if (i % 16 == 0) cur ++;
		int bit = (i & 15) * 2;
		data_zip[cur] |= data[i << 1] << bit;
		data_zip[cur] |= data[i << 1 | 1] << (bit + 1);
	}
	fwrite(data_zip, sizeof (unsigned int), cur + 1, fileHandle);

	free(data_zip);
	free(data);

	return ;
}

void data_writer(node *tree, int n, int K, FILE* fileHandle)
{
	int *occurTimes = (int *) malloc((SIGMA + 1) * sizeof(int));//
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < K; ++j) occurTimes[tree[i].data[j]] ++;
	}

	DLL *head = insert(NULL, occurTimes[0], 0); 
	DLL *pre = head;
	for (int i = 1; i < SIGMA; ++i) {
		pre = insert(pre, occurTimes[i], i);
	}

	htnode *root = NULL;
	for (int i = 0; i < SIGMA - 1; ++i) {
		DLL *MIN = NULL, *SMIN = NULL;
		for (DLL *cur = head; cur; cur = cur -> nxt) {
			if (!MIN) {
				MIN = cur;
			}
			else if (MIN -> key > cur -> key) {
				SMIN = MIN;
				MIN = cur;
			}
			else if (!SMIN) {
				SMIN = cur;
			}
			else if (SMIN -> key > cur -> key) {
				SMIN = cur;
			}
		}

		head = disconnect(MIN, head);
		head = disconnect(SMIN, head);

		//new tree node : fa
		htnode *anc = (htnode *) malloc(sizeof (htnode));
		anc -> id = SIGMA;
		anc -> fa = NULL;

		anc -> lc = MIN -> p; 
		MIN -> p -> fa = anc;

		anc -> rc = SMIN -> p;
		SMIN -> p -> fa = anc;

		//new list node
		DLL *_new = (DLL *) malloc(sizeof (DLL));
		_new -> pre = NULL;
		_new -> nxt = head;
		if (head) head -> pre = _new;
		head = _new;

		_new -> key = (MIN -> key) + (SMIN -> key);
		_new -> p = anc;

		free(MIN);
		free(SMIN);

		if (i == SIGMA - 2) root = anc;
	}

	free(head);

	int *data = (int *) malloc((SIGMA << 3) * sizeof (int));//
	short *ord = (short *) malloc((SIGMA << 3) * sizeof (short));//

	compress_dfs(root, data, 0, ord);
	tree_writer(data, 511, fileHandle);
	fwrite(ord, sizeof (short), 256, fileHandle);

	unsigned char buffer = 0;
	int bit = 0;
	for (int o = 0; o < n; ++o) {
		int i = p[o];
		for (int j = 0; j < K; ++j) {
			int val = tree[i].data[j];
			if (o == 0) {
				assert(i == 0);
			}
			int len = strlen(code[val]);
			for (int k = 0; k < len; ++k) {
				bit ++;
				if ((int) bit > 8) {
					bit = 1;
					fwrite(&buffer, sizeof (unsigned char), 1, fileHandle);
					buffer = 0;
				}
//				if (o == 0) printf("%d\n", code[val][k] - '0');
				buffer |= (code[val][k] - '0') << (bit - 1);
			}
		}
	}
	if (bit) fwrite(&buffer, sizeof (unsigned char), 1, fileHandle);

	free(occurTimes);
}

#undef SIGMA

int main(int argc, char **argv)
{
//	freopen("1.out", "w", stdout);
	int fileSize = 0;
	int n = 0;
	int K = 8;
	int blockSize = get_mem(K);

	FILE* fileHandle;

	fileHandle = fopen("data.in", "rb");
	if (fileHandle) {
		fileSize = size_of_(fileHandle);
		n = fileSize / blockSize;
		assert(fileSize % blockSize == 0);

		node *tree = init(n, K, fileHandle);
		fclose(fileHandle);

		FILE *fileHandle = fopen("data_zip.bin", "wb");
		fwrite(&K, sizeof (int), 1, fileHandle);

		int *data = (int *) malloc(2 * n * sizeof (int));//
		p = (int *) malloc(n * sizeof (int));//
		compress(tree, 0, data);
		tree_writer(data, n, fileHandle);
		data_writer(tree, n, K, fileHandle);

		for (int i = 0; i < n; ++i) free(tree[i].data);
		free(tree);
		free(p);

		fclose(fileHandle);
	}
	else {
		printf("ERROR : can't open file\n");
	}
	return 0;
}

#undef MAX_BYTE

