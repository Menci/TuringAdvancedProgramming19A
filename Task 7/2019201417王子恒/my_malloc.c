#ifdef COMPLETETIME

#include <malloc.h>

void *my_malloc(size_t size) {
    void *tmp = malloc(size);
    printf("malloc(%d) = %p\n", (int)size, tmp);
    return tmp;
}
void my_free(void *tmp) {
    free(tmp);
    printf("free(%p)\n", tmp);
}

#endif
