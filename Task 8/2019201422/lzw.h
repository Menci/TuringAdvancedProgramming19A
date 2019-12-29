#include "struct.h"

static int cnt,P,Q;
static unsigned char W,V;
static Treap* my_treap[byte_max];
static unsigned char str_s[M];
// static trie_node tool;

static void Init();
static void Out(int x);
static void Search();
static void Research();
static int comp(const void *a,const void *b);

void Compress();
void Decompress();

extern Treap* Treap_build(cmp_pointer comp, int width);
extern void Treap_insert(Treap *tmp, void* key);
extern bool Treap_delete(Treap *tmp, void* key);
extern void* Treap_find_data(Treap *tmp, int rnk);
extern int Treap_find_rnk(Treap *tmp, void* key);
extern void* Treap_find(Treap *tmp, void* key);
extern void* Treap_find_nxt(Treap *tmp, void* key);
extern void* Treap_free(Treap *tmp);