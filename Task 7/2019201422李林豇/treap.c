#include"treap.h"
#include"malloc.c"
#define malloc my_malloc 
#define free my_free

static void update(t_node *now_node)
{
	now_node->siz = (now_node->lc==NULL ? 0:now_node->lc->siz) + (now_node->rc==NULL ? 0:now_node->rc->siz) + now_node->cnt;
}

static t_node *new_node()
{
	t_node *now_node = (t_node*)malloc(sizeof(t_node));
	now_node->lc = now_node->rc = NULL;
	now_node->siz = now_node->cnt = 1;
	now_node->rnd = rand();
	return now_node;
}

static void free_node(t_node* now_node)
{
	free(now_node->data);
	free((void*)now_node);
}

static void rotate(t_node **now_node, int k)
{
	t_node *v = (*now_node)->son[k^1];
	(*now_node)->son[k^1] = v->son[k];
	v->son[k]= (*now_node);
	update((*now_node));
	update((*now_node) = v);
}

static void insert(t_node **now_node,const void *key, cmp_pointer comp, int width)
{
	if((*now_node) == NULL)
	{
		(*now_node) = new_node();
		(*now_node)->data = malloc(width);
		memcpy((*now_node)->data, key, width);
		return;
	}
	if(!comp(key, (*now_node)->data))
	{
		++(*now_node)->siz;
		++(*now_node)->cnt;
		return;
	}
	int d = comp(key, (*now_node)->data) > 0;
	++(*now_node)->siz;
	insert(&((*now_node)->son[d]), key, comp, width);
	if((*now_node)->son[d]->rnd < (*now_node)->rnd)
		rotate(now_node, d ^ 1);
	return;
}

static bool del(t_node **now_node, const void *key, cmp_pointer comp)
{
	if((*now_node)==NULL)
		return false;
	if(!comp(key, (*now_node)->data))
	{
		if((*now_node)->cnt > 1)
		{
			--(*now_node)->cnt;
			--(*now_node)->siz;
			return true;
		}
		if((*now_node)->lc==NULL || (*now_node)->rc==NULL)
		{
			t_node* temp;
			temp = (*now_node)->lc == NULL ? (*now_node)->rc : (*now_node)->lc;
			free_node(*now_node);
			*now_node = temp;
			return true;
		}
		if((*now_node)->lc->rnd < (*now_node)->rc->rnd)
			rotate(now_node, 1);
		else
			rotate(now_node, 0);
		return del(now_node, key, comp);
	}
	int d = comp(key, (*now_node)->data) > 0;
	if(!del(&((*now_node)->son[d]), key, comp))
		return false;
	--(*now_node)->siz;
	return true;
}

static void* find_key(t_node *now_node, int rnk, cmp_pointer comp)
{
	if(now_node->siz < rnk)
		return NULL;
	while(1)
	{
		if(rnk > (now_node->lc==NULL ? 0:now_node->lc->siz) + now_node->cnt)
			rnk -= (now_node->lc==NULL ? 0:now_node->lc->siz) + now_node->cnt, now_node = now_node->rc;
		else
		{
			if(rnk <= (now_node->lc==NULL ? 0:now_node->lc->siz))
				now_node = now_node->lc;
			else
				return now_node->data;
		}
	}
}

static int find_rnk(t_node *now_node, const void *key, cmp_pointer comp)
{
	int rnk = 0;
	while(now_node != NULL)
	{
		if(!comp(key, now_node->data))
			return rnk + (now_node->lc==NULL ? 0:now_node->lc->siz);
		if(comp(key, now_node->data) > 0)
			rnk += (now_node->lc==NULL ? 0:now_node->lc->siz) + now_node->cnt, now_node = now_node->rc;
		else
			now_node = now_node->lc;
	}
	return rnk;
}


static void* find_pre(t_node *now_node, const void* key, cmp_pointer comp)
{
	void* pre = NULL;
	while(now_node != NULL)
	{
		if(comp(key, now_node->data)<=0)
			now_node = now_node->lc;
		else
			pre = now_node->data, now_node = now_node->rc;
	}
	return pre;
}

static void* find_nxt(t_node *now_node, const void* key, cmp_pointer comp)
{
	void* nxt = NULL;
	while(now_node != NULL)
	{
		if(comp(key, now_node->data)>=0)
			now_node = now_node->rc;
		else
			nxt = now_node->data, now_node = now_node->lc;
	}
	return nxt;
}

static void free_tree(t_node *now_node)
{
	if(now_node==NULL)
		return;
	free(now_node->data);
	free_tree(now_node->lc);
	free_tree(now_node->rc);
	free(now_node);
}

Treap* Treap_build(cmp_pointer comp, int width)
{
	Treap* my_treap = malloc(sizeof(Treap));
	my_treap->comp = comp;
	my_treap->root = NULL;
	my_treap->width = width;
	return my_treap;
}

void Treap_insert(Treap *my_treap, void* key)
{
	insert(&(my_treap->root), key, my_treap->comp, my_treap->width);
}

bool Treap_delete(Treap *my_treap, void* key)
{
	return del(&(my_treap->root), key, my_treap->comp);
}

void* Treap_find_data(Treap *my_treap, int rnk)
{
	return find_key(my_treap->root, rnk, my_treap->comp);
}

int Treap_find_rnk(Treap *my_treap, void* key)
{
	return find_rnk(my_treap->root, key, my_treap->comp);
}

void* Treap_find_pre(Treap *my_treap, void* key)
{
	return find_pre(my_treap->root, key, my_treap->comp);
}

void* Treap_find_nxt(Treap *my_treap, void* key)
{
	return find_nxt(my_treap->root, key, my_treap->comp);
}

void* Treap_free(Treap *my_treap)
{
	free_tree(my_treap->root);
	free(my_treap);
}
