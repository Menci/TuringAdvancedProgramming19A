#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <sys/stat.h>
#include "tree.h"

int main(int argc, char *argv[]) {
    assert(argc == 3);

    FILE *input = fopen(argv[1], "r");
    assert(input);
    FILE *output = fopen(argv[2], "w");
    assert(output);

    struct stat fileinfo;
    stat(argv[1], &fileinfo);
    int size = fileinfo.st_size;
    int n = size / sizeof(node);

    node *tree = (node *)malloc(sizeof(node) * n);
    fread(tree, sizeof(node), n, input);

    LDR(0, tree, output);
    free(tree);
    return 0;
}