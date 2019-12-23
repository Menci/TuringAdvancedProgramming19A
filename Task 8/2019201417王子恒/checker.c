#include <stdio.h>

#define M 100000000 / K

typedef struct node {
	int l, r;
	int data[K];
} node;

node tr[M], tr_check[M];

int fsize(FILE *fp) {
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return size;
}
void calc_compression_ratio() {
	FILE *zip1 = fopen("zipped.bin", "rb");
	int zip1_size = fsize(zip1);
	FILE *zip2 = fopen("my_zip.bin", "rb");
	int zip2_size = fsize(zip2);
	printf("Compression Ratio : %f%%\n", (double)zip2_size * 100 / zip1_size);
	return;
}
int dfs(int x, int y, int k) {
	int i;
	if (x == -1 && y == -1) return 1;
	if (x == -1 || y == -1) return 0;
	for (i = 0; i < k; ++i) {
		if (tr[x].data[i] != tr_check[y].data[i]) return 0;
	}
	return dfs(tr[x].l, tr_check[y].l, k) & dfs(tr[x].r, tr_check[y].r, k);
}
int check() {
	FILE *zip1 = fopen("first_unzip.out", "r");
	FILE *zip2 = fopen("second_unzip.out", "r");
	int n, k, n_check, k_check;
	fscanf(zip1, "%d%d", &n, &k);
	fscanf(zip2, "%d%d", &n_check, &k_check);
	if (n != n_check || k != k_check) return 0;
	int i, j;
	for (i = 0; i < n; ++i) {
		fscanf(zip1, "%d%d", &tr[i].l, &tr[i].r);
		fscanf(zip2, "%d%d", &tr_check[i].l, &tr_check[i].r);
		for (j = 0; j < k; ++j) {
			fscanf(zip1, "%d", &tr[i].data[j]);
			fscanf(zip2, "%d", &tr_check[i].data[j]);
		}
	}
	return dfs(0, 0, k);
}
int main() {
	if (check()) {
		puts("Accept!");
		calc_compression_ratio();
	}
	else puts("Wrong Answer!");
	return 0;
}
