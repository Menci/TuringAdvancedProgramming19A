#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define max(a, b) (a) > (b) ? (a) : (b)

#define k 8

enum KIND {LEFT = 0, RIGHT = 1};

FILE *fp;
int len;
typedef struct{
	int left_child, right_child;
	char data[k];
} node;
node *p, *q;
unsigned *t;
int step = 0, sum, n;
char c[30000000];
void print(int c, int dep) {
	if (c < 16) {
		if (dep == 1) {
			if (c < 26) putchar(c + 'A');
		}
		else putchar(c + 'a');
		return;
	}
	int x = c % 16;
	print(c / 16, dep + 1);
	putchar(x + 'A');
}
void dfs(int ro) {
	if (ro == -1) {
		printf ("257 ");
		return;
	}
	for (int j = 0; j < k ; ++j) {
		printf ("%d ", (int)(p + ro)->data[j] + 128);
	}
	dfs((p+ro)->left_child);
	dfs((p+ro)->right_child);
}
int main () {
	freopen ("data.in", "w", stdout);
	
	len = sizeof(node);
	fp = fopen("testdata.in", "a");
	n = ftell(fp) / len;
	fclose(fp);
	fp = fopen("testdata.in", "rb");

	p = malloc(len * n);
	fread(p, len, n, fp);
	dfs(0);
	free(p);
	return 0;
}
