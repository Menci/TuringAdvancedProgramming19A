#include "everything.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int hash_val;
    int K = 0;
    int err_flag = _SUCC;
    char default_input[] = "input.bin";
    char default_output[] = "zipped.bin";
    char *input_file = default_input;
    char *output_file = default_output;
    switch(argc) {
    case 4:
        if(strcmp(argv[1], "-z") == 0) input_file = argv[3];
        if(strcmp(argv[1], "-e") == 0) output_file = argv[3];
    case 3:
        K = atoi(argv[2]);
    case 2:
        break;
    default:
        err_flag = _ERR_SYNTAX;
        break;
    }
    if(err_flag == _SUCC) {
        if(strcmp(argv[1], "-z") == 0) {
            if(K == 0) err_flag = _ERR_SYNTAX;
            else err_flag |= zip_tree(K, input_file, output_file, &hash_val);
        }
        if(strcmp(argv[1], "-e") == 0)
            err_flag |= extract_tree(output_file, &hash_val);
    }
    if(err_flag != 0) {
        if(err_flag & _ERR_SYNTAX)
            printf("Error : Syntax error.\n");
        else if(err_flag & _ERR_OPEN_FAILED)
            printf("Error : Open failed. Please check your R/W Authority.\n");
        else if(err_flag & _ERR_MALLOC_FAILED)
            printf("Error : Memory allocation failed. Please check your data scale.\n");
        else if(err_flag & _ERR_DATASIZE)
            printf("Error : Wrong data size (K). Please check your input.\n");

        printf("Error : err_id = 0x%x\n", err_flag);
    }
    else {
        if(strcmp(argv[1], "-z") == 0) {
            printf("zipped successfully.\n");
            struct stat stbuf;
            stat(input_file, &stbuf);
            long original_size = stbuf.st_size;
            stat(output_file, &stbuf);
            long zipped_size = stbuf.st_size;
            printf("original_size = %ld, zipped_size = %ld\nzip rate = %lf\n", 
                original_size, zipped_size, 1.0 * zipped_size / original_size);
        }
        if(strcmp(argv[1], "-e") == 0)
            printf("extracted successfully.\n");
        printf("hash_id = 0x%x\n", hash_val);
    }
    return 0;
}