#ifndef _ENCODE_H_
#define _ENCODE_H_
#include "tree.h"
#include <stddef.h>
void encodeDFS(int);
void* encodeStructToArray(node *, size_t *, size_t);
#endif