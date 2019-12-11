#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>

#define malloc(size) my_malloc(size)
#define free(ptr) my_free(ptr)

void *my_malloc(size_t size);

void my_free(void *ptr);

#endif

