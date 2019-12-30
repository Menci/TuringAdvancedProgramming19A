#ifndef _METAKAR1_TREE
#define _METAKAR1_TREE

#include <stdio.h>

#define K 8

typedef struct node {
    int lch, rch;
    char ch[K];
} node;

void LDR(int x, node *tree, FILE *output);
void DataDFS(int x, node *tree, FILE *output);
void ChDFS(int x, node *tree, FILE *output, char *info, int *now);
void BuildTree(int *tot, node *tree, char *info, FILE *input);

#endif