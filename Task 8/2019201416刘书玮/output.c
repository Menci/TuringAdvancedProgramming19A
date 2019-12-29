#include<stdio.h>
#include<stdlib.h>

void output(char *_data_base, int lenth)
{
	FILE *fp = fopen("1.out","wb");
	char *addtion = malloc(1);
	*addtion = lenth % 8;
	fwrite(addtion, 1, 1, fp);
	fwrite(_data_base, 1, (lenth + 7) / 8, fp);
	fclose(fp);
}
