#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>

#define BUFFER_SIZE 25

void *malloc(size_t size) {
    void *(*sys_malloc)(size_t size);
    char *error, *ptr;
    char buf[BUFFER_SIZE] = {0};

    sys_malloc = dlsym(RTLD_NEXT, "malloc");
    if ((error = dlerror()) != NULL) {
        fputs("Unexpected error occured.\n", stderr);
        fputs(error, stderr);
        exit(1);
    }

    ptr = sys_malloc(size);
    sprintf(buf, "malloc %lu\n", size);
    write(STDERR_FILENO, buf, sizeof(buf));
    return ptr;
}

void free(void *ptr) {
    void (*sys_free)(void *);
    char *error;
    char buf[BUFFER_SIZE] = {0};

    sys_free = dlsym(RTLD_NEXT, "free");
    if ((error = dlerror()) != NULL) {
        fputs("Unexpected error occured.\n", stderr);
        fputs(error, stderr);
        exit(1);
    }

    sys_free(ptr);
    sprintf(buf, "free %p\n", ptr);
    write(STDERR_FILENO, buf, sizeof(buf));
    return;
}