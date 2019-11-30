#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <pthread.h>

#define N 20000
#define n 30
#define lim_ins 20
#define lim_jud 1000
#define maxn 400
#define num_of_pthread 2

int a[N], m, id;
void *tmp[maxn];
pthread_t pid[num_of_pthread + 1];

typedef struct {
    void * base;
    size_t nitems;
    size_t width;
    int (*fcmp)(const void *, const void *);
}node;

int cmp(const void *i, const void *j) {
    return ( (*(int*)i) >= (*(int*)j)) ? (1) : (-1);
}
int tans(void *i, void *j, size_t width) {
    return (j - i) / width;
}
void print(int *tmp) {
    for (int i = 0; i < n; ++i) printf("%d ", tmp[i]); puts("");
}
void swap(void *i, void *j, int width) {
    memcpy(tmp,   i, width);
    memcpy(  i,   j, width);
    memcpy(  j, tmp, width);
}
bool judge(void *base, size_t nitems, size_t width, int (*fcmp)(const void *, const void *)) {
    void *End = base + (nitems - 1) * width, *i = base;
    for (; i < End; i += width) {
        if (fcmp(i, i + width) > 0) return false;
        i += width; if (i < End && fcmp(i, i + width) > 0) return false;
        i += width; if (i < End && fcmp(i, i + width) > 0) return false;
        i += width; if (i < End && fcmp(i, i + width) > 0) return false;
    }
    return true;
}
void ins_sort(void *base, size_t nitems, size_t width, int (*fcmp)(const void *, const void *)) {
    void *i, *j, *x, *End = base + (nitems - 1) * width;
    for (i = base + width; i <= End; i += width) {
        for (j = base; j < i; j += width) {
            if (fcmp(i, j) < 0) break;
            j += width; if (fcmp(i, j) < 0) break;
            j += width; if (fcmp(i, j) < 0) break;
            j += width; if (fcmp(i, j) < 0) break;
        }
        if (j < i) {
            memcpy(tmp, i, width);
            for (x = i; x > j; x -= width) {
                memcpy(x, x - width, width);
                x -= width; if (x > j) memcpy(x, x - width, width);
                x -= width; if (x > j) memcpy(x, x - width, width);
                x -= width; if (x > j) memcpy(x, x - width, width);
            }
            memcpy(j, tmp, width);
        }
    }
}
void Qsort(void *obj) {
    node *t = (node *)obj;
    void * base = t->base; 
    size_t nitems = t->nitems, width = t->width;
    int (*fcmp)(const void *, const void *) = t->fcmp;
    if (width < lim_jud && judge(base, nitems, width, fcmp)) return;
    if (nitems <= lim_ins) {
        ins_sort(base, nitems, width, fcmp);
        return;
    }
    void *l, *r, *i, *j, *x;
    i = l = base; j = r = base + (nitems - 1) * width;
    x = base + (nitems >> 1) * width;
    while (i <= j) {
        while (fcmp(i, x) < 0) i += width;
        while (fcmp(x, j) < 0) j -= width;
        if (i <= j) {
            if (i == x) x = j;
            else if (j == x) x = i;
            memcpy(tmp,   i, width);
            memcpy(  i,   j, width);
            memcpy(  j, tmp, width);
            i += width; j -= width; 
        }
    }
    if (l < j) {
        t->nitems = (j - base) / width + 1;
        Qsort(t);
    }
    if (i < r) {
        t->base = i; t->nitems = (r -    i) / width + 1;
        Qsort(t);
    }
}
void qsort(void *base, size_t nitems, size_t width, int (*fcmp)(const void *, const void *)) {
    node *obj; id = nitems / num_of_pthread;
    obj->nitems = id; obj->width = width; obj->fcmp=fcmp;
  /*  for (int i = 1; i < num_of_pthread; ++i) {
        printf ("%d ", id);
        obj->base = base + (i - 1) * width * id;
        pthread_create(&pid[i], NULL, (void *)Qsort, (void *)obj);
    }*/
/*
    obj.nitems = nitems - id * (num_of_pthread - 1);
    printf ("%d\n", nitems - id * (num_of_pthread - 1));
    obj.base = base + (num_of_pthread - 1) * width * id;
    pthread_create(&pid[num_of_pthread], NULL, (void *)Qsort, (void *)&obj);
*/
/*
    for (int i = 1; i < num_of_pthread; ++i) {
        pthread_join(pid[i], NULL);
    }*/
    obj->base = base; obj->nitems = nitems;
    pthread_create(&pid[0], NULL, (void *)Qsort, (void *)obj);
    pthread_join(pid[0], NULL);
}
int main () {
    srand(19260817);
    for (int i = 0; i < n; ++i) a[i] = rand()%10;
    for (int i = 0; i < n; ++i) printf ("%d ", a[i]); printf ("\n");
    qsort(a, n, sizeof(a[0]), cmp);
    for (int i = 0; i < n; ++i) printf ("%d ", a[i]); printf ("\n");
    return 0;
}