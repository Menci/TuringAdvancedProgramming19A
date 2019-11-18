#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define N 16
#define n 15
#define lim_ins 20
#define lim_jud 1000
#define maxn 400

int a[N], m;
void *tmp[maxn];

int cmp(const void *i, const void *j) {
    return ( (*(int*)i) >= (*(int*)j)) ? (1) : (-1);
}
int tans(void *i, void *j, size_t width) {
    return (i - j) / width;
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
void qsort(void *base, size_t nitems, size_t width, int (*fcmp)(const void *, const void *)) {
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
    if (l < j) qsort(base, (j - base) / width + 1, width, fcmp);
    if (i < r) qsort(   i, (r -    i) / width + 1, width, fcmp);
}

int main () {
    puts("--time");
    return 0;
}