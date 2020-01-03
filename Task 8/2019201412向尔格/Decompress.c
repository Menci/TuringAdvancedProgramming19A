#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define maxn 10000010
#define Maxn 1010
#define Bias 128
#define INT 8

char s[maxn], dfs_array[maxn], Leaf[Maxn];

// dfs_array dfs树的括号序列

int Len, tot, ch[Maxn][2], Cnt[Maxn], dfs_clock, number[Maxn], sum_leaf, Len_Huffman, Len_Total;

// Len s数组的长度
// Cnt[i] = i 出现的次数
// ch 记录孩子
// dfs_array dfs_clock 树的括号表示
// number[i] 表示哈夫曼树中节点i对应的char值
// Leaf[i]表示从左到右第i个叶子节点是谁
// Len_Huffman Huffman数组的长度
// Len_total 存储Huffman映射需要的长度

typedef struct NODE {
	int id, Val;
	struct NODE * Next;
} node;

node * Head = NULL;

node * newnode () { //malloc 新建节点
	node * tmp = (node *) malloc (sizeof (node));
	return tmp;
}

void Insert (int nowid, int nowval) { //链表实现插入排序
	if (Head == NULL) {
		Head = newnode ();
		Head -> id = nowid;
		Head -> Val = nowval;
		Head -> Next = NULL;
		return ;
	}

	node * tmp = Head;
	if (nowval <= tmp -> Val) {
		tmp = newnode ();
		tmp -> id = nowid;
		tmp -> Val = nowval;
		tmp -> Next = Head;
		Head = tmp;
		return;
	}

	while (1) {
		if (tmp -> Next == NULL) {
			node * TMP = newnode ();
			TMP -> id = nowid;
			TMP -> Val = nowval;
			tmp -> Next = TMP;
			TMP -> Next = NULL;
			break;
		}

		if (tmp -> Next -> Val >= nowval) {
			node * TMP = newnode ();
			TMP -> id = nowid;
			TMP -> Val = nowval;

			TMP -> Next = tmp -> Next;
			tmp -> Next = TMP;
			break;
		}
		tmp = tmp -> Next;
	}
}

void Init() {
	for (int i = 0; i < Len; ++i) {
		++ Cnt[ s[i] + Bias];
	}

	for (int i = 0; i < (1 << INT); ++i) {
		if (Cnt[i]) {
			Insert (++ tot, Cnt[i]);
			number[tot] = i;
		}
	}
}

void Min_found () {
	node * tmp_first = Head;
	node * tmp_second = Head -> Next;
	Head = Head -> Next -> Next;
	
	++ tot;
	ch[tot][0] = tmp_first -> id;
	ch[tot][1] = tmp_second -> id;

	int nowval = tmp_first -> Val + tmp_second -> Val;
	free (tmp_first), free (tmp_second);
	Insert (tot, nowval);
}

void Make_Tree () {
	while (Head -> Next != NULL) {
		Min_found ();
	}
	free (Head);
}

bool is_leaf (int h) {
	if (!ch[h][0] && !ch[h][1]) return true;
	return false;
}

void Dfs (int h) {
	if (!h) return ;
	if (is_leaf(h)) {
		Leaf [sum_leaf ++] = (char) (number[h] - Bias);
	}

	dfs_array[dfs_clock ++] = '0';
	Dfs (ch[h][0]);
	Dfs (ch[h][1]);
	dfs_array[dfs_clock ++] = '1';
}

char Huffman[maxn * 4], Represent[(1 << 8) + 5][10000], Long[Maxn];
// Represent[i][j] 第i个数对应的字符串
// Long[i]代表第i个数对应的字符串的长度
void dfs_huffman (int h, int length) {
	if (!h) return ;
	if (is_leaf (h)) {
		Long[number[h]] = length;
		for (int i = 0; i < length; ++i) {
			Represent[number[h]][i] = Huffman[i];
		}
	}

	Huffman[length] = '0';
	dfs_huffman (ch[h][0], length + 1);
	Huffman[length] = '1';
	dfs_huffman (ch[h][1], length + 1);
}

void Create_Huffmantree () {
	Init ();
	Make_Tree ();
	Dfs (tot);
	dfs_huffman (tot, 0);
}

char Pre_work[maxn];

void Huffmancode() {
	for (int i = 0; i < Len; ++i) {
		for (int j = 0; j < Long[s[i] + Bias]; ++j) {
			Huffman[Len_Huffman ++] = Represent[s[i] + Bias][j];
		}
	}

	int tmp = Len_Huffman;
	if (Len_Huffman % INT != 0) {
		for (int i = 1; i <= (Len_Huffman % INT); ++i) {
			Huffman[Len_Huffman ++] = '0';
		}
	}

	int gross = 0;
	for (int i = 0; i < Len_Huffman; i += INT) {
		int value = 0;
		for (int j = i; j <= i + INT - 1; ++j) {
			if (Huffman[j] == '1') value += (1 << (j - i) );
		}
		Pre_work[gross ++] = (char) (value - Bias);
	}

	FILE * fp;
	fp = fopen ("result.bin", "wb");

	printf ("%d\n", Len_Huffman);
	printf ("%d\n", dfs_clock);
	printf ("%d\n", sum_leaf);

	fwrite (&dfs_clock, sizeof (int), 1, fp);
	fwrite (&sum_leaf, sizeof (int), 1, fp);
	fwrite (dfs_array, sizeof (char), dfs_clock, fp);
	fwrite (Leaf, sizeof (char), sum_leaf, fp);
	fwrite (&tmp, sizeof (int), 1, fp);
	fwrite (Pre_work, sizeof (char), gross, fp);
	
	fclose (fp);
}

int main() {

	freopen ("bona.out" ,"w", stdout);

	FILE * fp;
	fp = fopen ("Thunder.bin", "rb");
	Len = fread (s, sizeof (char), maxn - 10, fp);
	fclose (fp);

	Create_Huffmantree ();
	Huffmancode ();

	return 0;
}
