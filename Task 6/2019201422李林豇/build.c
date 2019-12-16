#include"build.h"

LN* build(int len_of_loop)
{
    LN *now, *fir, *last;
    int i = 1;

    fir = new_node();
    fir->data = 1;
    last = fir;

    while(i++<len_of_loop)
    {
        now = new_node();
        now->data = i;
        link_node(last, now);
        
        last = now;
    }

    link_node(last, fir);

    return fir;
}