#ifdef RUNTIME
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_BUFFER_SIZE 50
void *malloc(size_t size) {
	void *(*real_malloc)(size_t size);
	real_malloc = dlsym(RTLD_NEXT, "malloc");
	void *ret = real_malloc(size);
	static char bufm[MAX_BUFFER_SIZE];
	sprintf(bufm, "malloc(%lu) = %p;\n", size, ret);
	write(STDERR_FILENO, bufm, sizeof(bufm));
	return ret;
}
void free(void *p) {
	void (*real_free)(void *);
	real_free = dlsym(RTLD_NEXT, "free");
	real_free(p);
	static char buff[MAX_BUFFER_SIZE];
	sprintf(buff, "free(%p);\n", p);
	write(STDERR_FILENO, buff, sizeof(buff));
	return;
}
#undef MAX_BUFFER_SIZE
#endif