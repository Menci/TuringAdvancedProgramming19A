#include<stdio.h>

int main()
{
	FILE* input = fopen("input.bin", "rb");
	FILE* output = fopen("zipped.bin", "rb");
	
	fseek(input, 0, SEEK_END);
	fseek(output, 0 ,SEEK_END);
	double isize = (double)ftell(input);
	double osize = (double)ftell(output);

	printf("rate:%.5f %d %d\n",osize/isize, (int)osize, (int)isize);

	return 0;
}
