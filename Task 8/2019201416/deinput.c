#include<stdio.h>

int deinput(char* _base, int size)
{
	FILE *fp = fopen("1.out", "rb");
	int t = fread(_base, 1, size, fp);
	fclose(fp);
	return t;
}
