#include "zip.h"
#include "tree.h"
#include <sys/stat.h>
#include <malloc.h>

static int FileSize(char *filename) {
    struct stat fileinfo;
    stat(filename, &fileinfo);
    return fileinfo.st_size;
}

void Zip(char *filename, FILE *input, FILE *output) {
    int size = FileSize(filename);
    int n = size / sizeof(node);

    node *tree = (node *)malloc(sizeof(node) * n);
    fread(tree, sizeof(node), n, input);
    fwrite(&n, sizeof(int), 1, output);

    DataDFS(0, tree, output);

    int now = 8;
    char info = 0;
    ChDFS(0, tree, output, &info, &now);
    if (now != 8) fwrite(&info, 1, 1, output);

    free(tree);
}

void Unzip(FILE *input, FILE *output) {
    int n;
    fread(&n, sizeof(int), 1, input);

    node *tree = (node *)malloc(sizeof(node) * n);
    char *info = (char *)malloc(sizeof(char) * n);

    fseek(input, sizeof(int) + n * K, SEEK_SET);
    for (int i = 0; i < n; i += 4) {
        char ch;
        fread(&ch, 1, 1, input);
        info[i] = (ch >> 6) & 3;
        info[i + 1] = (ch >> 4) & 3;
        info[i + 2] = (ch >> 2) & 3;
        info[i + 3] = ch & 3;
    }

    int tot = 0;
    BuildTree(&tot, tree, info, input);
    LDR(0, tree, output);

    free(tree);
}