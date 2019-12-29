#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define NUM_OF_CHILDREN 2
#define NUM_OF_BITS 8

static FILE *input;
static int ROOT;
static int *array;
static int total_node;

static node decode_tree[maxn];

int ceiling(int a, int b) {
	return (a + b - 1) / b;
}

void decode(int current) {

	static int cnt = 0;

	decode_tree[current].left = decode_tree[current].right = -1;
	
	char data[K + 10] = {0};
	fread(data, K * sizeof(char), 1, input);

	for (int i = 0; i < K; i++)
		decode_tree[current].data[i] = data[i];

	int left_bit = *array & (1 << cnt);
	int right_bit = *array & (1 << (cnt + 1));

	cnt += 2;
	if (cnt == NUM_OF_BITS * sizeof(int)) {
		cnt = 0;
		array ++;
	}

	if (left_bit) {
		++total_node;
		decode_tree[current].left = total_node;
		decode(total_node);
	}

	if (right_bit) {
		++total_node;
		decode_tree[current].right = total_node;
		decode(total_node);
	}
}

void dfs(int current) {

	int left = decode_tree[current].left;
	int right = decode_tree[current].right;

	if (left != -1) dfs(left);
	if (right != -1) dfs(right);
}

int check(int current, int decode_current) {
	if (current == -1 && decode_current == -1)
		return 1;
	if (current == -1 || decode_current == -1) 
		return 0;

	for (int i = 0; i < K; i++)
		if (tree[current].data[i] != decode_tree[decode_current].data[i]) {
			printf("Match Failed at #id = %d : %d %d\n", 
					current, tree[current].data[i], decode_tree[decode_current].data[i]);
			return 0;
		}

	return check(tree[current].left, decode_tree[decode_current].left) &
		check(tree[current].right, decode_tree[decode_current].right);
}

int main() {

	freopen("a.out", "w", stdout);

	input = fopen("zipped1.bin", "rb");

	int n;
	fread(&n, sizeof(int), 1, input);
	fread(&ROOT, sizeof(int), 1, input);

	int cnt = ceiling(n * NUM_OF_CHILDREN, NUM_OF_BITS * sizeof(int));

	array = malloc(cnt * sizeof(int));
	int *memo = array;

	fread(array, sizeof(int), cnt, input);

	decode(ROOT);

	dfs(ROOT);

	FILE *output_ans = fopen("data.ans", "r");
	for (int i = 0; i < n; i++) {
		fscanf(output_ans, "%d%d", &tree[i].left, &tree[i].right);
		for (int j = 0; j < K; j++) fscanf(output_ans, "%d", &tree[i].data[j]);
	}

	if (check(ROOT, ROOT)) {
		printf("Accepted!\n");
	} else 
		printf("Wrong Answer!");

	fclose(output_ans);

	free(memo);
	return 0;
}
