#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define N 10000010
#define M 100000000 / K

typedef struct node {
	int l, r;
	uint8_t data[K];
} node;

node tr[M];
int fsize(FILE *fp) {
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return size;
}

void unzip(FILE *fp, int n) {
	fread(tr, sizeof(node), n, fp);
	return;
}

int cnt_dir, cnt_data, h_cnt_num, h_cnt_dir, h_cnt_data, num[256], code[256];
uint8_t dir[N], data[N], h_dir[N], h_data[N];
void dfs(int x) {
	int i;
	for (i = 0; i < K; ++i) {
		data[++cnt_data] = tr[x].data[i];
		++num[data[cnt_data]];
	}
	if (tr[x].l != -1) {
		dir[++cnt_dir] = 1;
		dir[++cnt_dir] = 0;
		dfs(tr[x].l);
	}
	if (tr[x].r != -1) {
		dir[++cnt_dir] = 1;
		dir[++cnt_dir] = 1;
		dfs(tr[x].r);
	}
	dir[++cnt_dir] = 0;
	return;
}

typedef struct huffman_node {
	int l, r, f, w, id;
	uint8_t data;
} h_node;
int cmp(const void *a, const void *b) { return (((h_node *)a) -> w - ((h_node *)b) -> w); }
h_node h_tr[N];
h_node q1[N], q2[N];
int l1, r1, l2, r2;
h_node h_init(uint8_t data, int w) {
	h_node x;
	x.l = x.r = x.f = -1;
	x.w = w;
	x.id = ++h_cnt_num;
	x.data = data;
	return x;
}
h_node get_min() {
	if (l1 > r1) return q2[l2++];
	if (l2 > r2) return q1[l1++];
	if (q1[l1].w < q2[l2].w) return q1[l1++];
	return q2[l2++];
}
int huffman() {
	int i;
	for (i = 0; i < 256; ++i) {
		if (num[i]) {
			h_node x = h_init(i, num[i]);
			q1[++r1] = x;
			code[i] = x.id;
		}
	}
	int root = q1[l1].id;
	h_tr[root] = q1[l1];
	qsort(q1, r1 + 1, sizeof(h_node), cmp);
	while (r1 - l1 + 1 + r2 - l2 + 1 > 1) {
		h_node x1 = get_min();
		h_node x2 = get_min();
		h_node x3 = h_init(0, x1.w + x2.w);
		x1.f = x2.f = x3.id;
		x3.l = x1.id;
		x3.r = x2.id;
		h_tr[x1.id] = x1;
		h_tr[x2.id] = x2;
		h_tr[x3.id] = x3;
		root = x3.id;
		q2[++r2] = x3;
	}
	return root;
}
void init() {
	cnt_dir = cnt_data = h_cnt_num = h_cnt_dir = h_cnt_data = 0;
	l1 = l2 = 0;
	r1 = r2 = -1;
	memset(num, 0, sizeof(num));
}
void h_dfs(int x) {
	if (h_tr[x].l == -1 && h_tr[x].r == -1) {
		h_data[++h_cnt_data] = h_tr[x].data;
		return;
	}
	h_dir[++h_cnt_dir] = 0;
	h_dfs(h_tr[x].l);
	h_dir[++h_cnt_dir] = 1;
	h_dfs(h_tr[x].r);
	return;
}
int nodq[N], qcnt;
uint8_t zip[N];
void write(int root) {
	FILE *my_zip = fopen("my_zip.bin", "wb");
	int i, j, cnt;
	int n = (cnt_dir >> 3) + (cnt_dir % 8 != 0);
	for (i = 0; i < n; ++i) {
		zip[i] = 0;
		for (j = (i << 3) + 1; j <= (i + 1) << 3; ++j) {
			zip[i] <<= 1;
			zip[i] |= dir[j];
		}
	}
	fwrite(zip, sizeof(uint8_t), n, my_zip);
	n = (h_cnt_dir >> 3) + (h_cnt_dir % 8 != 0);
	for (i = 0; i < n; ++i) {
		zip[i] = 0;
		for (j = (i << 3) + 1; j <= (i + 1) << 3; ++j) {
			zip[i] <<= 1;
			zip[i] |= h_dir[j];
		}
	}
	fwrite(zip, sizeof(uint8_t), n, my_zip);
	fwrite(h_data + 1, sizeof(uint8_t), h_cnt_data, my_zip);
	n = 0;
	for (i = 1, cnt = 0; i <= cnt_data; ++i) {
		int x = code[data[i]];
		if (x == root) {
			if (cnt == 8) {
				cnt = 0;
				++n;
			}
			++cnt;
			continue;
		}
		qcnt = 0;
		while (x != -1) {
			nodq[++qcnt] = x;
			x = h_tr[x].f;
		}
		while (qcnt > 1) {
			if (cnt == 8) {
				cnt = 0;
				++n;
			}
			++cnt;
			zip[n] <<= 1;
			zip[n] |= (h_tr[nodq[qcnt]].r == nodq[qcnt - 1]);
			--qcnt;
		}
	}
	zip[n++] <<= 8 - cnt;
	fwrite(zip, sizeof(uint8_t), n, my_zip);
	return;
}
void zipper() {
	init();
	dfs(0);
	int root = huffman();
	h_dfs(root);
	h_dir[++h_cnt_dir] = 1;
	write(root);
	return;
}
int main() {
	FILE *zip = fopen("zipped.bin", "rb");
	int zip_size = fsize(zip);
	int n = zip_size / sizeof(node);
	unzip(zip, n);
	zipper();
	return 0;
}
