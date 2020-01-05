#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define NUM_OF_BITS 8
#define NUM_OF_CHILDREN 2
int ROOT;

static FILE *input;
static FILE *output_compress, *output_ans;

void recurse(int current) {

	static int mask = 0;
	static int cnt = 0;

	if (current == -1) return;

	if (tree[current].left != -1) {
		mask |= 1 << cnt;
	}

	if (tree[current].right != -1) {
		mask |= 1 << (cnt + 1);
	}

	cnt += 2;
	if (cnt == sizeof(int) * NUM_OF_BITS) {
		fwrite(&mask, sizeof(int), 1, output_compress);
		cnt = 0;
		mask = 0;
	}

	recurse(tree[current].left);
	recurse(tree[current].right);

	if (current == ROOT && cnt > 0) {
		fwrite(&mask, sizeof(int), 1, output_compress);
	}
}

void encode(int current) {
	if (current == -1)
		return;

	char data[K];
	for (int i = 0 ; i < K; i++)
		data[i] = tree[current].data[i];

	fwrite(data, K * sizeof(char), 1, output_compress);

	encode(tree[current].left);
	encode(tree[current].right);
}

int main() {

	FILE *fp;
	int length;

	//fp = fopen("input.bin", "rb");
	fp = fopen("1.in", "rb");

	if (fp == NULL)  perror("Error opening file");
	else {
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	int total_nodes = length / (NUM_OF_CHILDREN * sizeof(int) + K);

	output_compress = fopen("zipped1.bin", "wb");
	output_ans = fopen("data.ans", "w");

	int n;
	n = total_nodes;
	fwrite(&n, sizeof(int), 1, output_compress);
	for(int i = 0; i < n; i++) {

		int left, right;
		fread(&left, sizeof(int), 1, fp);
		fread(&right, sizeof(int), 1, fp);

		tree[i].left = left;
		tree[i].right = right;

		fread(tree[i].data, K * sizeof(char), 1, fp);

	}

	fwrite(&ROOT, sizeof(int), 1, output_compress);

	recurse(ROOT);
	encode(ROOT);

	fclose(input);
	fclose(output_compress);

	for (int i = 0; i < n; i++) {
		fprintf(output_ans, "%d %d\n", tree[i].left, tree[i].right);
		for (int j = 0; j < K; j++)
			fprintf(output_ans, "%d ", tree[i].data[j]);
		fprintf(output_ans, "\n");
	}
	fclose(output_ans);

	return 0;
}
