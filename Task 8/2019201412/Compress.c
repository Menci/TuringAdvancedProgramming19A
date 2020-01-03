#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define maxn 10000010
#define Maxn 1010
#define Bias 128
#define INT 8

int dfs_clock, sum_leaf, Len_Huffman, Len, tot, ch[Maxn][2], Stack[Maxn], Cnt_stack, TMP, Cnt_leaf, Leaf_val[Maxn], Cnt_result;

char dfs_array[maxn], Leaf[Maxn], Pre_work[maxn], Huffman[maxn * 4], result[maxn];

void Create_tree() {
	for (int i = 0; i < dfs_clock; ++i) {
		if (dfs_array[i] == '0') {
			Stack[++ Cnt_stack] = ++ tot;
		}
		else {
			-- Cnt_stack;
			if (Cnt_stack) {
				if (!ch[Stack[Cnt_stack]][0]) {
					ch[Stack[Cnt_stack]][0] = Stack[Cnt_stack+1];
				}
				else ch[Stack[Cnt_stack]][1] = Stack[Cnt_stack+1];
			}
		}
	}
}

bool is_leaf(int h) {
	if (!ch[h][0] && !ch[h][1]) return true;
	return false;
}

void dfs (int h, int Length) {
	if (!h) return;
	if (is_leaf(h)) {
		Leaf_val[h] = (Leaf[Cnt_leaf ++] + Bias);
	}
	dfs (ch[h][0], Length + 1);
	dfs (ch[h][1], Length + 1);
}

void get_result () {
	for (int i = 0; i < TMP;) {
		int root = 1;
		for (int j = i; ; ++j) {
			if (Huffman[j] == '0') root = ch[root][0];
			else root = ch[root][1];
			if (is_leaf(root)) {
				result[Cnt_result ++] = (char)(Leaf_val[root] - Bias);
				printf ("%d\n", Leaf_val[root] - Bias);
				i = j + 1;
				break;
			}
		}
	}
	FILE * fp;
	fp = fopen ("Final.bin", "wb");
	fwrite (result, sizeof (char), Cnt_result, fp);
}

int main () {
	
	freopen ("Bona.out", "w", stdout);
	
	FILE * fp;
	fp = fopen ("result.bin", "rb");
	int tmp = fread (&dfs_clock, sizeof (int), 1, fp);
	tmp = fread (&sum_leaf, sizeof (int), 1, fp);
	tmp = fread (dfs_array, sizeof (char), dfs_clock, fp);
	tmp = fread (Leaf, sizeof (char), sum_leaf, fp);
	tmp = fread (&TMP, sizeof (int), 1, fp);
	Len = fread (Pre_work, sizeof(char), maxn * 4 - 10, fp);
	fclose (fp);

	for (int i = 0; i < Len; ++i) {
		for (int j = 0; j < INT; ++j) {
			if ( (Pre_work[i] + Bias) & (1 << j)) {
				Huffman[Len_Huffman ++] = '1';
			}
			else Huffman[Len_Huffman ++] = '0';
		}
	}

	Create_tree();
	dfs (1, 1);
	get_result();

	return 0;
}
