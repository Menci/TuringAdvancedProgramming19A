#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zip.h"

int main(int argc, char *argv[]) {
    assert(argc == 4);
    FILE *input = fopen(argv[2], "r");
    assert(input);
    FILE *output = fopen(argv[3], "w");
    assert(output);

    if (!strcmp(argv[1], "zip")) {
        Zip(argv[2], input, output);
        fclose(input);
        fclose(output);
        return 0;
    }
    if (!strcmp(argv[1], "unzip")) {
        Unzip(input, output);
        fclose(input);
        fclose(output);
        return 0;
    }

    fprintf(stderr, "Error!\n");
    return 0;
}