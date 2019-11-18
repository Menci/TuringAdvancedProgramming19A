#ifndef _DEBUG
#   pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#endif

 #include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

 #define DIRECT_SORT (32)

 typedef int (*compare_t)(const void*, const void*);
typedef unsigned char* pointer_t;

 static void directSort(void *base, size_t cnt, size_t size, compare_t cmp, void *pool) {
    if(cnt <= 1) return;

     size_t n1, n2;
    void *b1, *b2, *tmp = pool;
    n1 = cnt / 2, n2 = cnt - n1;
    b1 = base, b2 = base + n1 * size;

     directSort(b1, n1, size, cmp, pool);
    directSort(b2, n2, size, cmp, pool);

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
    memcpy(base, pool, size * (cnt - n2));
}

 static void indirectSort(void **base, size_t cnt, compare_t cmp, void **pool) {
    if(cnt <= 1) return;

     size_t n1, n2;
    void **b1, **b2, **tmp = pool;
    n1 = cnt / 2, n2 = cnt - n1;
    b1 = base, b2 = base + n1;

     indirectSort(b1, n1, cmp, pool);
    indirectSort(b2, n2, cmp, pool);

     while(n1 > 0 && n2 > 0) {
        if(cmp(*b1, *b2) <= 0) {
            *tmp = *b1;
            ++b1, --n1;
        } else {
            *tmp = *b2;
            ++b2, --n2;
        }
        tmp++;
    }

     if(n1 > 0) {
        memcpy(tmp, b1, sizeof(void*) * n1);
    }
    memcpy(base, pool, sizeof(void*) * (cnt - n2));
}

 void slowSort(void *base, size_t cnt, size_t size, compare_t cmp) {
    size_t totSize = cnt * size;
    if(size > DIRECT_SORT)
        totSize = 2 * cnt * sizeof(void*) + size;

     pointer_t tmp = (pointer_t)malloc(totSize);

     if(size > DIRECT_SORT) {
        pointer_t ip = (pointer_t)base;
        void **tp = (void**)(tmp + cnt * sizeof(void*));
        void *tmpStorage = (void*)(tp + cnt);

         for(void **t = tp; (void*)t < tmpStorage; ip += size) {
            *t++ = ip;
        }

         indirectSort(tp, cnt, cmp, (void**)tmp);

         pointer_t kp;
        size_t i;
        for(i = 0, ip = (pointer_t)base; i < cnt; i++, ip += size)
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
        directSort(base, cnt, size, cmp, tmp);
    }
    free(tmp);
}