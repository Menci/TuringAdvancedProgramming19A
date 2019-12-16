#ifdef COMPLETETIME
#ifndef _INC_DEBUG
#define _INC_DEBUG

#include <stdio.h>
#include <stdlib.h>

#define malloc(size) my_malloc(size)
#define free(tmp) my_free(tmp)

void *my_malloc(size_t size);
void my_free(void *tmp);

#endif
#endif
