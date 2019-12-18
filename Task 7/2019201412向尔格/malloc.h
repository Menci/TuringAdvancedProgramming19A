#define malloc(h) Mymalloc(h)
#define free(h) Myfree(h)

void * Mymalloc(size_t size);
void Myfree(void *p);
