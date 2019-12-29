#include<stdio.h>

void deoutput(char *_base, int size)
{
	//for(int i=0;i<size;i++){
	//	printf("%x",*(_base + i));
	//}
	FILE *fp = fopen("my.out", "wb");
	fwrite(_base, 1, size, fp);
	fclose(fp);
}
