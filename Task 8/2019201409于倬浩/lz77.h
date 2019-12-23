#ifndef _MY_LZ77_H_
#define _MY_LZ77_H_
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
// #define WINDOW_SIZE 127
// #define BUFFER_SIZE 127
#define FULL_BYTE 8
#define LENTH_BITS 4
#define DISTANCE_BITS 12
#define WINDOW_SIZE 4095
#define BUFFER_SIZE 255
#define LENTH_MAX ((1 << LENTH_BITS) - 1)
// #define BYTE_MASK 0xff
// #define LENTH_MASK 0xff
// #define DISTANCE_SHIFT 8
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
uint8_t *compress(uint8_t *src, size_t nmemb, size_t *);
uint8_t *decompress(uint8_t *src, size_t nmemb, size_t original_size, size_t* size);
uint8_t* readCompressed(char *filename, size_t *array_size, size_t *decompressed_size);
uint8_t* readDecompressed(char *filename, size_t *filesize);
void writeCompressed(char *filename, uint8_t *src, size_t decompressed_size, size_t array_size);
void writeDecompressed(char *filename, uint8_t *src, size_t array_size);

#endif