#include "everything.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

static int n;
static int K;
static int size;
static char *data;
static char *res_node;
static char *res_val;

#define son(u, type) (*(int *)(data + (u) * size + (type) * 4))
#define val(u) (data + (u) * size + 8)

extern int hash(char *str, int len);

int zip_dfs(int u, int *id, char **res_1, char **res_2) {
    (*id)++;
    long long val = hash(val(u), K);
    //printf("    %lld\n", val);
    memcpy(*res_2, val(u), K), (*res_2) += K;
    char flag = 0;
    if(son(u, 0) >= 0) flag |= 1;
    if(son(u, 1) >= 0) flag |= 2;
    **res_1 |= flag << ((*id & 3) * 2);
    if((*id & 3) == 3) (*res_1) += 1;
    if(flag & 1) {
        val *= big_num[flag];
        val += zip_dfs(son(u, 0), id, res_1, res_2);
        val %= mod;
    }
    if(flag & 2) {
        val *= big_num[flag];
        val += zip_dfs(son(u, 1), id, res_1, res_2);
        val %= mod;
    }
    return val;
}

int zip_tree(int data_size, char *from_file_name, char *to_file_name, int *hash_val) {
    FILE *fp = fopen(from_file_name, "rb");
    if(fp == NULL)
        return _ERR_OPEN_FAILED;
    
    struct stat stbuf;
    stat(from_file_name, &stbuf);
    K = data_size;
    size = 4 * upper_div(K, 4) + 8;
    n = stbuf.st_size / size;
    if(stbuf.st_size % size != 0) return _ERR_DATASIZE;
    
    int Index = -1;
    int *in_deg = calloc(n, sizeof(int));
    res_node = calloc(upper_div(n, 4), sizeof(char));
    res_val = calloc(n, size - 8);
    data = malloc(n * size);
    if(res_node == NULL || res_val == NULL || in_deg == NULL)
        return _ERR_MALLOC_FAILED;
    
    fread(data, size, n, fp);
    for(int i = 0; i < n; i++) {
        if(son(i, 0) >= 0) in_deg[son(i, 0)]++;
        if(son(i, 1) >= 0) in_deg[son(i, 1)]++;
    }

    for(int i = 0; i < n; i++) {
        if(in_deg[i] == 0) {
            char *r_n = res_node, *r_v = res_val;
            *hash_val = zip_dfs(i, &Index, &r_n, &r_v);
        }
    }
    fclose(fp);

    fp = fopen(to_file_name, "wb");

    if(fp == NULL)
        return _ERR_OPEN_FAILED;

    fwrite(&n, sizeof(int), 1, fp);
    fwrite(res_node, sizeof(char), upper_div(n, 4), fp);
    fwrite(res_val, K, n, fp);
    fclose(fp);

    free(data);
    free(in_deg);
    free(res_node);
    free(res_val);
    return _SUCC;
}