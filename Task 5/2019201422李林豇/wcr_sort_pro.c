#ifndef _DEBUG
#   pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#endif

#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DIRECT_SORT     (32)
#define MAX_THREAD_NUM  ((size_t)4)

typedef int (*compare_t)(const void*, const void*);
typedef unsigned char* pointer_t;

typedef struct {
    void *base, *pool;
    size_t cnt, size;
    compare_t cmp;
} directSortArg_t;

static void directMerge(void *base, size_t n1, size_t n2, size_t size, void *pool, compare_t cmp) {
    void *b1 = base, *b2 = base + n1 * size, *tmp = pool;
    while(n1 > 0 && cmp(b1, b2) <= 0) {
        base += size, b1 += size, --n1;
    }
    size_t total = n1 + n2;
    switch(size) {
        case sizeof(uint8_t):
            while(n1 > 0 && n2 > 0) {
                if(cmp(b1, b2) <= 0) {
                    *(uint8_t*)tmp = *(uint8_t*)b1;
                    b1 += size, --n1;
                } else {
                    *(uint8_t*)tmp = *(uint8_t*)b2;
                    b2 += size, --n2;
                }
                tmp += size;
            }
            break;
        case sizeof(uint64_t):
            while(n1 > 0 && n2 > 0) {
                if(cmp(b1, b2) <= 0) {
                    *(uint64_t*)tmp = *(uint64_t*)b1;
                    b1 += size, --n1;
                } else {
                    *(uint64_t*)tmp = *(uint64_t*)b2;
                    b2 += size, --n2;
                }
                tmp += size;
            }
            break;
        default:
            while(n1 > 0 && n2 > 0) {
                if(cmp(b1, b2) <= 0) {
                    memcpy(tmp, b1, size);
                    b1 += size, --n1;
                } else {
                    memcpy(tmp, b2, size);
                    b2 += size, --n2;
                }
                tmp += size;
            }
            break;
    }
    if(n1 > 0) {
        memcpy(tmp, b1, size * n1);
    }
    memcpy(base, pool, size * (total - n2));
}

static void directSort(void *_arg) {
    directSortArg_t *arg = (directSortArg_t*)_arg;
    void *base = arg -> base, *pool = arg -> pool;
    size_t cnt = arg -> cnt, size = arg -> size;
    compare_t cmp = arg -> cmp;
    if(cnt <= 1) return;

    size_t n1, n2;
    void *b1, *b2;
    n1 = cnt / 2, n2 = cnt - n1;
    b1 = base, b2 = base + n1 * size;

    directSortArg_t arg1 = {base : b1, pool : pool, cnt : n1, size : size, cmp : cmp};
    directSortArg_t arg2 = {base : b2, pool : pool, cnt : n2, size : size, cmp : cmp};

    directSort((void*)&arg1), directSort((void*)&arg2);
    directMerge(base, n1, n2, size, pool, cmp);
}

typedef struct {
    void **base, **pool;
    size_t cnt;
    compare_t cmp;
} indirectSortArg_t;

static void indirectMerge(void **base, size_t n1, size_t n2, void **pool, compare_t cmp) {
    void **src1 = base, **src2 = base + n1, **tmp = pool;
    while(n1 > 0 && cmp(*src1, *src2) <= 0) {
        ++base, ++src1, --n1;
    }
    size_t total = n1 + n2;
    while(n1 > 0 && n2 > 0) {
        if(cmp(*src1, *src2) <= 0) {
            *tmp = *src1;
            ++src1, --n1;
        } else {
            *tmp = *src2;
            ++src2, --n2;
        }
        tmp++;
    }
    if(n1 > 0) {
        memcpy(tmp, src1, sizeof(void*) * n1);
    }
    memcpy(base, pool, sizeof(void*) * (total - n2));
}

static void indirectSort(void *_arg) {
    indirectSortArg_t *arg = (indirectSortArg_t*)_arg;
    void **base = arg -> base, **pool = arg -> pool;
    size_t cnt = arg -> cnt;
    compare_t cmp = arg -> cmp;

    if(cnt <= 1) return;
    
    size_t n1, n2;
    void **b1, **b2;
    n1 = cnt / 2, n2 = cnt - n1;
    b1 = base, b2 = base + n1;

    indirectSortArg_t arg1 = {base : b1, pool : pool, cnt : n1, cmp : cmp};
    indirectSortArg_t arg2 = {base : b2, pool : pool, cnt : n2, cmp : cmp};

    indirectSort((void*)&arg1), indirectSort((void*)&arg2);
    indirectMerge(base, n1, n2, pool, cmp);
}

void slowSort(void *base, size_t cnt, size_t size, compare_t cmp) {
    size_t totSize = cnt * size;
    if(size > DIRECT_SORT)
        totSize = 2 * cnt * sizeof(void*) + size;

    pointer_t tmp = (pointer_t)malloc(totSize);
    pthread_t pid[MAX_THREAD_NUM];

    if(size > DIRECT_SORT) {
        pointer_t ip = (pointer_t)base;
        void **tp = (void**)(tmp + cnt * sizeof(void*));
        void *tmpStorage = (void*)(tp + cnt);
        for(void **t = tp; (void*)t < tmpStorage; ip += size) {
            *t++ = ip;
        }

        indirectSortArg_t args[MAX_THREAD_NUM];
        size_t per = cnt / MAX_THREAD_NUM, rem = cnt % MAX_THREAD_NUM;
        args[0] = (indirectSortArg_t) {
            .base = tp, 
            .pool = (void**)tmp, 
            .cnt = (per + rem), 
            .cmp = cmp
        };
        for(size_t i = 1; i < MAX_THREAD_NUM; i++) {
            args[i] = (indirectSortArg_t) {
                .base = tp + rem + i * per, 
                .pool = (void**)tmp + rem + i * per, 
                .cnt = per, 
                .cmp = cmp
            };
        }
        for(size_t i = 0; i < MAX_THREAD_NUM; i++)
            if(pthread_create(pid + i, NULL, (void*)indirectSort, (void*)(args + i))) {
                indirectSort((void*)(args + i));
                pid[i] = (pthread_t)-1;
            }
        for(size_t i = 0; i < MAX_THREAD_NUM; i++)
            if(~pid[i]) pthread_join(pid[i], NULL);
        for(size_t it = 1, tot = per + rem; it < MAX_THREAD_NUM; it++) {
            indirectMerge(tp, tot, per, (void**)tmp, cmp);
            tot += per;
        }

        pointer_t kp;
        ip = (pointer_t)base;
        for(size_t i = 0; i < cnt; i++, ip += size)
            if((kp = tp[i]) != ip) {
                size_t j = i;
                pointer_t jp = ip;
                memcpy(tmpStorage, ip, size);
                do {
                    size_t k = (size_t)(kp - (pointer_t)base) / size;
                    tp[j] = jp;
                    memcpy(jp, kp, size);
                    j = k, jp = kp, kp = tp[k];
                } while(kp != ip);
                tp[j] = jp;
                memcpy(jp, tmpStorage, size);
            }
    } else {
        directSortArg_t args[MAX_THREAD_NUM];
        size_t per = cnt / MAX_THREAD_NUM, rem = cnt % MAX_THREAD_NUM;
        args[0] = (directSortArg_t) {
            .base = base, 
            .pool = tmp, 
            .cnt = (per + rem), 
            .size = size, 
            .cmp = cmp
        };
        for(size_t i = 1; i < MAX_THREAD_NUM; i++) {
            args[i] = (directSortArg_t) {
                .base = (base + (rem + i * per) * size), 
                .pool = (tmp + (rem + i * per) * size), 
                .cnt = per, 
                .size = size, 
                .cmp = cmp
            };
        }
        for(size_t i = 0; i < MAX_THREAD_NUM; i++)
            if(pthread_create(pid + i, NULL, (void*)directSort, (void*)(args + i))) {
                directSort((void*)(args + i));
                pid[i] = (pthread_t)-1;
            }
        for(size_t i = 0; i < MAX_THREAD_NUM; i++)
            if(~pid[i]) pthread_join(pid[i], NULL);
        for(size_t it = 1, tot = per + rem; it < MAX_THREAD_NUM; it++) {
            directMerge(base, tot, per, size, tmp, cmp);
            tot += per;
        }
    }
    free(tmp);
}