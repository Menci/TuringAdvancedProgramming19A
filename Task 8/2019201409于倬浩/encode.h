#ifndef _ENCODE_H_
#define _ENCODE_H_
#include "tree.h"
#include <stddef.h>
void* encodeStructToArray(node *, size_t *, size_t);
void* compressArray(void *);
#endif