#include "binary_search_tree.h"

node *rt = NULL;

node *newnode(int x)
{
	node *ret = (node*)malloc(sizeof(node));
	ret->ch[0] = ret->ch[1] = NULL;
	ret->cnt = ret->siz = 1;
	ret->fa = NULL;
	ret->x = x;
	return ret;
}

void update(node *x)
{
	x->siz = x->cnt;
	if(x->ch[0] != NULL)
		x->siz += x->ch[0]->siz;
	if(x->ch[1] != NULL)
		x->siz += x->ch[1]->siz;
}

void rotate(node *x)
{
	node *y = x->fa, *z = y->fa;
	int l = y->ch[1] == x;
	if(z != NULL)
		z->ch[z->ch[1] == y] = x;
	x->fa = z; y->fa = x;
	if(x->ch[l ^ 1] != NULL)	
		x->ch[l ^ 1]->fa = y;
	y->ch[l] = x->ch[l ^ 1];
	x->ch[l ^ 1] = y;
	update(y);update(x);
	if(z != NULL)update(z);
}

void splay(node *x)
{
	if(x->fa == NULL)return;
	node *y;
	for(y = x->fa; y != NULL; rotate(x), y = x->fa)
		if(y->fa != NULL)
		{
			if(y->ch[1] == x ^ y->fa->ch[1] == y)
				rotate(y);
			else
				rotate(x);
		}
	x->fa = NULL;
	update(x);
}

node* find(int x)
{
	node *now = rt;
	while(now != NULL && now->x != x)
	{
		if(now->x > x)
			now = now->ch[0];
		else
			now = now->ch[1];
	}
	return now;
}

node* findl(node *p, int x)
{
	node *now = p, *ans = NULL;
	while(now != NULL)
	{
		if(now->x >= x)
			now = now->ch[0];
		else
			ans = now, now = now->ch[1];	
	}
	return ans;
}

node *findr(node *p, int x)
{
	node *now = p, *ans = NULL;
	while(now != NULL)
	{
		if(x < now->x)
			ans = now, now = now->ch[0];
		else
			now = now->ch[1];	
	}
	return ans;
}

node* insert(int x)
{
	if(rt == NULL)
		return newnode(x);
	
	int l = 0;
	node *now = rt, *fa = NULL;
	while(now != NULL && now->x != x)
	{
		fa = now;
		if(now->x > x)
			now = now->ch[l = 0];
		else
			now = now->ch[l = 1];	
	}

	if(now != NULL)
	{
		now->cnt++;
		update(now);
	}
	else
	{
		now = newnode(x);
		now->fa = fa;
		fa->ch[l] = now;
	}

	splay(now);
	return now;
}

node* dec(int x)
{
	node *now = find(x), *nrt;
	assert(now != NULL);
	now->cnt--;splay(now);
	if(now->cnt > 0)
		return now;
	
	if(now->ch[0] == NULL && now->ch[1] == NULL)
	{
		free(now);
		return NULL;
	}
	else if(now->ch[0] != NULL && now->ch[1] != NULL)
	{
		splay(nrt = findl(now, x));
		nrt->ch[1] = now->ch[1];
		if(nrt->ch[1] != NULL)
			nrt->ch[1]->fa = nrt;
	}
	else
	{
		nrt = now->ch[now->ch[0] == NULL];
		nrt->fa = NULL;
	}

	free(now);
	update(nrt);
	return nrt;
}

int findkth(int k)
{
	node *now = rt;
	while(now != NULL)
	{
		if(now->ch[0] != NULL && k <= now->ch[0]->siz)
			now = now->ch[0];
		else if(now->ch[1] == NULL || k <= now->siz - now->ch[1]->siz)
			return now->x;
		else
		{
			k -= now->siz - now->ch[1]->siz;
			now = now->ch[1];
		}
	}
	return -1;
}

int findrk(int x)
{
	int ret = 0;
	node *now = rt;
	while(now != NULL)
	{
		if(now->x < x)
		{
			if(now->ch[0] != NULL)
				ret += now->ch[0]->siz;
			ret += now->cnt;
	   		now = now->ch[1];
		}
		else
			now = now->ch[0];
	}
	return ret;
}

int query(int x, int ty)
{
	node *ret = ty ? findr(rt, x) : findl(rt, x);
	return ret == NULL ? -1 : ret->x;
}

int read()
{
	int x = 0;char ch = getchar();
	while(ch < '0' || '9' < ch)ch = getchar();
	while('0' <= ch && ch <= '9')x = x * 10 + (ch ^ 48), ch = getchar();
	return x;
}

int write(int x)
{
	if(x > 9)write(x / 10);
	putchar(x % 10 ^ 48);
}

int writes(int x)
{
	if(x < 0)
		putchar('-'), x = -x;
	write(x);
	putchar('\n');
}

void freeall(node *x)
{
	if(x->ch[0] != NULL)
		freeall(x->ch[0]);
	if(x->ch[1] != NULL)
		freeall(x->ch[1]);
	free(x);
}
