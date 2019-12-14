#include "fileio.h"
#include "tree.h"
#include <stdio.h>
#include <sys/stat.h>
void* readOriginal(size_t *size) {
	FILE *fp = fopen("input.bin", "r");
	struct stat statbuf;
    stat("input.bin", &statbuf);
    *size = statbuf.st_size;
	void *buffer = malloc(*size);
	fread(buffer, *size, 1, fp);
	fclose(fp);
	return buffer;
}
void* readCompressed(size_t *size) {
	FILE *fp = fopen("zipped.bin", "r");
	fread(size, sizeof(size_t), 1, fp);
	void *buffer = malloc(*size);
	fread(buffer, *size, 1, fp);
	fclose(fp);
	return buffer;
}
void writeCompressed(void *buffer, size_t size) {
	FILE *fp = fopen("zipped.bin", "w");
	fwrite(&size, sizeof(size_t), 1, fp);
	fwrite(buffer, size, 1, fp);
	fclose(fp);
}