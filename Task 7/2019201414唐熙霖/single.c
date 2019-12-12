// Scapegoat_tree.h

#include <stdio.h>
#define NUM_OF_SON 2
#define MAX_STK 100100

const int inf = 0X3f3f3f3f;
const double alpha = 0.75;

typedef double db;

typedef struct node
{
	int val, sz;
	struct node *fa, *son[NUM_OF_SON];
}node;

typedef struct scapegoat_tree
{
	int tot;
	node *root;
}spg;

typedef struct stack_for_spg
{
	int tot;
	node *el[MAX_STK];
}spgstk;

spgstk *newstk();

node *new_node();
spg *initial();
void termination(spg *, spgstk *);

int balance(node *);
void seek(spgstk *, node *);
node *build(spgstk *stk, int, int);
void rebuild(spg *, spgstk *, node *);
void insert(spg *, spgstk *, int);
void del(spg *, node *);
node *get_pos(spg *, int);

int query_rank(spg *, int);
int query_kth(spg *, int);
int query_pre(spg *, int);
int query_sub(spg *, int);

// Scapegoat_tree.c

#include <stdlib.h>

spgstk *new_stk()
{
	spgstk *res = (spgstk *)malloc(sizeof(spgstk));
	res->tot = 0;
	return res;
}

node *new_node()
{
	node *res = (node *)malloc(sizeof(node));
	res->val = res->sz = 0;
	res->fa = res->son[0] = res->son[1] = NULL;
	return res;
}

spg *initial()
{
	spg *tree = (spg *)malloc(sizeof(spg));
	tree->tot = 2, tree->root = new_node();

	node *root = tree->root;
	root->val = -inf, root->sz = 2, root->son[1] = new_node();

	node *boundary = root->son[1];
	boundary->val = inf, boundary->sz = 1, boundary->fa = root;

	return tree;
}

void termination(spg *tree, spgstk *stk)
{
	free(stk);
	while (tree->root != NULL)
		del(tree, tree->root);
	free(tree);
}

int balance(node *k)
{
	return (k->son[0] == NULL \
	|| alpha * k->sz >= (db)k->son[0]->sz) \
	&& (k->son[1] == NULL \
	|| alpha * k->sz >= (db)k->son[1]->sz);
}

void seek(spgstk *stk, node *k)
{
	if (k == NULL)
		return;
	seek(stk, k->son[0]);
	stk->el[(++stk->tot)] = k;
	seek(stk, k->son[1]);
}

node *build(spgstk *stk, int l, int r)
{
	if (l > r)
		return NULL;

	int mid = (l + r) >> 1;
	node *now = stk->el[mid];
	now->son[0] = build(stk, l, mid - 1);
	now->son[1] = build(stk, mid + 1, r);

	now->sz = 1;
	if (now->son[0] != NULL)
		now->son[0]->fa = now,
		now->sz += now->son[0]->sz;
	if (now->son[1] != NULL)
		now->son[1]->fa = now,
		now->sz += now->son[1]->sz;

	return now;
}

void rebuild(spg *tree, spgstk *stk, node *k)
{
	stk->tot = 0;
	seek(stk, k);

	node *fa = k->fa, *now = build(stk, 1, stk->tot);

	now->fa = fa;
	if(fa != NULL)
		fa->son[fa->son[1] == k] = now;
	if (tree->root == k)
		tree->root = now;
}

void insert(spg *tree, spgstk *stk, int x)
{
	int son;
	node *now = tree->root, *new_nd = new_node(), *inv = NULL;
	tree->tot++, new_nd->val = x, new_nd->sz = 1;

	while (1)
	{
		now->sz++;
		son = (x >= now->val);

		if (now->son[son] == NULL)
		{
			new_nd->fa = now, now->son[son] = new_nd;
			break;
		}
		else
			now = now->son[son];
	}
	while (new_nd != NULL)
	{
		if (!balance(new_nd))
			inv = new_nd;
		new_nd = new_nd->fa;
	}
	if(inv != NULL)
		rebuild(tree, stk, inv);
}

void del(spg *tree, node *k)
{
	if (k->son[0] != NULL && k->son[1] != NULL)
	{
		node *aim = k->son[0];
		while (aim->son[1] != NULL)
			aim = aim->son[1];
		k->val = aim->val;
		k = aim;
	}

	node *fa = k->fa, *goat = k->son[0] == NULL ? k->son[1] : k->son[0];
	if (fa != NULL)
		fa->son[k == fa->son[1]] = goat;
	if (goat != NULL)
		goat->fa = fa;
	if (tree->root == k)
		tree->root = goat;
	
	while (fa != NULL)
		fa->sz--, fa = fa->fa;
	free(k);
}

inline node *get_pos(spg *tree, int x)
{
	node *now = tree->root;
	while (now->val != x)
		x < now->val ? now = now->son[0] : (now = now->son[1]);
	return now;
}

inline int query_rank(spg *tree, int x)
{
	int res = 0;
	node *now = tree->root;
	while (now != NULL)
		now->val < x ? \
		(res += (now->son[0] == NULL ? 1 : now->son[0]->sz + 1), \
		now = now->son[1])\
		: (now = now->son[0]);
	return res;
}

inline int query_kth(spg *tree, int k)
{
	node *now = tree->root;
	while (1)
	{
		if ((now->son[0] != NULL ? now->son[0]->sz + 1 : 1) == k)
			return now->val;
		(now->son[0] != NULL && now->son[0]->sz >= k) \
		? now = now->son[0] 
		: (k -= (now->son[0] == NULL ? 1 : now->son[0]->sz + 1), \
		now = now->son[1]);
	}
}

inline int query_pre(spg *tree, int x)
{
	int res;
	node *now = tree->root;
	while (now != NULL)
		now->val < x 
		? (res = now->val, now = now->son[1]) 
		: (now = now->son[0]);
	return res;
}

inline int query_sub(spg *tree, int x)
{
	int res;
	node *now = tree->root;
	while (now != NULL)
		now->val > x 
		? (res = now->val, now = now->son[0]) 
		: (now = now->son[1]);
	return res;
}
// main.c

#include <stdio.h>
 
inline int read()
{
	int x = 0, f = 1;
	char ch = getchar();
	while (ch < '0' || ch > '9')
	{
		if (ch == '-')
			f = -1;
		ch = getchar();
	}
	while (ch <= '9' && ch >= '0')
	{
		x = (x<<1) + (x<<3) + ch - '0';
		ch = getchar();
	}
	return x * f;
}

void print(int x)
{
	if (abs(x) == inf)
		return (void)(putchar('-'), putchar('1'));
	if (x < 0)
		putchar('-'), x = -x;
	if (x >= 10)
		print(x / 10);
	putchar(x % 10 + '0');
}

int main()
{
	int n, opt, x;
	spg *tree = initial();
	spgstk *stk = new_stk();

	scanf("%d", &n);
	while (n--)
	{
		scanf("%d%d", &opt, &x);
		switch (opt)
		{
		case 0:
			insert(tree, stk, x);
			break;
		case 1:
			del(tree, get_pos(tree, x));
			break;
		case 2:
			print(query_kth(tree, x + 1));
			puts("");
			break;
		case 3:
			print(query_rank(tree, x) - 1);
			puts("");
			break;
		case 4:
			print(query_pre(tree, x));
			puts("");
			break;
		case 5:
			print(query_sub(tree, x));
			puts("");
			break;
		}
	}
	termination(tree, stk);
	return 0;
}
