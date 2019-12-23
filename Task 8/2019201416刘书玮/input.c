#include<stdio.h>
#define K 8
struct tree{
	int left, right;
	char bas[K];
};

void input(struct tree* _base, int n, int k)
{
	FILE *fp = fopen("8.in", "rb");
	fread(_base, k + 8, n, fp);
	fclose(fp);
}
