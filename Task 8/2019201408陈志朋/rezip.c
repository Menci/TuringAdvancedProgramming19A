#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define k 8

enum KIND {LEFT = 0, RIGHT = 1};

FILE *fp;
int len, sum = 0, n;
typedef struct{
	int left_child, right_child;
	char data[k];
} node;
node *p, *q;
unsigned *t;
unsigned tmp;
char ch;

void dfs(int ro) {
	if (ro == -1) return;
	for (int j = 0; j < k ; ++j) {
		printf ("%d ", (p + ro)->data[j]);
	}
	puts("");
	dfs((p+ro)->left_child);
	dfs((p+ro)->right_child);
}

bool tras(int ro, int fa, int kind) {
	int x; char ch;
	for (int i = 0; i < k; ++i) {
		x = 0;
		scanf ("%d", &x);
		if (x == 257) {
			if (fa != -1) {
				if (kind == LEFT) (p + fa)->left_child = -1;
				else (p + fa)->right_child = -1;
			}
			return false;
		}
		x -= 128;
		(p + ro)->data[i] = x;
	}
	if (fa != -1) {
		if (kind == LEFT) (p + fa)->left_child = ro;
		else (p + fa)->right_child = ro;
	}
	sum++;
	return true;
}

void rezip(int ro, int fa, int kind) {
	bool flag;
	flag = tras(ro, fa, kind);
	if (flag) {
		rezip(sum, ro, LEFT);
		rezip(sum, ro, RIGHT);
	}
}

int main () {
	freopen ("me.out", "r", stdin);
	freopen ("test.out", "w", stdout);
	FILE *fp = fopen("testdata.in", "a");
	n = ftell(fp);
	fclose(fp);
	len = sizeof(node);

	p = malloc(len * n);

	rezip(0, -1, -1);
	dfs(0);

	free(p);
	return 0;
}
