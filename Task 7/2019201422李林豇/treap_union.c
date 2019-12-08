#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define lc son[0]
#define rc son[1]

const int N=5e5+100;

typedef int (*cmp_pointer)(const void *, const void *);

int comp(const void *a, const void *b)
{
	return (*(int*)a) - (*(int*)b);
}

struct Tree_node{
	struct Tree_node *son[2];
	void *data;
	int cnt, siz, rnd;
}*root;
typedef struct Tree_node t_node;

void update(t_node *tmp)
{
	tmp->siz = (tmp->lc==NULL ? 0:tmp->lc->siz) + (tmp->rc==NULL ? 0:tmp->rc->siz) + tmp->cnt;
}

t_node *new_node()
{
	t_node *tmp = (t_node*)malloc(sizeof(t_node));
	tmp->lc = tmp->rc = NULL;
	tmp->siz = tmp->cnt = 1;
	tmp->rnd = rand();
	return tmp;
}

void free_node(t_node* tmp)
{
	free(tmp->data);
	free((void*)tmp);
}

void rotate(t_node **tmp, int k)
{
	t_node *v = (*tmp)->son[k^1];
	(*tmp)->son[k^1] = v->son[k];
	v->son[k]= (*tmp);
	update((*tmp));
	update((*tmp) = v);
}

void insert(t_node **tmp,const void *key, cmp_pointer comp, int width)
{
	if((*tmp) == NULL)
	{
		(*tmp) = new_node();
		(*tmp)->data = malloc(width);
		memcpy((*tmp)->data, key, width);
		return;
	}
	if(!comp(key, (*tmp)->data))
	{
		++(*tmp)->siz;
		++(*tmp)->cnt;
		return;
	}
	int d = comp(key, (*tmp)->data) > 0;
	++(*tmp)->siz;
	insert(&((*tmp)->son[d]), key, comp, width);
	if((*tmp)->son[d]->rnd < (*tmp)->rnd)
		rotate(tmp, d ^ 1);
	return;
}

bool del(t_node **tmp, const void *key, cmp_pointer comp)
{
	if((*tmp)==NULL)
		return false;
	if(!comp(key, (*tmp)->data))
	{
		if((*tmp)->cnt > 1)
		{
			--(*tmp)->cnt;
			--(*tmp)->siz;
			return true;
		}
		if((*tmp)->lc==NULL || (*tmp)->rc==NULL)
		{
			t_node* temp;
			temp = (*tmp)->lc == NULL ? (*tmp)->rc : (*tmp)->lc;
			free_node(*tmp);
			*tmp = temp;
			return true;
		}
		if((*tmp)->lc->rnd < (*tmp)->rc->rnd)
			rotate(tmp, 1);
		else
			rotate(tmp, 0);
		return del(tmp, key, comp);
	}
	int d = comp(key, (*tmp)->data) > 0;
	if(!del(&((*tmp)->son[d]), key, comp))
		return false;
	--(*tmp)->siz;
	return true;
}

void* find_key(t_node *tmp, int rnk, cmp_pointer comp)
{
	if(tmp->siz < rnk)
		return NULL;
	while(1)
	{
		if(rnk > (tmp->lc==NULL ? 0:tmp->lc->siz) + tmp->cnt)
			rnk -= (tmp->lc==NULL ? 0:tmp->lc->siz) + tmp->cnt, tmp = tmp->rc;
		else
		{
			if(rnk <= (tmp->lc==NULL ? 0:tmp->lc->siz))
				tmp = tmp->lc;
			else
				return tmp->data;
		}
	}
}

int find_rnk(t_node *tmp, const void *key, cmp_pointer comp)
{
	int rnk = 0;
	while(tmp != NULL)
	{
		if(!comp(key, tmp->data))
			return rnk + (tmp->lc==NULL ? 0:tmp->lc->siz);
		if(comp(key, tmp->data) > 0)
			rnk += (tmp->lc==NULL ? 0:tmp->lc->siz) + tmp->cnt, tmp = tmp->rc;
		else
			tmp = tmp->lc;
	}
	return rnk;
}


void* find_pre(t_node *tmp, const void* key, cmp_pointer comp)
{
	void* pre = NULL;
	while(tmp != NULL)
	{
		if(comp(key, tmp->data)<=0)
			tmp = tmp->lc;
		else
			pre = tmp->data, tmp = tmp->rc;
	}
	return pre;
}

void* find_nxt(t_node *tmp, const void* key, cmp_pointer comp)
{
	void* nxt = NULL;
	while(tmp != NULL)
	{
		if(comp(key, tmp->data)>=0)
			tmp = tmp->rc;
		else
			nxt = tmp->data, tmp = tmp->lc;
	}
	return nxt;
}

void free_tree(t_node *tmp)
{
	if(tmp==NULL)
		return;
	free(tmp->data);
	free_tree(tmp->lc);
	free_tree(tmp->rc);
	free(tmp);
}

struct Treap{
	t_node *root;
	cmp_pointer comp;
	int width;
};
typedef struct Treap Treap;

Treap* Treap_build(cmp_pointer comp, int width)
{
	Treap* tmp = malloc(sizeof(Treap));
	tmp->comp = comp;
	tmp->root = NULL;
	tmp->width = width;
	return tmp;
}

void Treap_insert(Treap *tmp, void* key)
{
	insert(&(tmp->root), key, tmp->comp, tmp->width);
}

bool Treap_delete(Treap *tmp, void* key)
{
	return del(&(tmp->root), key, tmp->comp);
}

void* Treap_find_data(Treap *tmp, int rnk)
{
	return find_key(tmp->root, rnk, tmp->comp);
}

int Treap_find_rnk(Treap *tmp, void* key)
{
	return find_rnk(tmp->root, key, tmp->comp);
}

void* Treap_find_pre(Treap *tmp, void* key)
{
	return find_pre(tmp->root, key, tmp->comp);
}

void* Treap_find_nxt(Treap *tmp, void* key)
{
	return find_nxt(tmp->root, key, tmp->comp);
}

void* Treap_free(Treap *tmp)
{
	free_tree(tmp->root);
}

int main()
{
	int n, opt, x;
	int *p;
	Treap *my_treap = Treap_build(comp, sizeof(int));
	scanf("%d", &n);
	for (int i = 1; i <= n;i++)
	{
		scanf("%d%d", &opt, &x);
		++opt;
		switch(opt)
		{
			case 1:
				Treap_insert(my_treap, &x);
				break;
			case 2:
				Treap_delete(my_treap, &x);
				break;
			case 3:
				p = Treap_find_data(my_treap, x);
				printf("%d\n", p == NULL ? -1 : *p);
				break;
			case 4:
				printf("%d\n", Treap_find_rnk(my_treap, &x));
				break;
			case 5:
				p = Treap_find_pre(my_treap, &x);
				printf("%d\n", p == NULL ? -1 : *p);
				break;
			case 6:
				p = Treap_find_nxt(my_treap, &x);
				printf("%d\n", p == NULL ? -1 : *p);
				break;
		}
	}
	Treap_free(my_treap);
	return 0;
}