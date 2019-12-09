#include <stdio.h>
#include <stdlib.h>

#include "memcheck.h"

#undef malloc
#undef free

void* _malloc(size_t size) {
    void *res = malloc(size);
    fprintf(stderr, "Malloc(%ld) : %p\n", size, res);
    return res;
}

void _free(void* ptr) {
    fprintf(stderr, "Free : %p\n", ptr);
    return free(ptr);
}
