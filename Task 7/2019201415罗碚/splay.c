#include<stdio.h>
#include<malloc.h>

struct node
{
	int num, siz, tim;
	struct node *ls, *rs, *fa;
};

struct node* make_node(int n)
{
	struct node* x = (struct node *) malloc(sizeof(struct node));
	x -> ls = x -> rs = x -> fa = NULL;
	x -> num = n;
	x -> siz = x -> tim = 1;
	return x;
}

struct splay
{
	struct node* root;
};

void init(struct splay *x)
{
	x -> root = NULL;
}

int siz(struct node *x)
{
	if(x == NULL) return 0;
	return x -> siz;
}

void calcsize(struct node *x)
{
	x -> siz = x -> tim + siz(x -> ls) + siz(x -> rs);
}

void zag(struct node *x)
{
	if(x -> fa == NULL) return ;
	struct node *y = x -> fa;
	y -> rs = x -> ls;
	if(x -> ls != NULL) x -> ls -> fa = y;
	if(y -> fa != NULL)
		if(y -> fa -> ls == y)
			y -> fa -> ls = x;
		else
			y -> fa -> rs = x;
	x -> fa = y -> fa;
	x -> ls = y;
	y -> fa = x;
	calcsize(y);
	calcsize(x);
}

void zig(struct node *x)
{
	if(x -> fa == NULL) return ;
	struct node *y = x -> fa;
	y -> ls = x -> rs;
	if(x -> rs != NULL) x -> rs -> fa = y;
	if(y -> fa != NULL)
		if(y -> fa -> ls == y)
			y -> fa -> ls = x;
		else
			y -> fa -> rs = x;
	x -> fa = y -> fa;
	x -> rs = y;
	y -> fa = x;
	calcsize(y);
	calcsize(x);
}

void splay(struct node *x,struct splay *r)
{
	struct node *y;
	while((y = x -> fa) != NULL)
	{
		if(y -> fa == NULL)
		{
			if(y -> ls == x)
				zig(x);
			else
				zag(x);
		}
		else if(y -> fa -> ls == y)
		{
			if(y -> ls == x)
			{
				zig(y);
				zig(x);
			}
			else
			{
				zag(x);
				zig(x);
			}
		}
		else
		{
			if(y -> ls == x)
			{
				zig(x);
				zag(x);
			}
			else
			{
				zag(y);
				zag(x);
			}
		}
	}
	r -> root = x; 
}

struct node* binsert(struct node *x, int n)
{
	if(x -> num == n)
	{
		x -> tim ++;
		calcsize(x);
		return x;
	}
	struct node* res;
	if(x -> num > n)
	{
		if(x -> ls == NULL)
		{
			res = x -> ls = make_node(n);
			res -> fa = x;
		}
		else
			res = binsert(x -> ls, n);
		calcsize(x);
		return res;
	}
	if(x -> num < n)
	{
		if(x -> rs == NULL)
		{
			res = x -> rs =  make_node(n);
			res -> fa = x;
		}
		else
			res = binsert(x -> rs, n);
		calcsize(x);
		return res;
	}
}

void insert(struct splay *r, int n)
{
	if(r -> root == NULL)
	{
		r -> root = make_node(n);
		return ;
	}
	splay(binsert(r -> root, n), r);
}

struct node* find(struct node *x, int n)
{
	if(x == NULL) return x;
	if(x -> num == n) return x;
	if(x -> num > n) return find(x -> ls, n);
	return find(x -> rs, n);
}

void del(struct splay *r, int n)
{
	splay(find(r -> root, n), r);
	if(r -> root -> tim >= 2)
	{
		r -> root -> tim --;
		calcsize(r -> root);
		return ;
	}
	struct node *ls = r -> root -> ls;
	struct node *rs = r -> root -> rs;
	free(r -> root);
	if(ls == NULL && rs == NULL)
	{
		r -> root = NULL;
		return ;
	}
	if(ls == NULL)
	{
		rs -> fa = NULL;
		r -> root = rs;
		return ;
	}
	if(rs == NULL)
	{
		ls -> fa = NULL;
		r -> root = ls;
		return ;
	}
	ls -> fa = NULL;
	while(ls -> rs != NULL) ls = ls -> rs;
	splay(ls, r);
	ls -> rs = rs;
	rs -> fa = ls;
	calcsize(ls);
}

int fk(struct node *x, int k)
{
	if(siz(x -> ls) >= k) return fk(x -> ls, k);
	k -= siz(x -> ls);
	if(x -> tim >= k) return x -> num;
	k -= x -> tim;
	return fk(x -> rs, k);
}

int getkth(struct splay *r, int k)
{
	return fk(r -> root, k);
}

int getrnk(struct splay *r, int n)
{
	insert(r, n);
	int res = siz(r -> root -> ls);
	del(r, n);
	return res;
}

int getrmax(struct splay *r, int n)
{
	int res;
	insert(r, n);
	struct node *x = r -> root -> ls;
	if(x == NULL)
		res = -1;
	else
	{
		while(x -> rs != NULL) x = x -> rs;
		res = x -> num;
	}
	del(r, n);
	return res;
}

int getrmin(struct splay* r, int n)
{
	int res;
	insert(r, n);
	struct node *x = r -> root -> rs;
	if(x == NULL)
		res = -1;
	else
	{
		while(x -> ls != NULL) x = x -> ls;
		res = x -> num;
	}
	del(r, n);
	return res;
}
