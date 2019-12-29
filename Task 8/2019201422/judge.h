#include"struct.h"

static char cnt_son[N];

static char rand_char();
static int rand_int(int n);
static void rand_tree(int n);

bool is_node(int i);
void print(int i);
bool check();

extern void zip_tree();
extern void unzip_tree();
extern void compress();
extern void discompress();
