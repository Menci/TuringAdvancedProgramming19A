#include "treap.h"

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#define malloc myMalloc
#define free myFree
void *newNode(size_t size) { return malloc(size); }
void deleteNode(void *cur) { free(cur); }
void clearTreap(Treap *cur) {
    if (cur->l) clearTreap(cur->l);
    if (cur->r) clearTreap(cur->r);
    deleteNode(cur);
}
void check() {
    if (cntnode) fprintf(stderr, "Memory have leaked");
}
void updata(Treap *cur) {
    cur->size = 1 + (cur->l ? cur->l->size : 0) + (cur->r ? cur->r->size : 0);
}
int Size(struct Treap *x) { return x ? x->size : 0; }

Treap *Merge(Treap *A, Treap *B) {  //合并操作
    if (!A) return B;
    if (!B) return A;
    if (A->fix < B->fix) {
        A->r = Merge(A->r, B);
        A->updata(A);
        return A;
    } else {
        B->l = Merge(A, B->l);
        B->updata(B);
        return B;
    }
}
Droot Split(Treap *x, int k) {  //拆分操作
    Droot y;
    if (!x) return y.first = y.second = NULL, y;
    if (x->Size(x->l) >= k) {
        y = Split(x->l, k);
        x->l = y.second;
        x->updata(x);
        y.second = x;
    } else {
        y = Split(x->r, k - x->Size(x->l) - 1);
        x->r = y.first;
        x->updata(x);
        y.first = x;
    }
    return y;
}
int getKth(int k, Treap *pos) {  //查找第K小
    Droot x = Split(pos, k - 1);
    Droot y = Split(x.second, 1);
    Treap *ans = y.first;
    pos = Merge(Merge(x.first, ans), y.second);
    return ans->key;
}
int getRank(Treap *x, int v) {  //询问一个数是第几大
    if (!x) return 0;
    return v < x->key ? getRank(x->l, v) : getRank(x->r, v) + x->Size(x->l) + 1;
}
void Insert(int v) {  //插入操作
    int k = getRank(root, v);
    Droot x = Split(root, k);
    Treap *n = newTreap(v);
    root = Merge(Merge(x.first, n), x.second);
}
void Delete(int k) {  //删除操作
    Droot x = Split(root, k - 1);
    Droot y = Split(x.second, 1);
    deleteNode(y.first);
    root = Merge(x.first, y.second);
}
int findPre(int v, Treap *pos) {
    int k = getRank(pos, v);
    if (!k) return -1;
    int vv = getKth(k, pos);
    if (vv != v) return vv;
    if (k == 1) return -1;
    return getKth(k - 1, pos);
}
int findNxt(int v, Treap *pos) {
    int k = getRank(pos, v);
    Droot tmp = Split(pos, k);
    int ans = 0;
    if (tmp.second) {
        ans = getKth(1, tmp.second);
    } else
        ans = -1;
    pos = Merge(tmp.first, tmp.second);
    return ans;
}
Treap *newTreap(int key) {
    Treap *cur = newNode(sizeTreap);
    cur->fix = rand(), cur->key = key, cur->l = cur->r = NULL, cur->size = 1;
    cur->Size = Size, cur->updata = updata, cur->merge = Merge;
    return cur;
}
