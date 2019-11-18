#   pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define max_len 100
#define max_width 1000
#define swap_divide_line 70
#define unaligned_num 63
#define char_bytes 1
#define int_bytes 4
#define long_long_bytes 8
const int divide_line = 1;
const int int_size = 4;
const int big_struct_line = 8;

static char *TEMP;
static char char_tmp;
static int int_tmp;
static long long long_long_tmp;

void memcpy_swap(char *a, char *b, int width)
{
    memcpy(TEMP, a, width);
    memcpy(a, b, width);
    memcpy(b, TEMP, width);
}

void selection_sort(char *head, size_t size, size_t width, int (*compar)(const void *, const void *))
{
    char *max, *p;
    int tmp;
    while(size>1)
    {
        max = head;
        tmp = size;
        p = head;
        while(--tmp)
        {
            p += width;
            if(compar(p, max)>0)
                max = p;
        }

        memcpy_swap(max, p, width);
        --size;
    }
}

static char *Array, *temp_ary;
unsigned long long *point_ary;
unsigned char *vis;



void merge_sort_char(void *head, size_t size, size_t width, int (*compar)(const void *, const void *))
{
    if(size <= divide_line)
    {
        // selection_sort(head, size, width, compar);
        return;
    }

    size_t size_l, size_r;
    void *head_l, *head_r , *pt;

    size_l = size>>1; 
    size_r = size - size_l;

    head_l = head; 
    head_r = head + size_l * width;

    pt = temp_ary;

    merge_sort_char(head_l, size_l, width, compar);
    merge_sort_char(head_r, size_r, width, compar);

    while(size_l > 0 && size_r > 0)
    {
        if(compar(head_l, head_r) > 0)
            *(char *)pt = *(char *)head_r, head_r += width, --size_r;
        else
            *(char *)pt = *(char *)head_l, head_l += width, --size_l;
        pt += width;
    }
    if(size_l)
        memcpy(pt, head_l, size_l * width);

    memcpy(head, temp_ary, (size - size_r) * width);
}

void merge_sort_long_long(void *head, size_t size, size_t width, int (*compar)(const void *, const void *))
{
    if(size <= divide_line)
    {
        // selection_sort(head, size, width, compar);
        return;
    }

    size_t size_l, size_r;
    unsigned long long *head_l, *head_r , *pt;

    size_l = size>>1; 
    size_r = size - size_l;

    head_l = head; 
    head_r = head + size_l * width;

    pt = temp_ary;

    merge_sort_long_long(head_l, size_l, width, compar);
    merge_sort_long_long(head_r, size_r, width, compar);

    while(size_l > 0 && size_r > 0)
    {
        if(compar(head_l, head_r) > 0)
            *pt = *head_r, head_r++, --size_r;
        else
            *pt = *head_l, head_l++, --size_l;
        pt++;
    }

    if(size_l)
        memcpy(pt, head_l, size_l * width);

    memcpy(head, temp_ary, (size - size_r) * width);
}

void merge_sort_normal(void *head, size_t size, size_t width, int (*compar)(const void *, const void *))
{
    if(size <= divide_line)
    {
        return;
    }

    size_t size_l, size_r;
    void *head_l, *head_r , *pt;

    size_l = size>>1; 
    size_r = size - size_l;

    head_l = head; 
    head_r = head + size_l * width;

    pt = temp_ary;

    merge_sort_normal(head_l, size_l, width, compar);
    merge_sort_normal(head_r, size_r, width, compar);

    while(size_l > 0 && size_r > 0)
    {
        if(compar(head_l, head_r) > 0)
            memcpy(pt, head_r, width), head_r += width, --size_r;
        else
            memcpy(pt, head_l, width), head_l += width, --size_l;
        pt += width;
    }

    if(size_l)
        memcpy(pt, head_l, size_l * width);

    memcpy(head, temp_ary, (size - size_r) * width);
}

void merge_sort_pointer(void *head, size_t size, size_t width, int (*compar)(const void *, const void *))
{
    if(size <= divide_line)
    {
        // selection_sort(head, size, width, compar);
        return;
    }

    size_t size_l, size_r;
    void **head_l, **head_r , **pt;

    size_l = size>>1; 
    size_r = size - size_l;

    head_l = head; 
    head_r = head + size_l * width;

    pt = temp_ary;

    merge_sort_pointer(head_l, size_l, width, compar);
    merge_sort_pointer(head_r, size_r, width, compar);

    while(size_l > 0 && size_r > 0)
    {
        if(compar(*head_l,*head_r) > 0)
            *pt = *head_r, head_r++, --size_r;
        else
            *pt = *head_l, head_l++, --size_l;
        pt++;
    }

    if(size_l)
        memcpy(pt, head_l, size_l * width);

    memcpy(head, temp_ary, (size - size_r) * width);
}


void big_struct_sort(void *base, size_t nmemb, size_t width, int (*compar)(const void *, const void *))
{
    temp_ary = malloc(nmemb * sizeof(void *) * 2  + width);
    point_ary = temp_ary + nmemb * sizeof(void *);
    TEMP = (void *)(point_ary + nmemb );

    char *ip = (char *)base;
    void **t = point_ary;
    size_t n = nmemb;

    // printf("%lld\n", base);
    while((void *) t < TEMP)
    {
        *t++ = ip;
        ip += width;
    }

    merge_sort_pointer(point_ary, nmemb, sizeof(void *), compar);

    char *kp;
    size_t i;

    for(i = 0,ip = (char *)base; i < nmemb; ++i, ip += width)
        if(point_ary[i] != ip)
        {   
            kp = point_ary[i];
            size_t x = i;
            char *xp = ip;
            memcpy (TEMP, ip, width);
            do
            {
                size_t k = (kp - (char *)base) / width;
                point_ary[x] = xp;
                memcpy(xp, kp, width);
                x = k;
                xp = kp;
                kp = point_ary[k];
            }   
            while (kp != ip);

            point_ary[x] = xp;
            memcpy(xp, TEMP, width);
        }

    free(temp_ary);
}
void my_sort(void *base, size_t nmemb, size_t width, int (*compar)(const void *, const void *))
{
    unsigned char is_sor = 1, is_rev = 1;
    int i=nmemb, tmp;
    void *head = base, *tail;
    while((is_sor||is_rev)&&(--i))
    {
        tmp = compar(head, tail = head + width);
        is_sor &= (tmp <= 0);
        is_rev &= (tmp >= 0);
        head = tail;
    }
    if(is_sor)
        return;
    if(is_rev)
    {
        TEMP = malloc(width);
        head = base;
        tail = base + (nmemb - 1) * width;
        while(head < tail)
        {
            memcpy_swap(head, tail, width);
            head += width;
            tail -= width;
        }
        free(TEMP);
        return;
    }
    if(width > big_struct_line)
    {
        big_struct_sort(base, nmemb, width, compar);
        return ;
    }

    Array = malloc (nmemb*(width + sizeof(void *)) + width);
    temp_ary = Array;
    TEMP = Array + nmemb*(width  + sizeof(void *));
    switch(width)
    {
        case char_bytes:
            merge_sort_char(base, nmemb, width, compar);break;
        case long_long_bytes:
            merge_sort_long_long(base, nmemb, width, compar);break;
        default:
            merge_sort_normal(base, nmemb, width, compar);break;
    }

    free(Array);
}

int comp(const void *x,const void *y)
{
    return (int)(*(int *)x) - (int)(*(int *)y); 
}
int a[2000020];
int main()
{
    int n=2000000;
    // scanf("%d",&n);
    for(int i = 1;i <= n;i++)
    {
        a[i] = n-i;
    }
    my_sort(a+1, n, sizeof(int), comp);
    for(int i=1;i<=n;i++)
        printf("%d ",a[i]);
    puts("");
    return 0;
}
