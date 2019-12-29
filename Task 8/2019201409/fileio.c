#include "tree.h"
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
node* readOriginal(size_t *nmemb, const char* filename) {
	FILE *fp = fopen(filename , "r");
	struct stat statbuf;
    stat(filename , &statbuf);
    *nmemb = statbuf.st_size / sizeof(node);
	printf("[input.bin] size = %d Bytes, K = %d, n = %d\n", statbuf.st_size, K, *nmemb);
	node *buffer = malloc(sizeof(node) * (*nmemb) * 2);
	memset(buffer, 0, sizeof(node) * (*nmemb) * 2);
	fread(buffer, sizeof(node) * (*nmemb), 1, fp);
	fclose(fp);
	return buffer;
}
void* readCoded(size_t *size) {
	FILE *fp = fopen("zipped.bin", "r");
	struct stat statbuf;
    stat("zipped.bin", &statbuf);
    *size = statbuf.st_size;
	void *buffer = malloc((*size) * 2);
	fread(buffer, *size, 1, fp);
	fclose(fp);
	return buffer;
}
void writeCoded(void *buffer, size_t size) {
	FILE *fp = fopen("zipped.bin", "w");
	fwrite(buffer, size, 1, fp);
	fclose(fp);
}