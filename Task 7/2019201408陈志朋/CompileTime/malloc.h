#ifndef _MALLOC_H
#define _MALLOC_H

#define malloc(x) mymalloc(x)
#define free(x) myfree(x)

void *mymalloc(size_t siz);
void free(void *p);

#endif