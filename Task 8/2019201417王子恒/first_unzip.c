#include <stdio.h>

#define M 100000000 / K

typedef struct node {
	int l, r;
	unsigned char data[K];
} node;

node tr[M];

int fsize(FILE *fp) {
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return size;
}

void unzip(FILE *fp, int n) {
	int i, j;
	fread(tr, sizeof(node), n, fp);
	printf("%d %d\n", n, K);
	for (i = 0; i < n; ++i) {
		printf("%d %d", tr[i].l, tr[i].r);
		for (j = 0; j < K; ++j)
			printf(" %d", tr[i].data[j]);
		putchar('\n');
	}
	return;
}
int main() {
	freopen("first_unzip.out", "w", stdout);
	FILE *zip = fopen("zipped.bin", "rb");
	int zip_size = fsize(zip);
	int n = zip_size / sizeof(node);
	unzip(zip, n);
	return 0;
}
