#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <stdbool.h>
#define maxn 300010
#define Maxn 10000010
#define K 4
#define LONG (((K - 1) / 4 + 1) * 4 - K)
#define CHAR 8
#define INT 32

char s[maxn];

int dot;

typedef struct NODE {
	int left_child, right_child;
	char data[K];
}node;

node Tree[maxn];

typedef struct OJBK {
	char DATA[K];
}ojbk;

ojbk Save[maxn];

int dfs_clock, Gross;

char dfs_array[Maxn];

void Dfs (int h) {
	if (h == -1) return ;
	dfs_array[++ dfs_clock] = '0';
	++ Gross;
	for (int i = 0; i < K; ++i) {
		Save[Gross].DATA[i] = Tree[h].data[i];
	}
	
	Dfs (Tree[h].left_child);
	Dfs (Tree[h].right_child);

	dfs_array[++ dfs_clock] = '1';
}

void Done() {
	FILE * fp;
	fp = fopen ("Thunder.bin", "wb");
	fwrite (&dfs_clock, sizeof (int), 1, fp);
	fwrite (dfs_array, sizeof (char), dfs_clock, fp);
	fwrite (&Gross, sizeof(int), 1, fp);

	for (int i = 1; i <= Gross; ++i) {
		fwrite (Save[i].DATA, sizeof(char), K, fp);
	}

	fclose (fp);
}

int main () {
	freopen ("bona.out", "w", stdout);
	FILE * fp;
	int cnt = 0;
	fp = fopen ("1.in", "rb");
	while (1) {
		cnt = fread (&Tree[dot].left_child, sizeof (int), 1, fp);
		if (cnt != 1) break;
		cnt = fread (&Tree[dot].right_child, sizeof (int), 1, fp);
		printf ("%d\n", Tree[dot].right_child);
		cnt = fread (Tree[dot].data, sizeof (char), K, fp);
	//	cnt = fread (s, sizeof (char), LONG, fp);
		++ dot;
	}

	printf ("%d\n", dot);
	fclose (fp);

	Dfs(0);
	Done();
	return 0;
}
