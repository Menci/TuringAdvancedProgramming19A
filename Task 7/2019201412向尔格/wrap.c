#ifdef compile
#include <stdio.h>
#include <malloc.h>

void * Mymalloc (size_t size) {
	void * p = malloc (size);
	printf ("malloc(%d) = %p\n", (int)size, p);
	return p;
}

void Myfree (void * p) {
	free (p);
	printf ("free(%p)\n", p);
}
#endif
