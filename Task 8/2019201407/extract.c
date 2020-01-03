#include "everything.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static char *data;
static char *data_son;
static char *data_val;
static int n;
static int K;

#define son(u) ((data_son[(u) / 4] & (3 << (((u) & 3) * 2))) >> (((u) & 3) * 2))
#define val(u) (data_val + (u) * K)

int hash(char *str, int len) {
    long long res = 0;
    for(int i = 0; i < len; i++) res *= 256, res += str[i], res %= mod;
    return res;
}

int extract_dfs(int *id) {
    (*id)++;
    long long val = hash(val(*id), K);
    //printf("    %lld\n", val);
    char flag = son(*id);
    if(flag & 1) {
        val *= big_num[flag];
        val += extract_dfs(id);
        val %= mod;
    }
    if(flag & 2) {
        val *= big_num[flag];
        val += extract_dfs(id);
        val %= mod;   
    }
    return val;
}

int extract_tree(char *file_name, int *hash_val) {
    FILE *fp = fopen(file_name, "rb");
    if(fp == NULL)
        return _ERR_OPEN_FAILED;

    struct stat stbuf;
    stat(file_name, &stbuf);
    data = malloc(stbuf.st_size);
    if(data == NULL)
        return _ERR_MALLOC_FAILED;
    
    data = data;
    fread(data, 1, stbuf.st_size, fp);
    n = *(int*) data;
    data_son = data + 4;
    data_val = data_son + upper_div(n, 4);
    K = (stbuf.st_size - upper_div(n, 4) - 4) / n;
    if((stbuf.st_size - upper_div(n, 4) - 4) % n != 0) return _ERR_DATASIZE;
    
    int tmp = -1;
    *hash_val = extract_dfs(&tmp);
    fclose(fp);

    free(data);
    return _SUCC;
}