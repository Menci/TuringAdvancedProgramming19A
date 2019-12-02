#include<stdio.h>
#include<string.h>

#define ARG_NUM 4
#define LEN_OF_SECOND 2
#define FALSE 0
#define TRUE 1

void cmd_help(void);
unsigned char check_digit(char* arg);
unsigned char check_direct(char* arg);
unsigned char check_cmd(int argv, char* arg[]);