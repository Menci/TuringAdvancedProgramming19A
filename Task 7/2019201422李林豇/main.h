#include<stdio.h>
#include<stdbool.h>
#include"struct.h"

extern Treap* Treap_build(cmp_pointer comp, int width);
extern void Treap_insert(Treap *tmp, void* key);
extern bool Treap_delete(Treap *tmp, void* key);
extern void* Treap_find_data(Treap *tmp, int rnk);
extern int Treap_find_rnk(Treap *tmp, void* key);
extern void* Treap_find_pre(Treap *tmp, void* key);
extern void* Treap_find_nxt(Treap *tmp, void* key);
extern void* Treap_free(Treap *tmp);
extern void openfile();
extern void check_memory();

static void solve(Treap* my_treap);
int comp(const void* a, const void *b);
