#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"struct.h"

extern LN* build(int now);

extern LN* goto_node_wise(LN* node, int gap);
extern LN* goto_node_anti(LN* node, int gap);
extern void del_node(LN *now);
extern void print_node(LN *now);

extern unsigned char check_cmd(int argv, char* arg[]);
extern void cmd_help(void);
extern unsigned char check_digit(char* arg);
extern unsigned char check_direct(char* arg);


static void solve(int len_of_loop, unsigned char anti, int len_of_gap);

