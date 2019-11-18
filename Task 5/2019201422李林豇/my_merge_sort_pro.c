#   pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")

#include <pthread.h>
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
#define pthread_num 2

const int divide_line = 1;
const int int_size = 4;
const int big_struct_line = 8;

typedef int (*cmp_pointer)(const void*, const void*);

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

static char *Array, *temp_ary;
unsigned long long *point_ary;
unsigned char *vis;

typedef struct {
    void *head;
    size_t size,width;
    cmp_pointer compar;
    void *temp_ary;
}merge_arg;

void merge_char(void *head_l, size_t size_l, void *head_r, size_t size_r, size_t width, cmp_pointer compar, void *temp_ary)
{
    void *pt = temp_ary, *head = head_l;
    size_t size = size_l + size_r;

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

void merge_sort_char(merge_arg *arg)
{
    if(arg->size <= divide_line)
    {
        // selection_sort(head, size, width, compar);
        return;
    }

    size_t size_l, size_r;
    void *head_l, *head_r;

    size_l = (arg -> size)>>1; 
    size_r = (arg -> size) - size_l;

    head_l = arg -> head; 
    head_r = arg -> head + size_l * (arg -> width);

    merge_arg arg1,arg2;
    arg1 = (merge_arg){head_l, size_l, arg->width, arg->compar, arg->temp_ary};
    arg2 = (merge_arg){head_r, size_r, arg->width, arg->compar, arg->temp_ary};
    merge_sort_char(&arg1);
    merge_sort_char(&arg2);

    merge_char(head_l, size_l, head_r, size_r,arg -> width,arg -> compar, arg -> temp_ary);
}

void merge_long_long(unsigned long long *head_l, size_t size_l, unsigned long long *head_r, size_t size_r, size_t width, cmp_pointer compar, void *temp_ary)
{
    unsigned long long *pt = temp_ary;
    void  *head = (void *)head_l;
    size_t size = size_l + size_r;

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
void merge_sort_long_long(merge_arg *arg)
{
   if(arg->size <= divide_line)
    {
        // selection_sort(head, size, width, compar);
        return;
    }

    size_t size_l, size_r;
    unsigned long long *head_l, *head_r;

    size_l = (arg -> size)>>1; 
    size_r = (arg -> size) - size_l;

    head_l = arg -> head; 
    head_r = arg -> head + size_l * (arg -> width);

    merge_arg arg1,arg2;
    arg1 = (merge_arg){head_l, size_l,arg -> width,arg -> compar, arg ->  temp_ary};
    arg2 = (merge_arg){head_r, size_r,arg -> width,arg -> compar, arg ->  temp_ary};
    merge_sort_long_long(&arg1);
    merge_sort_long_long(&arg2);

    merge_long_long(head_l, size_l, head_r, size_r,arg -> width,arg -> compar, arg->temp_ary);
}

void merge_normal(void *head_l, size_t size_l, void *head_r, size_t size_r, size_t width, cmp_pointer compar, void *temp_ary)
{
    void *pt = temp_ary, *head = head_l;
    size_t size = size_l + size_r;

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

void merge_sort_normal(merge_arg *arg)
{
    if(arg->size <= divide_line)
    {
        // selection_sort(head, size, width, compar);
        return;
    }

    size_t size_l, size_r;
    void *head_l, *head_r;

    size_l = (arg -> size)>>1; 
    size_r = (arg -> size) - size_l;

    head_l = arg -> head; 
    head_r = arg -> head + size_l * (arg -> width);

    merge_arg arg1,arg2;
    arg1 = (merge_arg){head_l, size_l,arg -> width,arg -> compar};
    arg2 = (merge_arg){head_r, size_r,arg -> width,arg -> compar};
    merge_sort_normal(&arg1);
    merge_sort_normal(&arg2);
    merge_normal(head_l, size_l, head_r, size_r,arg -> width,arg -> compar,arg -> temp_ary);
//     while(size_l > 0 && size_r > 0)
//     {
//         if(arg -> compar(head_l, head_r) > 0)
//             memcpy(pt, head_r, arg -> width), head_r += arg -> width, --size_r;
//         else
//             memcpy(pt, head_l, arg -> width), head_l += arg -> width, --size_l;
//         pt += arg -> width;
//     }

//     if(size_l)
//         memcpy(pt, head_l, size_l * (arg -> width));

//     memcpy(arg -> head, temp_ary, (arg -> size - size_r) * (arg -> width));
}

void merge_pointer(void **head_l, size_t size_l, void **head_r, size_t size_r, size_t width, cmp_pointer compar, void *temp_ary)
{
    void **pt = temp_ary, **head = head_l;
    size_t size = size_l + size_r;

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
void merge_sort_pointer(merge_arg *arg)
{
    if(arg->size <= divide_line)
    {
        // selection_sort(head, size, width, compar);
        return;
    }

    size_t size_l, size_r;
    void **head_l, **head_r , **pt;

    size_l = (arg -> size)>>1; 
    size_r = (arg -> size) - size_l;

    head_l = arg -> head; 
    head_r = arg -> head + size_l * (arg -> width);

    merge_arg arg1,arg2;
    arg1 = (merge_arg){head_l, size_l,arg -> width,arg -> compar,arg ->  temp_ary};
    arg2 = (merge_arg){head_r, size_r,arg -> width,arg -> compar,arg ->  temp_ary};
    merge_sort_pointer(&arg1);
    merge_sort_pointer(&arg2);

    merge_pointer(head_l, size_l, head_r, size_r,arg -> width,arg -> compar,arg -> temp_ary);
    // while(size_l > 0 && size_r > 0)
    // {
    //     if(arg -> compar(*head_l,*head_r) > 0)
    //         *pt = *head_r, head_r++, --size_r;
    //     else
    //         *pt = *head_l, head_l++, --size_l;
    //     pt++;
    // }

    // if(size_l)
    //     memcpy(pt, head_l, size_l * (arg -> width));

    // memcpy(arg -> head, temp_ary, (arg -> size - size_r) * (arg -> width));
}

void big_struct_sort(void *base, size_t nmemb, size_t width, cmp_pointer compar)
{
    temp_ary = malloc(nmemb * sizeof(void *) * 2  + width);
    point_ary = temp_ary + nmemb * sizeof(void *);
    TEMP = (void *)(point_ary + nmemb);

    char *ip = (char *)base;
    void **t = point_ary;
    size_t n = nmemb;

    // printf("%lld\n", base);
    while((void *) t < TEMP)
    {
        *t++ = ip;
        ip += width;
    }

    pthread_t pth_id[pthread_num];
    size_t rem = nmemb % pthread_num, div = nmemb / pthread_num, all_s = 1;
    merge_arg arg[pthread_num];
    while(all_s < pthread_num)
        all_s <<= 1;
    arg[0] = (merge_arg){point_ary, div + rem, sizeof(void *), compar, temp_ary}; 
    for(int i = 1; i < pthread_num;i++)
        arg[i] = (merge_arg) {arg[i-1].head + (arg[i-1].size) * sizeof(void *), div, sizeof(void *), compar, arg[i-1].temp_ary + (arg[i-1].size) * sizeof(void *)};

    for(int i = 0; i < pthread_num; i++)
        if(pthread_create(pth_id + i, NULL, (void *)merge_sort_pointer, (void *)&arg[i]))
        {
            Warnning();
            return;
        }
        for(int i = 0; i < pthread_num; i++)
            pthread_join(pth_id[i], NULL);
        for(int s = 1; s < all_s; s<<=1)
            for(int i = 0; i+s < pthread_num;i += s)
            {
                merge_pointer(arg[i].head, arg[i].size, arg[i+s].head, arg[i+s].size, sizeof(void *), compar, temp_ary);
                arg[i].size += arg[i+s].size;
            }
    
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

void Warnning()
{
    puts("Your computer can't afford so much pthreads.");
    puts("We advice you to buy another one at www.taobao.com.");
}
void check(int l,int r);
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
    // Warnning();
    // merge_arg arg = (merge_arg) {base, nmemb, width, compar, temp_ary};
    pthread_t pth_id[pthread_num];
    size_t rem = nmemb % pthread_num, div = nmemb / pthread_num, all_s = 1;
    merge_arg arg[pthread_num];
    while(all_s < pthread_num)
        all_s <<= 1;
    arg[0] = (merge_arg){base, div + rem, width, compar, temp_ary}; 
    for(int i = 1; i < pthread_num;i++)
        arg[i] = (merge_arg) {arg[i-1].head + (arg[i-1].size) * width, div, width, compar, arg[i-1].temp_ary + (arg[i-1].size) * width};
    switch(width)
    {
        case char_bytes:
            for(int i = 0; i < pthread_num; i++)
                if(pthread_create(pth_id + i, NULL, (void *)merge_sort_char, (void *)&arg[i]))
                {
                    Warnning();
                    return;
                }
            for(int i = 0; i < pthread_num; i++)
                pthread_join(pth_id[i], NULL);
            for(int s = 1; s < all_s; s<<=1)
                for(int i = 0; i+s < pthread_num;i += s)
                {
                    merge_char(arg[i].head, arg[i].size, arg[i+s].head, arg[i+s].size, width, compar, arg[i].temp_ary);
                    arg[i].size += arg[i+s].size;
                }
            break;
        case long_long_bytes:
            for(int i = 0; i < pthread_num; i++)
                if(pthread_create(pth_id + i, NULL, (void *)merge_sort_long_long, (void *)&arg[i]))
                {
                    Warnning();
                    return;
                }
            for(int i = 0; i < pthread_num; i++)
                pthread_join(pth_id[i], NULL);
            for(int s = 1; s < all_s; s<<=1)
                for(int i = 0; i+s < pthread_num;i += s)
                {
                    merge_long_long(arg[i].head, arg[i].size, arg[i+s].head, arg[i+s].size, width, compar, temp_ary);
                    arg[i].size += arg[i+s].size;
                }
            break;
        default:
            for(int i = 0; i < pthread_num; i++)
                if(pthread_create(pth_id + i, NULL, (void *)merge_sort_normal, (void *)&arg[i]))
                {
                    Warnning();
                    return;
                }
            for(int i = 0; i < pthread_num; i++)
                pthread_join(pth_id[i], NULL);
            for(int s = 1; s < all_s; s<<=1)
                for(int i = 0; i+s < pthread_num;i += s)
                {
                    merge_normal(arg[i].head, arg[i].size, arg[i+s].head, arg[i+s].size, width, compar, temp_ary);
                    arg[i].size += arg[i+s].size;
                }
            break;
    }

    free(Array);
}

int comp(const void *x,const void *y)
{
    return (int)(*(int *)x) - (int)(*(int *)y); 
}
long long a[2000020];
void check(int l,int r)
{
    for(int i=l+1;i<=r;i++)
        if(comp(a+i,a+i-1)<0)
        {
            printf("error on %d\n",i);
            printf("pre=%d next=%d\n",a[i-1],a[i]);
            return ;
        }
    puts("YES");
}
int main()
{
    int n=2000000;
    // scanf("%d",&n);
    for(int i = 1;i <= n;i++)
    {
        a[i] = i&1 ? n-i: i;
    }
    my_sort(a+1, n, sizeof(long long), comp);
    for(int i=1;i<=n;i++)
        printf("%lld ",a[i]);
    puts("");
    return 0;
}
