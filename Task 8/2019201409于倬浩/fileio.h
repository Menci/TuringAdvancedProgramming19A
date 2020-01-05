#ifndef _FILEIO_H_
#define _FILEIO_H_
#include <stddef.h>
node* readOriginal(size_t *, const char *);
void* readCoded(size_t *);
void writeCoded(void *, size_t);
#endif