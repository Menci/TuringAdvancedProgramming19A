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

const int divide_line = 8;
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

void char_swap(char *a, char *b, int width)
{
    char_tmp = *a;
    *a = *b;
    *b = char_tmp;
}

void int_swap(int *a, int *b, int width)
{
    int_tmp = *a;
    *a = *b;
    *b = int_tmp;
}

void long_long_swap(long long *a, long long *b, int width)
{
    long_long_tmp = *a;
    *a = *b;
    *b = long_long_tmp;
}

void selection_sort(char *head,char *tail,size_t width, int (*compar)(const void *, const void *),void (*swap)(char *, char *,int))
{
    char *max, *p;
    while(head < tail)
    {
        max = head;

        for(p = head + width;p <= tail;p += width)
            if(compar(p,max)>0)
                max = p;

        swap(max, tail, width);
        tail -= width;
    }
}

static int (*comp_tmp)(const void *, const void *);

int pointer_comp(const void *a, const void *b)
{
    return comp_tmp(*(void **)a,*(void **)b);
}

void big_struct_sort(void *base, size_t nmemb, size_t width, int (*compar)(const void *, const void *));

void my_sort(void *base, size_t nmemb, size_t width, int (*compar)(const void *, const void *))
{
    char *head_stk[max_len],*tail_stk[max_len];

    TEMP = malloc(width);

    void (*swap)(char *, char *,int);
    if(width == char_bytes)
        swap = char_swap;
    else if(width == int_bytes)
        swap = int_swap;
    else if(width == long_long_bytes)
        swap = long_long_swap;
    else
        swap = memcpy_swap;

    char *head, *tail;
    char *fst, *lst;
    char *mid;
    int stk_pt = 0, size;

    head_stk[++stk_pt] = (char *)base;
    tail_stk[stk_pt] = (char *)base + (nmemb-1) * width;
    
    head = head_stk[stk_pt];
    tail = tail_stk[stk_pt]; 
    unsigned char is_sorted = 1, is_reverse = 1;
    int tmp;
    while(head!=tail)
    {
        tmp = compar(head,head + width);
        is_sorted &= (tmp<=0);
        is_reverse &= (tmp>=0);
        head += width;
    }
    if(is_sorted)
    {
        free(TEMP);
        return;
    }
    if(is_reverse)
    {
        head = head_stk[stk_pt];
        tail = tail_stk[stk_pt];
        while(head<tail)
        {
            swap(head, tail, width);
            head += width;
            tail -= width;
        }
        free(TEMP);
        return;
    }

    // if(width != big_struct_line)
    // {
    //     free(TEMP);
    //     big_struct_sort(base, nmemb, width, compar);
    //     return;
    // }

    while(stk_pt)
    {
        head = head_stk[stk_pt];
        tail = tail_stk[stk_pt];
        --stk_pt;

        if(head >= tail)
            continue;

        size = (tail - head) / width;
        if(size <= divide_line)
            selection_sort(head, tail, width, compar, swap);
        else
        {
            mid = head + (size>>1)*width;

            if(compar(head, mid)> 0)
                swap(head, mid, width);
            if(compar(head, tail)> 0)
                swap(head, tail, width);
            if(compar(mid, tail)> 0)
                swap(mid, tail, width);
            // to move the base element at the mid place             

            fst = head;
            lst = tail;

            //head becomes a empty space for the base element
            while(fst < lst)
            {
                fst += width;
                lst -= width;
                while(fst <= tail && compar(fst, mid) <= 0)
                    fst += width;
                while(mid < lst && compar(lst, mid) > 0)
                    lst -= width;
                if(fst > lst)
                    break;
                swap(fst , lst, width);
                if(mid == lst)
                    mid = fst;
            }
            // swap(lst, head, width);
            
            fst -= width;
            if(width < big_struct_line)
                while(fst >= head && !compar(fst, mid))
                    fst -= width;
            ++stk_pt;
            head_stk[stk_pt] = head;
            tail_stk[stk_pt] = fst;
            
            lst += width;
            if(width < big_struct_line)
                while(lst <= tail && !compar(lst, mid))
                    lst += width;          
            ++stk_pt;
            tail_stk[stk_pt] = tail;
            head_stk[stk_pt] = lst;
        }
        
    }
    free(TEMP);
    return ;
}

int comp(const void *x,const void *y)
{
    return (int)(*(int *)x) - (int)(*(int *)y); 
}
int a[10000100];
int main()
{
    int n=20;
    // scanf("%d",&n);
    for(int i = 1;i <= n;i++)
    {
        a[i] = i&1 ? i : n-i;
    }
    big_struct_sort(a+1, n, sizeof(int), comp);
    for(int i=1;i<=n;i++)
        printf("%d ",a[i]);
    puts("");
    return 0;
}

void big_struct_sort(void *base, size_t nmemb, size_t width, int (*compar)(const void *, const void *))
{
    comp_tmp = compar;
    long long *pointer = malloc(nmemb * sizeof(base) );
    long long *head, *tail;
    char *pt = base;
    head = pointer;
    tail = pointer + (nmemb - 1) * sizeof(base);
    while(head < tail)
    {
        *head = (long long)pt;
        ++head;
        pt += width;
    }

    my_sort(pointer, nmemb, sizeof(base), pointer_comp);

    head = pointer;
    while(head < tail)
    {
        *head -= (long long)*pointer;
        *head >>= 3;
        ++head;
    }
    head = pointer;
    char *vis = malloc(nmemb);
    memset(vis, 0, nmemb);
    int x, i = 0;
    void *pre;
    while(head < tail)
    {
        x = head + (i<<3);
        if(x == i)
            continue;
        vis[i] = 1;
        memcpy(TEMP, pre = (base + i*width), width);
        while(vis[x])
        {
            vis[x] = 1;
            memcpy(pre, base + x * width, width);
            pre = base + x * width;
            x = pointer[x];
        }
        memcpy(pre, TEMP, width);
    }
    free(vis);
    free(pointer);
}