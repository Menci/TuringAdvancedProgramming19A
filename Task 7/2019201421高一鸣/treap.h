#define sizeTreap (sizeof(struct Treap))
typedef struct Treap Treap;
typedef struct Droot Droot;
struct Treap {
  struct Treap *l, *r;
  int fix, key, size;
  void (*updata)(struct Treap *cur);
  int (*Size)(struct Treap *cur);
  Treap *(*merge)(Treap *A, Treap *B);
} * root;
struct Droot {
  Treap *first, *second;
};
Treap *newTreap(int key);
Treap *Merge(Treap *A, Treap *B);
Droot Split(Treap *x, int k);
int getKth(int k, Treap *pos);
int getRank(Treap *x, int v);
void Insert(int v);
void Delete(int k);
int findPre(int v, Treap *pos);
int findNxt(int v, Treap *pos);
void clearTreap(Treap *cur);