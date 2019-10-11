#include <stdio.h>
#include <stdint.h>
#include "double.h"

uint64_t checker(uint64_t a, uint64_t b, char op) {
    double_t x, y, ans;
    x = *(double_t*)(&a);
    y = *(double_t*)(&b);

    if(op == '+') ans = __add__(x, y);
    else if(op == '-') ans = __min__(x, y);
    else if(op == '*') ans = __mul__(x, y);
    else if(op == '/') ans = __div__(x, y);
    
    return *(uint64_t*)(&ans);
}

#ifdef __LOCAL_DEBUG__

void print_detail(pointer_t start, unsigned size) {
    printf("details: ");
    for(unsigned i = 0; i < size; i++)
        printf("%2X ", start[i]);
    putchar('\n');
}

int main() {
    printf("0x%lx", checker(0x3fe23b36c9c91893ul, 0x40043cbb4af00741ul, '/'));
    return 0;
}

#endif
