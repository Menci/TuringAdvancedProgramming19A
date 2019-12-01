#include <math.h>
#include <stdio.h>
#include <string.h>

size_t size;
void *base;
int (*compar)(const void *, const void *);
void sort(void *arr, void *left, void *right);

void swap(void *a, void *b, size_t size) {
    static char tmp[1010];
    memcpy(tmp, a, size), memcpy(a, b, size), memcpy(b, tmp, size);
}

// 8 byte
void insertSort8Byte(void *a, int len) {
    long long *pos = a, tmpp;
    for (int i = 1; i < len; i++) {
        tmpp = *(long long *)(a + i * 8);
        pos = a + (i - 1) * 8;
        while (pos >= (long long *)a && compar(pos, &tmpp) > 0)
            *(pos + 1) = *pos, pos--;
        *(pos + 1) = tmpp;
    }
}
void *partition8Byte(void *arr, void *left, void *right) {
    long long *i = left, *j = right, tmpp = *(long long *)left;
    while (i < j) {
        while (i < j && (compar(j, &tmpp) > 0)) j--;
        if (i < j) *i = *j, i++;
        while (i < j && (compar(i, &tmpp) <= 0)) i++;
        if (i < j) *j = *i, j--;
    }
    *i = tmpp;
    return i;
}
void sort8Byte(void *arr, void *left, void *right) {
    if (right - left <= 8 * 8) {
        insertSort8Byte(left, (right - left) / size + 1);
        return;
    }
    void *j;
    j = partition8Byte(arr, left, right);
    if (j - size >= left) sort8Byte(arr, left, j - 8);
    if (right >= j + size) sort8Byte(arr, j + 8, right);
}

// 1 byte
void sortOneByte(void *base, size_t nmemb) {
    int num[256];
    memset(num, 0, sizeof(num));
    char sbase[255];
    for (int i = 0; i <= 255; i++) sbase[i] = i;
    for (int j = 0; j < nmemb; j++) {
        int x = *((char *)base + j);
        if (x < 0) x += 256;
        num[x]++;
    }
    sort(sbase, sbase, sbase + 255);
    char *tmp = sbase;
    for (int i = 0; i <= 255; i++, tmp++) {
        for (int j = 1; j <= num[i]; j++) *(char *)base = *tmp, base++;
    }
}

// all byte
void insertSort(void *a, int len, size_t size) {
    int i;
    void *pos = a;
    static char tmp[1010];
    for (i = 1; i < len; i++) {
        memcpy(tmp, a + i * size, size);
        pos = a + (i - 1) * size;
        while (pos >= a && compar(pos, tmp) > 0)
            memcpy(pos + size, pos, size), pos -= size;
        memcpy(pos + size, tmp, size);
    }
}
void *partition(void *arr, size_t size, void *left, void *right) {
    static char tmp[1010];
    void *i = left, *j = right;
    swap(left, left + (int)((right - left) / size / 2) * size, size);
    memcpy(tmp, left, size);
    while (i < j) {
        while (i < j && (compar(j, tmp) > 0)) j -= size;
        if (i < j) memcpy(i, j, size), i += size;
        while (i < j && (compar(i, tmp) <= 0)) i += size;
        if (i < j) memcpy(j, i, size), j -= size;
    }
    memcpy(i, tmp, size);
    return i;
}
void sort(void *arr, void *left, void *right) {
    if (right - left <= 8 * size) {
        insertSort(left, (right - left) / size + 1, size);
        return;
    }
    void *j;
    j = partition(arr, size, left, right);
    if (j - size >= left) sort(arr, left, j - size);
    if (right >= j + size) sort(arr, j + size, right);
}

int ifSorted(size_t nmemb) {
    int flag = 0;
    void *tmp = base;
    for (int i = 0; i < nmemb - 1; i += 5, tmp += size) {
        if (compar(tmp, tmp + size) > 0) return 0;
    }
    return 1;
}
int ifReversedSorted(size_t nmemb) {
    int flag = 0;
    void *tmp = base;
    for (int i = 0; i < nmemb - 1; i += 5, tmp += size) {
        if (compar(tmp, tmp + size) < 0) return 0;
    }
    if (!flag) {
        void *b1, *b2;
        b1 = base, b2 = base + (nmemb - 1) * size;
        while (b1 <= b2) swap(b1, b2, size), b1 += size, b2 -= size;
    }
    return 1;
}
void My_sort(void *_base, size_t nmemb, size_t _size,
                         int (*cmp)(const void *, const void *)) {
    size = _size, base = _base, compar = cmp;
    if (ifSorted(nmemb)) return;
    if (ifReversedSorted(nmemb)) return;
    if (size == 1) {
        sortOneByte(base, nmemb);
        return;
    }
    if (size == 8) {
        sort8Byte(base, base, base + (nmemb - 1) * size);
        return;
    }
    sort(base, base, base + (nmemb - 1) * size);
}