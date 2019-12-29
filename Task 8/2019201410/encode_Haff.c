#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

#define NUM_OF_BITS 8
#define NUM_OF_CHILDREN 2
#define MAX_CHAR 300
#define MAXN 1000000
int ROOT;

static FILE *input;
static FILE *output_compress, *output_ans;

char code[MAX_CHAR][MAX_CHAR];

typedef struct Haffnode {
	int weight;
	char data;
	struct Haffnode *left;
	struct Haffnode *right;
} Haffnode;

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

static int count[MAX_CHAR];

void sort(Haffnode *node, int length) {
	for(int i = 0; i < length; i++) {
		int Max_weight = node[i].weight;
		int Max_node = i;
		for(int j = i + 1; j < length; j ++) {
			if (node[j].weight > Max_weight) {
				Max_weight = node[j].weight;
				Max_node = j;
			}
		}

		if (Max_node != i) {
			Haffnode t;
			t = node[Max_node];
			node[Max_node] = node[i];
			node[i] = t;
		}
	}
}

Haffnode left[MAXN], right[MAXN];

void build_Haff(Haffnode *node, int total_node) {
	if (total_node == 1)
		return ;
	
	sort(node, total_node);

	Haffnode parent;

	left[total_node] = node[total_node - 2];
	right[total_node] = node[total_node - 1];

	parent.left = &left[total_node];
	parent.right = &right[total_node];
	parent.weight = (left[total_node].weight) + (right[total_node].weight);
	node[ total_node - 2 ] = parent;

	build_Haff(node, total_node - 1);
}

void coding(Haffnode *node, char *current_code, int length) {
	if (node == NULL)
		return;
	if (node -> left == NULL && node -> right == NULL) {

		current_code[length] = '\0';
		strcpy(code[ (node->data) + 128 ], current_code);

		return;
	}

	current_code[length] = '0';
	if (node -> left != NULL)
		coding(node->left, current_code, length + 1);

	current_code[length] = '1';
	if (node -> right != NULL)
		coding(node->right, current_code, length + 1);
}

void encode(int current) {

	static int mask = 0;
	static int cnt = 0;

	if (current == -1)
		return;

	for (int i = 0 ; i < K; i++) {
		int current_char = tree[current].data[i] + 128;
		for(int j = 0; j < strlen(code[current_char]); j++) {
			if ( code[current_char][j] == '1' ) mask |= (1 << cnt);
			cnt++;
			if (cnt == NUM_OF_BITS) {
				fwrite(&mask, sizeof(char), 1, output_compress);
				cnt = 0;
				mask = 0;
			}
		}
	}

	encode(tree[current].left);
	encode(tree[current].right);

	if (current == ROOT && cnt > 0) {
		fwrite(&mask, sizeof(char), 1, output_compress);
		cnt = 0;
		mask = 0;
	}
}

static int *array_mask;
static int *array_data;

Haffnode *root;

void decode(int current) {

	static int dfs_clock = 0;

	static int cnt_mask = 0;
	static int cnt_data = 0;

	int left_child = *array_mask & (1 << cnt_mask);
	int right_child = *array_mask & (1 << (cnt_mask + 1));

	char data[MAX_CHAR];
	Haffnode *current_node = root;
	while(1) {
		if (current_node -> left == NULL && current_node -> right == NULL) {
			break;
		}
		if ( *array_data & (1 << cnt_data) )
			current_node = current_node -> left;
		else 
			current_node = current_node -> right;

		cnt_data++;

		if (cnt_data == NUM_OF_BITS * sizeof(int)) {
			array_data ++;
			cnt_data = 0;
		}
	}

	cnt_mask += 2;
	if (cnt_mask == NUM_OF_BITS * sizeof(int)) {
		array_mask ++;
		cnt_mask = 0;
	}

	if (left_child) {
		++dfs_clock;
		decode(dfs_clock);
	}

	if (right_child) {
		++dfs_clock;
		decode(dfs_clock);
	}
}

int ceiling(int x, int y) {
	return (x + y - 1) / y;
}

void Decode(int n) {
	FILE *fp;
	fp = fopen("zipped.bin", "rb");
	int cnt_mask = ceiling(n * 2, NUM_OF_BITS * sizeof(int));
	int cnt_data =  n;
	array_mask = malloc(cnt_mask * sizeof(int));
	array_data = malloc(cnt_data * sizeof(int));
	decode(ROOT);
}


int main() {

	FILE *fp;
	int length;

	//fp = fopen("input.bin", "rb");
	fp = fopen("8.in", "rb");

	if (fp == NULL)  perror("Error opening file");
	else {
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	int total_nodes = length / (NUM_OF_CHILDREN * sizeof(int) + K);

	output_compress = fopen("zipped8.bin", "wb");
	output_ans = fopen("data.ans", "w");
	freopen("t.out","w",stdout);

	int n;
	n = total_nodes;
	for(int i = 0; i < n; i++) {
		int left, right;
		fread(&left, sizeof(int), 1, fp);
		fread(&right, sizeof(int), 1, fp);
		tree[i].left = left;
		tree[i].right = right;
		fread(tree[i].data, K * sizeof(char), 1, fp);
		for (int j = 0; j < K; j ++) {
			count[ tree[i].data[j] + 128] ++;
		}
	}

	Haffnode node[MAX_CHAR];
	int cnt_node = 0;


	for(int i = 0; i < 256; i++) {
		if (count[i]) {
			node[cnt_node].data = i - 128;
			node[cnt_node].weight = count[i];
			node[cnt_node].left = NULL;
			node[cnt_node].right = NULL;
			cnt_node ++;
		}
	}

	char char_code[MAX_CHAR];

	build_Haff(node, cnt_node);
	coding(&node[0], char_code, 0);

	root = &node[0];

	recurse(ROOT);
	encode(ROOT);

	return 0;

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
