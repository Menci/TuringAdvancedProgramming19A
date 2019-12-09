#include <stdio.h>
#include <malloc.h>

#ifdef COMPILE_TIME

void *my_malloc(size_t size)
{
	void *ptr = malloc(size);
	printf("%p : + %d\n", ptr, (int) size);
	return ptr;
}

void my_free(void *ptr)
{
	printf("%p : erased\n", ptr);
	free(ptr);
}

#else 

void *my_malloc(size_t size)
{
	return malloc(size);
}

void my_free(void *ptr)
{
	free(ptr);
}

#endif

