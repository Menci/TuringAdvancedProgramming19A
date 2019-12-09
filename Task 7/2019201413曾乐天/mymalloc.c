#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef unsigned long long ull;

char to16(int x)
{
	if(x < 10)return x ^ 48;
	else return 'A' + x - 10;
}

void _fwrite16(ull x, FILE *f)
{
	if(x > 15)_fwrite16(x / 16, f);
	putc(to16(x % 16), f);
}

void fwrite16(ull x, FILE *f)
{
	putc('0', f);
	putc('x', f);
	_fwrite16(x, f);
}

void outstring(const char *str, FILE *f)
{
	char *ptr = (char*)str;
	while(*ptr != '\n')
		putc(*(ptr++), f);
}

void *malloc(size_t size)
{
	void *(*mall)(size_t size);
	char *err;
	mall = dlsym(RTLD_NEXT, "malloc");	
	if((err = dlerror()) != NULL)
	{
		putc('Q', stderr);
		exit(-1);
	}
	void *ptr = mall(size);

	outstring("malloc \n", stderr);
	fwrite16((ull)ptr, stderr);
	putc('\n', stderr);
	
	return ptr;
}


void free(void *ptr)
{
	void *(*freeze)(void *ptr);
	freeze = dlsym(RTLD_NEXT, "free");	
	
	outstring("free \n", stderr);
	fwrite16((ull)ptr, stderr);
	putc('\n', stderr);

	freeze(ptr);
}

