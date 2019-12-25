#include <stdio.h>
#include <string.h>
#include "everything.h"

extern void work(int n, int m, int type);

int main(int argc, char **argv) {
    int n, m, type = 0, cnt = 1;
    if(argc != 4 && argc != 3) return printf("Syntax Error\n"), 0;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    if(argc == 4) {
        if(strcmp(argv[3], "-\0") == 0) type = 1;
        else if(strcmp(argv[3], "+\0") == 0) type = 0;
        else return printf("Syntax Error\n"), 0;
    }
    work(n, m + 1, type);
    return 0;
}