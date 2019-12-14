#ifdef COMPILETIME

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

static bool flag = 0;
static FILE *fp;

void *mymalloc(size_t siz) {
    if (!flag) {
        flag = 1;
        fp = fopen("test.out", "w");
    }
    void *p = malloc(siz);
    fprintf (fp, "malloc: %p\n", p);
    return p;
}
void myfree(void *p) {
    if (!flag) {
        flag = 1;
        fp = fopen("test.out", "w");
    }
    fprintf (fp, "free: %p\n", p);
    free(p);
}

#endif