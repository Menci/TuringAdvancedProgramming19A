#ifndef _FILEIO_H_
#define _FILEIO_H_
#include <stddef.h>
node* readOriginal(size_t *, const char *);
void* readCompressed(size_t *);
void writeCompressed(void *, size_t);
#endif