#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int read() {
	int X = 0, w = 0; char ch = 0;
	while (!isdigit(ch)) { w |= ch == '-'; ch = getchar(); }
	while (isdigit(ch)) X = (X << 3) + (X << 1) + (ch ^ 48), ch = getchar();
	return w ? -X : X;
}

typedef struct balanced_tree{
	int key, cnt, siz;
	struct balanced_tree *fa, *l, *r;
} balanced_tree;
typedef balanced_tree* node;

node root = NULL;

int get_l_or_r(node x) { return x -> fa -> r == x; }
void update(node x) {
	if(x != NULL) {
		x -> siz = x -> cnt;
		if (x -> l != NULL)
			x -> siz += x -> l -> siz;
		if (x -> r != NULL)
			x -> siz += x -> r -> siz;
	}
}
void rotate(node x) {
	node y = x -> fa, z = y -> fa;
	if(x -> fa -> l == x) {
		y -> l = x -> r;
		if(y -> l != NULL) y -> l -> fa = y;
		x -> r = y;
	}
	else {
		y -> r = x -> l;
		if(y -> r != NULL) y -> r -> fa = y;
		x -> l = y;
	}
	y -> fa = x; x -> fa = z;
	if (z != NULL) {
		if(z -> l == y) z -> l = x;
		else z -> r = x;
	}
	update(y); update(x);
}
void splay(node x) {
	node f = x -> fa;
	while (f != NULL) {
		if (f -> fa != NULL)
			rotate(get_l_or_r(f) == get_l_or_r(x) ? f : x);
		rotate(x);
		f = x -> fa;
	}
	root = x;
}
void ins_node(int v) {
	if (root == NULL) {
		root = (node)malloc(sizeof(balanced_tree));
		root -> l = root -> r = root -> fa = NULL;
		root -> cnt = root -> siz = 1;
		root -> key = v;
		return;
	}
	node now = root, f = NULL;
	while (1) {
		if (now -> key == v) {
			++ (now -> cnt);
			update(now); update(f);
			splay(now);
			return;
		}
		f = now;
		if(now -> key < v) now = now -> r;
		else now = now -> l;
		if (now == NULL) {
			node x = (node)malloc(sizeof(balanced_tree));
			x -> l = x -> r = NULL;
			x -> fa = f;
			x -> cnt = x -> siz = 1;
			x -> key = v;
			if(f -> key < v) f -> r = x;
	   		else f -> l = x;
			update(f);
			splay(x);
			return;
		}
	}
}
int find_kth(int x) {
	node now = root;
	while (1) {
		while (now -> l != NULL && now -> l -> siz >= x) now = now -> l;
		int tmp = (now -> l == NULL ? 0 : now -> l -> siz) + now -> cnt;
		if (x <= tmp)
			return now -> key;
		x -= tmp;
		now = now -> r;
	}
}
int find_rank(int v) {
	node now = root;
	int ans = 0;
	while (1) {
		while (now -> key > v) now = now -> l;
		ans += now -> l == NULL ? 0 : now -> l -> siz;
		if (now -> key == v) {
			splay(now);
			return ans + 1;
		}
		ans += now -> cnt;
		now = now -> r;
	}
}
node pre() {
	node now = root -> l;
	while (now != NULL && now -> r != NULL) now = now -> r;
	return now;
}
node nxt() {
	node now = root -> r;
	while (now != NULL && now -> l != NULL) now = now -> l;
	return now;
}
void del_node(int x) {
	find_rank(x);
	if (root -> cnt > 1) {
		--(root -> cnt);
		return;
	}
	if (root -> l == NULL && root -> r == NULL) {
		root = NULL;
		return;
	}
	if (root -> l == NULL) {
		node oldroot = root;
		root = root -> r;
		root -> fa = NULL;
		free(oldroot);
		return;
	}
	if (root -> r == NULL) {
		node oldroot = root;
		root = root -> l;
		root -> fa = NULL;
		free(oldroot);
		return;
	}
	node oldroot = root;
	splay(pre());
	root -> r = oldroot -> r;
	oldroot -> r -> fa = root;
	free(oldroot);
	update(root);
}
int smaller_than_x(int x) {
	ins_node(x);
	int ans = find_rank(x) - 1;
	del_node(x);
	return ans;
}
int max_num_smaller_than_x(int k) {
	ins_node(k);
	node x = pre();
	int ans = x == NULL ? -1 : x -> key;
	del_node(k);
	return ans;
}
int min_num_bigger_than_x(int k) {
	ins_node(k);
	node x = nxt();
	int ans = x == NULL ? -1 : x -> key;
	del_node(k);
	return ans;
}
void clear_tree(node x) {
	if(x -> l != NULL) clear_tree(x -> l);
	if(x -> r != NULL) clear_tree(x -> r);
	free(x);
}
void del_balanced_tree() {
	clear_tree(root);
}
int main() {
	int n = read();
	while (n--) {
		int op = read(), k = read();
		if (op == 0) ins_node(k);
		if (op == 1) del_node(k);
		if (op == 2) printf("%d\n", find_kth(k));
		if (op == 3) printf("%d\n", smaller_than_x(k));
		if (op == 4) printf("%d\n", max_num_smaller_than_x(k));
		if (op == 5) printf("%d\n", min_num_bigger_than_x(k));
	}
	del_balanced_tree();
	return 0;
}
