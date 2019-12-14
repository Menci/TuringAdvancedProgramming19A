#include <stdbool.h>

#define INSERT 0
#define DELETE 1
#define FIND_KTH 2
#define FIND_TOTAL_MIN 3
#define FIND_MIN 4
#define FIND_MAX 5

typedef struct Node {
    int value;
    int size;
    int cnt;
    struct Node *pre;
    struct Node *left;
    struct Node *right;
} node;

node *Head;

void splay(node *operant);
void rotate(node *operant);

int min(int operant_1, int operant_2);
int max(int operant_1, int operant_2);

int count(node *operant);
void maintain(node *operant);
bool isroot(node *operant);
bool left_node(node *operant);

void Insert(int operant, node *current, node *father);
void delete(int operant, node *current);
int find_kth(int operant, node *current);
int find_total_min(int operant,node *current);
int find_min(int operant, node *current);
int find_max(int operant, node *current);

