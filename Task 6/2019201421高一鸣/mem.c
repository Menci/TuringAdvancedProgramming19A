#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
void* newNode(size_t size) {
	memh+=size;
	return memh-size;
}
void initMem(int n,size_t size) {
	memPool=malloc((n+20)*size);
	memh=memPool;
}
void clearMem() {
	free(memPool);
}