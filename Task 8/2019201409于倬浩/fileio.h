#ifndef _FILEIO_H_
#define _FILEIO_H_
#include <stddef.h>
void* readOriginal(size_t *);
void* readCompressed(size_t *);
void writeCompressed(void *, size_t);
#endif