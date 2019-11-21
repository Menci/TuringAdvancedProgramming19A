#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define swap(p, q) memcpy(tmp, p, size), memcpy(p, q, size), memcpy(q, tmp, size);

int a[1005], n;

void quicksort(void *head, void *tail, size_t size, int (*cmp)(const void *, const void *), void *tmp)
{
    int flag;
    if (tail < head + (size << 2))
    {
        void *ptr;
        for (ptr = head + size; ptr < tail; ptr += size)
            if (cmp(ptr - size, ptr) > 0)
                break;
        if (tail <= ptr)
            return;
    }
    void *leftptr = head, *rightptr = tail - size;
    void *pivot = tmp + size;
    memcpy(pivot, head, size);
    while (leftptr < rightptr)
    {
        while (leftptr < rightptr) {
            flag = cmp(pivot, rightptr);
            if(flag > 0) break;
            rightptr -= size;
            if(flag == 0) break;
        }
        memcpy(leftptr, rightptr, size);
        while (leftptr < rightptr) {
            flag = cmp(pivot, leftptr);
            if(flag < 0) break;
            leftptr += size;
            if(flag == 0) break;
        }
        memcpy(rightptr, leftptr, size);
    }
    memcpy(leftptr, pivot, size);
    quicksort(head, leftptr, size, cmp, tmp);
    quicksort(rightptr + size, tail, size, cmp, tmp);
}

void fpdsort(void *head, size_t nmemb, size_t size, int (*cmp)(const void *, const void *))
{
    void *tmp = malloc(size << 1);
    void *leftptr = head, *rightptr;
    for (unsigned int i = 0; i < nmemb; i++)
    {
        rightptr = head + (i + rand() % (nmemb - i)) * size;
        swap(leftptr, rightptr);
        leftptr += size;
    }
    quicksort(head, head + nmemb * size, size, cmp, tmp);
    free(tmp);
}

int main()
{
    return 0;
}