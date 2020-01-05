#include <stdio.h>
#include <stdint.h>

#define N 10000010
#define M 100000000 / K

typedef struct node {
	int l, r, f;
	uint8_t data[K];
} node;

node tr[M];
uint8_t zip[N], unzip[N << 3], vis[N];
int cnt, tot, h_cnt, node_sum;

int fsize(FILE *fp) {
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return size;
}

void init() {
	cnt = tot = h_cnt = node_sum = 0;
}
void build(int x) {
	int i;
	tr[x].f = -1;
	while (x != -1) {
		if (!vis[x]) {
			vis[x] = 1;
			tr[x].l = tr[x].r = -1;
		}
		if (unzip[tot++] == 0) {
			x = tr[x].f;
			continue;
		}
		if (unzip[tot++] == 0) tr[x].l = ++cnt;
		else tr[x].r = ++cnt;
		tr[cnt].f = x;
		x = cnt;
	}
	for (i = 0; i <= cnt; ++i) vis[i] = 0;
	node_sum = cnt + 1;
	cnt = node_sum * K;
	while (tot % 8) ++tot;
	return;
}

typedef struct huffman_node {
	int l, r, f;
	uint8_t data;
} h_node;
h_node h_tr[N];
void h_build(int x) {
	h_tr[x].f = -1;
	while (x != -1) {
		if (!vis[x]) {
			vis[x] = 1;
			h_tr[x].l = h_tr[x].r = -1;
		}
		if (unzip[tot++] == 0) {
			h_tr[x].l = ++h_cnt;
			h_tr[h_cnt].f = x;
			x = h_cnt;
		}
		else {
			while (x != -1 && (h_tr[x].l == -1 || h_tr[x].r != -1)) {
				x = h_tr[x].f;
			}
			if (x == -1) break;
			h_tr[x].r = ++h_cnt;
			h_tr[h_cnt].f = x;
			x = h_cnt;
		}
	}
	while (tot % 8) ++tot;
}
void h_dfs(int x) {
	int i;
	if (h_tr[x].l == -1 && h_tr[x].r == -1) {
		uint8_t c = 0;
		for (i = 0; i < 8; ++i) {
			c <<= 1;
			c |= unzip[tot++];
		}
		h_tr[x].data = c;
		return;
	}
	h_dfs(h_tr[x].l);
	h_dfs(h_tr[x].r);
	return;
}
uint8_t find() {
	int x = 0;
	if (h_tr[x].l == -1 && h_tr[x].r == -1) {
		++tot;
		return h_tr[x].data;
	}
	while (h_tr[x].l != -1 && h_tr[x].r != -1) {
		if (unzip[tot++] == 0) x = h_tr[x].l;
		else x = h_tr[x].r;
	}
	return h_tr[x].data;
}
void unzip_data() {
	int x = 0, i = 0;
	while (cnt--) {
		tr[x].data[i] = find();
		++i;
		if (i >= K) {
			i -= K;
			++x;
		}
	}
}
void write() {
	int i, j;
	printf("%d %d\n", node_sum, K);
	for (i = 0; i < node_sum; ++i) {
		printf("%d %d", tr[i].l, tr[i].r);
		for (j = 0; j < K; ++j)
			printf(" %d", tr[i].data[j]);
		putchar('\n');
	}
}
void _unzip(FILE *fp, int n) {
	init();
	int i, j;
	fread(zip, sizeof(uint8_t), n, fp);
	for (i = 0; i < n; ++i) {
		for (j = (i + 1) * 8 - 1; j >= i * 8; --j) {
			unzip[j] = (zip[i] >> 1 << 1) ^ zip[i];
			zip[i] >>= 1;
		}
	}
	build(0);
	h_build(0);
	h_dfs(0);
	unzip_data();
	write();
	return;
}
int main() {
	freopen("second_unzip.out", "w", stdout);
	FILE *zip = fopen("my_zip.bin", "rb");
	int zip_size = fsize(zip);
	int n = zip_size / sizeof(uint8_t);
	_unzip(zip, n);
	return 0;
}
