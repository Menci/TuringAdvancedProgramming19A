#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int size_of_(FILE* fileHandle)
{
	int file_curpos = ftell(fileHandle);
	int fileSize = 0;

	fseek(fileHandle, 0, SEEK_END);
	fileSize = ftell(fileHandle);

	fseek(fileHandle, file_curpos, SEEK_SET);

	return fileSize;
}

int main(int argc, char **argv)
{
	FILE* fileHandle = fopen("data.in", "rb");
	int orgSize = size_of_(fileHandle);
	fclose(fileHandle);

	fileHandle = fopen("data_zip.bin", "rb");
	int curSize = size_of_(fileHandle);
	fclose(fileHandle);

	printf("rate : %.2lf%%\n", 100.0 * curSize / orgSize);
	return 0;
}

