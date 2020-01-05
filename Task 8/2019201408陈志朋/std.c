#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define k 8

FILE *fp;
int len;
typedef struct{
	int left_child, right_child;
	char data[k];
} node;
node *p, *q;
bool *t;
int n;

void dfs(int ro) {
	if (ro == -1) return;
	for (int j = 0; j < k ; ++j) {
		printf ("%d ", (p + ro)->data[j]);
	}
	puts("");
	dfs((p+ro)->left_child);
	dfs((p+ro)->right_child);
}

int main () {
	freopen ("std.out", "w", stdout);

	int i, j;
	len = sizeof(node);
	fp = fopen("testdata.in", "a");
	n = ftell(fp);
	fclose(fp);
	fp = fopen("testdata.in", "r");
	p = malloc(len * n);
	fread(p, len, n, fp);
	
	dfs(0);

	free(p);
	return 0;
}
