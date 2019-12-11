#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

bool Taowa = 0;
FILE *ffre, *fmlc;
FILE *rfre, *rmlc;
void openfile()
{
	ffre = fopen("ffre.out","w");
	fmlc = fopen("fmlc.out","w");
}

void* my_malloc(size_t size)
{
	void* tmp = malloc(size);
	if(Taowa)
		return tmp;
	Taowa = 1;
	fprintf(fmlc,"%lld\n", (long long)tmp);
	Taowa = 0;
	return tmp;
}

void* my_free(void* pt)
{	
	free(pt);
	Taowa = 1;
	fprintf(ffre,"%lld\n", (long long)pt);
	Taowa = 0;
}

void check_memory()
{
	fclose(ffre);
	fclose(fmlc);
	rfre = fopen("ffre.out","r");
	rmlc = fopen("fmlc.out","r");
	long long x, sumfree = 0, sumalloc = 0;
	while(fscanf(rfre,"%lld",&x) != EOF)
		sumfree ^= x;
	fclose(rfre);
	while(fscanf(rmlc,"%lld",&x) != EOF)
		sumalloc ^= x;
	fclose(rmlc);
	if(sumfree == sumalloc)
		puts("No memory leak :)");
	else
		puts("Memory leak!");
	return;
}