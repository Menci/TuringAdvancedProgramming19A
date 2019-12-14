#include "persistent_treap.h"
extern node *null, *root;
node *kthElement(int k) {
	node *rt = root;
	int lsize;
	while (rt != null) {
		lsize = rt->l->sz + 1;
		if (lsize == k) return rt;
		if (k > lsize)
			k -= lsize, rt = rt->r;
		else
			rt = rt->l;
	}
	return rt;
}
int rankElement(int x) {
	int ret = 0;
	node *rt = root;
	while (rt != null) {
		if (x > rt->v)
			ret += rt->l->sz + 1, rt = rt->r;
		else
			rt = rt->l;
	}
	return ret;
}
void insert(int x) {
	pii t = split(root, rankElement(x));
	root = merge(t.a, merge(newNode(x), t.b));
}
void erase(int x) {
	pii t1 = split(root, rankElement(x)), t2 = split(t1.b, 1);
	root = merge(t1.a, t2.b);
	deleteNode(t2.a);
}
int precessor(int x) {
	int rnk = rankElement(x);
	if (rnk == 0) return -1;
	node *t = kthElement(rnk);
	return t->v;
}
int successor(int x) {
	int rnk = rankElement(x + 1);
	if (rnk == root->sz) return -1;
	node *t = kthElement(rnk + 1);
	return t->v;
}
void clearTree(node *x) {
	if (x == null) return;
	clearTree(x->l);
	clearTree(x->r);
	free(x);
}
void cleanup() {
	clearTree(root);
	free(null);
}