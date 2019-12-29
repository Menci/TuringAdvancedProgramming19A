#include"struct.h"
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>

#define lc son[0]
#define rc son[1]

static void update(tp_node *tmp);
static tp_node *new_node();
static void free_node(tp_node* tmp);
static void rotate(tp_node **tmp, int k);
static void insert(tp_node **tmp,const void *key, cmp_pointer comp, int width);
static bool del(tp_node **tmp, const void *key, cmp_pointer comp);
static void* find_key(tp_node *tmp, int rnk, cmp_pointer comp);
static int find_rnk(tp_node *tmp, const void *key, cmp_pointer comp);
static void* find(tp_node *tmp, const void* key, cmp_pointer comp);
static void* find_nxt(tp_node *tmp, const void* key, cmp_pointer comp);
static void free_tree(tp_node* tmp);

Treap* Treap_build(cmp_pointer comp, int width);
void Treap_insert(Treap *tmp, void* key);
bool Treap_delete(Treap *tmp, void* key);
void* Treap_find_data(Treap *tmp, int rnk);
int Treap_find_rnk(Treap *tmp, void* key);
void* Treap_find(Treap *tmp, void* key);
void* Treap_find_nxt(Treap *tmp, void* key);
void* Treap_free(Treap *tmp);
