#ifndef MEM_CHECK_H_

#define MEM_CHECK_H_

#include <stddef.h>

#if MEM_CHECK || _LOCAL || _DEBUD

#define malloc(size) _malloc(size)
#define free(ptr) _free(ptr)

void* _malloc(size_t size);
void _free(void* ptr);

#endif

#endif