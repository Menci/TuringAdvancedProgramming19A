#include "lz77.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/stat.h>
static uint8_t *arr;
static int ptr, buff, buffcnt;

void add_bit(bool b) {
    ++buffcnt;
    buff |= b << (buffcnt - 1);
    if(buffcnt == FULL_BYTE) {
        arr[ptr++] = buff;
        buff = 0, buffcnt = 0;
    }
}

void add_byte(uint8_t b) {
    buff |= (b & ((1 << (FULL_BYTE - buffcnt)) - 1)) << buffcnt;
    arr[ptr++] = buff;
    buff = b >> (FULL_BYTE - buffcnt);
}

bool get_bit() {
    if(buffcnt == 0) {
        buff = arr[ptr++];
        buffcnt = FULL_BYTE; 
    }
    bool ret = (buff >> (FULL_BYTE - buffcnt)) & 1;
    --buffcnt;
    return ret;
}

uint8_t get_byte() {
    if(buffcnt == 0) return arr[ptr++];
    uint8_t ret = buff >> (FULL_BYTE - buffcnt);
    buff = arr[ptr++];
    ret |= (buff & ((1 << (FULL_BYTE - buffcnt)) - 1)) << buffcnt;
    return ret;
}

uint8_t *compress(uint8_t *src, size_t nmemb, size_t *size) {
    uint8_t *ret = (uint8_t *) malloc(2 * nmemb);
    memset(ret, 0, nmemb);
    arr = ret;
    buffcnt = 0, ptr = 0;
    buff = 0;
    int window_start = -1;
    for(int i = 0, lenth; i < nmemb; i += lenth) {
        int maxlenth = 0, maxj = -1;
        int start = max(0, i - WINDOW_SIZE + 1), end = min(nmemb, i + BUFFER_SIZE);
        for(int j = start; j < i; ++j) {
            if(src[i] != src[j]) continue;
            int k = i;
            for(k; k < end && src[k] == src[j + k - i]; ++k);
            if(k - i > maxlenth) maxlenth = k - i, maxj = j;
        }
        if(maxlenth > LENTH_MAX) maxlenth = LENTH_MAX;
        if(maxlenth > 2) {
            add_bit(true);
            int dist = i - maxj;
            for(int t = 0; t < DISTANCE_BITS; ++t)
                add_bit((dist >> t) & 1);
            for(int t = 0; t < LENTH_BITS; ++t)
                add_bit((maxlenth >> t) & 1);
            lenth = maxlenth;
        }
        else {
            add_bit(false);
            add_byte(src[i]);
            lenth = 1;
        }
    }
    if(buffcnt) arr[ptr++] = buff;
    *size = ptr;
    return arr;
}
uint8_t *decompress(uint8_t *src, size_t nmemb, size_t original_size, size_t* size) {
    uint8_t *dst = calloc(original_size, 1);
    size_t cur = 0;
    ptr = 0, buffcnt = 0, arr = src;
    int operand;
    while(ptr < nmemb || (ptr == nmemb && buffcnt > 0)) {
        operand = get_bit();
        if(operand == 0) dst[cur++] = get_byte();
        else {
            int dist = 0, maxlenth = 0, begin = cur;
            for(int t = 0; t < DISTANCE_BITS; ++t)
                if(get_bit()) dist |= 1 << t;
            for(int t = 0; t < LENTH_BITS; ++t)
                if(get_bit()) maxlenth |= 1 << t;
            for(int j = 0; j < maxlenth; ++j) dst[cur++] = dst[begin - dist + j];
        }
    }
    *size = cur;
    return dst;
}

uint8_t* readCompressed(char *filename, size_t *array_size, size_t *decompressed_size) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return NULL;
    struct stat statbuf;
    stat(filename, &statbuf);
    *array_size = statbuf.st_size - sizeof(size_t);
    uint8_t* dest = malloc(*array_size);
    fread(decompressed_size, sizeof(size_t), 1, fp);
    fread(dest, 1, *array_size, fp);
    fclose(fp);
    return dest;
}

uint8_t* readDecompressed(char *filename, size_t *filesize) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return NULL;
    struct stat statbuf;
    stat(filename, &statbuf);
    *filesize = statbuf.st_size;
    uint8_t *dest = malloc(*filesize);
    fread(dest, 1, *filesize, fp);
    fclose(fp);
    return dest;
}

void writeCompressed(char *filename, uint8_t *src, size_t decompressed_size, size_t array_size) {
    FILE *fp = fopen(filename, "w");
    fwrite(&decompressed_size, sizeof(size_t), 1, fp);
    fwrite(src, 1, array_size, fp);
    fclose(fp);
}

void writeDecompressed(char *filename, uint8_t *src, size_t array_size) {
    FILE *fp = fopen(filename, "w");
    fwrite(src, 1, array_size, fp);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if(argc != 4 && argc != 5) {
        puts("Invalid Arguments!");
        return -1;
    }
    if(atoi(argv[1]) == 0) {
        printf("Compressing [%s] => [%s]\n", argv[2], argv[3]);
        int n;
        uint8_t *src = readDecompressed(argv[2], &n);
        size_t compressed_size;
        uint8_t *compressed = compress(src, n, &compressed_size);
        writeCompressed(argv[3], compressed, n, compressed_size);
        free(src), free(compressed);
        printf("Compresstion Ratio = %.3lf %% \n", 100.0 * compressed_size / n);
    }
    else if(atoi(argv[1]) == 1) {
        printf("Decompressing [%s] => [%s]\n", argv[2], argv[3]);
        int n;
        size_t array_size, decompressed_size, decompress_size_check;
        uint8_t *src = readCompressed(argv[2], &array_size, &decompressed_size);
        uint8_t *decompressed = decompress(src, array_size, decompressed_size, &decompress_size_check);
        printf("%d %d\n", decompressed_size, decompress_size_check);
        writeDecompressed(argv[3], decompressed, decompressed_size);
    }
    else if(atoi(argv[1]) == 2) {
        printf("Checking (Compressed)[%s] == (Decompressed)[%s] ?\n", argv[3], argv[2]);
        int n;
        size_t array_size, decompressed_size, decompress_size_check;
        uint8_t *src1 = readCompressed(argv[3], &array_size, &decompressed_size);
        uint8_t *decompressed = decompress(src1, array_size, decompressed_size, &decompress_size_check);
        int m;
        uint8_t *src2 = readDecompressed(argv[2], &m);
        assert(m == decompressed_size);
        for(int i = 0; i < m; ++i) assert(decompressed[i] == src2[i]);
        if(argc == 5) writeDecompressed(argv[4], decompressed, m);
        printf("Check Passed.\n");
        printf("Compresstion Ratio = %.3lf %% \n", 100.0 * array_size / decompressed_size);
    }
}