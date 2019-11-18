#include <stdio.h>
#include <string.h>

#define INSERT_LIMIT 20

typedef enum {
    ASCENDING, DESCENDING, RANDOM
}SORTED_TYPE;

void SpecialMemcopy(void *dest, void *src, size_t size) {
    switch (size) {
        case 1:
            *(char *)dest = *(char *)src;
            break;
        case 4:
            *(int *)dest = *(int *)src;
            break;
        case 8:
            *(long *)dest = *(long *)src;
            break;
        default:
            memcpy(dest, src, size);
    }
}

int IsSorted(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    void* lst = base + (nmemb - 1) * size;
    int up = 0, down = 0;
    for (void* i = base + size; i <= lst; i += size) {
        int comp = compar(i - size, i);
        if (comp > 0) down = 1;
        if (comp < 0) up = 1;
    }
    if (down && up) return RANDOM;
    if (down) return DESCENDING;
    return ASCENDING;
}

void Swap(void *a, void *b, size_t size) {
    char tmp[size];
    SpecialMemcopy(tmp, a, size);
    SpecialMemcopy(a, b, size);
    SpecialMemcopy(b, tmp, size);
}

void InsertSort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    void* lst = base + (nmemb - 1) * size;
    for (void *i = base + size; i <= lst; i += size) {
        char tmp[size];
        SpecialMemcopy(tmp, i, size);
        void *j = i;
        while (j > base && compar(tmp, j - size) < 0) {
            SpecialMemcopy(j, j - size, size);
            j -= size;
        }
        if (i == j) continue;
        SpecialMemcopy(j, tmp, size);
    }
}

void QuickSort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb <= INSERT_LIMIT) {
        InsertSort(base, nmemb, size, compar);
        return;
    }

    void *st = base, *ed = base + (nmemb - 1) * size;
    void *lst = ed;
    char lele[size], rele[size], mid[size], tmp[size];

    int a1, a2, a3, a4, a5;
    int seventh = (nmemb >> 3) + (nmemb >> 6) + 1;
    a3 = (nmemb - 1) >> 1;
    a2 = a3 - seventh;
    a1 = a2 - seventh;
    a4 = a3 + seventh;
    a5 = a4 + seventh;

    if (compar(base + a2 * size, base + a1 * size) < 0)                                                 // Sort a1~a5.
        Swap(base + a2 * size, base + a1 * size, size);
    if (compar(base + a3 * size, base + a2 * size) < 0) {
        Swap(base + a3 * size, base + a2 * size, size);
        if (compar(base + a2 * size, base + a1 * size) < 0)
            Swap(base + a2 * size, base + a1 * size, size);
    }
    if (compar(base + a4 * size, base + a3 * size) < 0) {
        Swap(base + a4 * size, base + a3 * size, size);
        if (compar(base + a3 * size, base + a2 * size) < 0) {
            Swap(base + a3 * size, base + a2 * size, size);
            if (compar(base + a2 * size, base + a1 * size) < 0)
                Swap(base + a2 * size, base + a1 * size, size);
        }
    }
    if (compar(base + a5 * size, base + a4 * size) < 0) {
        Swap(base + a5 * size, base + a4 * size, size);
        if (compar(base + a4 * size, base + a3 * size) < 0) {
            Swap(base + a4 * size, base + a3 * size, size);
            if (compar(base + a3 * size, base + a2 * size) < 0) {
                Swap(base + a3 * size, base + a2 * size, size);
                if (compar(base + a2 * size, base + a1 * size) < 0)
                    Swap(base + a2 * size, base + a1 * size, size);
            }
        }
    }

    if (compar(base + a2 * size, base + a4 * size)) {                                                   // Dual-Pivot Quick Sort.
        SpecialMemcopy(lele, base + a2 * size, size);
        SpecialMemcopy(rele, base + a4 * size, size);
        SpecialMemcopy(base + a2 * size, base, size);
        SpecialMemcopy(base + a4 * size, base + (nmemb - 1) * size, size);

        st += size;
        while (compar(st, lele) < 0 && st <= lst)
            st += size;
        ed -= size;
        while (compar(ed, rele) > 0 && ed >= base)
            ed -= size;

        void *p = st - size;
        int mark = 0;
        while ((p += size) <= ed) {
            SpecialMemcopy(tmp, p, size);
            if (compar(tmp, lele) <= 0) {
                SpecialMemcopy(p, st, size);
                SpecialMemcopy(st, tmp, size);
                st += size;
            }
            else if (compar(tmp, rele) >= 0) {
                while (compar(ed, rele) >= 0) {
                    if (ed <= p) {
                        ed -= size;
                        mark = 1;
                        break;
                    }
                    ed -= size;
                }
                if (mark) break;
                if (compar(ed, lele) <= 0) {
                    SpecialMemcopy(p, st, size);
                    SpecialMemcopy(st, ed, size);
                    st += size;
                }
                else
                    SpecialMemcopy(p, ed, size);
                SpecialMemcopy(ed, tmp, size);
                ed -= size;
            }
        }

        SpecialMemcopy(base, st - size, size);
        SpecialMemcopy(st - size, lele, size);
        SpecialMemcopy(base + (nmemb - 1) * size, ed + size, size);
        SpecialMemcopy(ed + size, rele, size);

        if (st - size - base > 0)
            QuickSort(base, (st - size - base) / size, size, compar);
        if (ed + 2 * size - base < nmemb * size)
            QuickSort(ed + 2 * size, nmemb - ((ed + 2 * size - base) / size), size, compar);

        while (!compar(st, lele) && st <= lst)
            st += size;
        while (!compar(ed, rele) && ed >= base)
            ed -= size;

        if (ed - st + size > 0)
            QuickSort(st, (ed - st + size) / size, size, compar);
    }
    else {                                                                                              // Single-Pivot Quick Sort.
        SpecialMemcopy(mid, base + a3 * size, size);
        while (st <= ed) {
            while (compar(st, mid) < 0)
                st += size;
            while (compar(ed, mid) > 0)
                ed -= size;
            if (st <= ed) {
                Swap(st, ed, size);
                st += size, ed -= size;
            }
        }

        if (ed + size - base > 0)
            QuickSort(base, (ed + size - base) / size, size, compar);
        if (nmemb * size > st - base)
            QuickSort(st, nmemb - (st - base) / size, size, compar);
    }
}

void Sort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    SORTED_TYPE type = IsSorted(base, nmemb, size, compar);

    if (type == ASCENDING) return;

    if (type == DESCENDING) {
        void *l = base, *r = base + (nmemb - 1) * size;
        while (l < r) {
            Swap(l, r, size);
            l += size, r -= size;
        }
        return;
    }

    QuickSort(base, nmemb, size, compar);
}

int main() {
    return 0;
}