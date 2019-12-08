#include<stdlib.h>
#define TYPE int

struct _node
{
	//count of the same values
	int count;
	//size of the tree below
	int size;
	//value of the node
	int value;
	//child[0]: address of left child, child[1]: address of right child
	struct _node* child[2];
	//address of father
	struct _node* parent;
};

struct _splay
{
	//size of the tree
	int size;
	//address of root
	struct _node *root;
};

struct _splay* _splay_build()
{
	//build a empty tree
	
	struct _splay* tmp = (struct _splay*)malloc(sizeof(struct _splay));
	tmp-> size = 0;
	tmp-> root = NULL;
	return tmp;
}

int _splay_getv(struct _node *p)
{
	return p-> value;
}

void _splay_clear(struct _node *p)
{
	//delete one node
	
	free(p);
}

void _splay_destory(struct _splay *p)
{
	//destory the tree
	
	p->size = 0;
	p->root = NULL;
	free(p);
}

void _splay_update(struct _node *p)
{
	//update the saved information
	//'p' is the address of the node
	
	if(p == NULL) return;
	p-> size = p-> count;
	if(p-> child[0] != NULL) p-> size += (p-> child[0])-> size;
	if(p-> child[1] != NULL) p-> size += (p-> child[1])-> size;
}

void _splay_rotate(struct _node *p)
{
	//rotate one node to it's father
	//'p' is the address of the node
	
	struct _node *fa  = p -> parent;
	struct _node *ffa = fa-> parent;
	TYPE k = (fa-> child[1] == p);
	fa-> child[k] = p -> child[k^1];
	if(fa-> child[k] !=NULL) (fa-> child[k])-> parent = fa;
	p-> child[k^1] = fa;
	fa-> parent = p;
	p -> parent = ffa;
	if(ffa != NULL) ffa-> child[ffa-> child[1] == fa] = p;
	_splay_update(fa);
	_splay_update(p);
}

void _splay_splay(struct _splay *T, struct _node *p)
{
	//make one node arrive to the root's place
	//'p' is the address of the node
	
	struct _node *fa;
	for(;fa = p-> parent; _splay_rotate(p))
	{
		if(fa-> parent !=NULL)
		{
			if((fa-> child[1] == fa) == (p-> child[1] == p)) _splay_rotate(fa);
			else _splay_rotate(p);
		}
	}
	T-> root = p;
}

struct _node *_splay_pre(struct _splay *T)
{
	//find the previous node in tree
	
	struct _node *now = (T-> root)-> child[0];
	if(now == NULL) return NULL;
	while(now-> child[1] != NULL) now = now-> child[1];
	return now;	
}

struct _node *_splay_nex(struct _splay *T)
{
	//find the next node in tree
	
	struct _node *now = (T-> root)-> child[1];
	if(now == NULL) return NULL;
	while(now-> child[0] != NULL) now = now-> child[0];
	return now;
}

int _splay_kth(struct _splay *T, int a)
{
	//find the kth node in tree
	//'a' equal to k
	
	struct _node *now = T-> root;
	while(1){
		if(now-> child[0] && (now-> child[0])-> size >= a) now = now-> child[0];
		else{
			a -= now-> count;
			if(now-> child[0]) a -= (now-> child[0])->size;
			if(a <= 0) return (*now).value;
			now = now-> child[1];
		}
	}
}

int _splay_rank(struct _splay *T, int a)
{
	//count how many numbers smaller than key
	//'a' is the key
	
	struct _node *now = T-> root;
	int ans = 0;
	while(1){
		if(now == NULL) return ans;
		if(a < now-> value) now = now-> child[0];
		else{
			if(now-> child[0] != NULL) ans += (now-> child[0])-> size;
			if(a == now-> value) return ans;
			ans += now-> count;
			if(now-> child[1] == NULL) return ans;
			now = now-> child[1];
		}
	}
}

void _splay_insert(struct _splay *T, int a)
{
	//create an empty node and then make it's value equal to a

	if(T-> root == NULL){
		T-> root = (struct _node*)malloc(sizeof(struct _node));
		(T-> root)-> value = a;
		(T-> root)-> size  = 1;
		(T-> root)-> count = 1;
	}
	else{
		struct _node *now = T-> root;
		struct _node *fa  = NULL;
		while(1){
			if(now-> value == a){
				now-> count ++;
				_splay_update(now);
				_splay_update(fa);
				_splay_splay(T, now);
				break;
			}
			fa = now;
			now = now-> child[a > now-> value];
			if(now == NULL){
				now = (struct _node*)malloc(sizeof(struct _node));
				now -> value    = a;
				now -> parent   = fa;
				now -> count    = 1;
				now -> size     = 1;
				now -> child[0] = NULL;
				now -> child[1] = NULL;
				fa -> size ++; 
				fa -> child[a > fa-> value ]= now;
				_splay_splay(T, now);
				break; 
			}
		}
	}
}

void _splay_erase(struct _splay *T, int a)
{
	//erase an appointed node
	if(T-> root == NULL) return;
	struct _node *now = T-> root;
	struct _node *fa  = NULL;
	while(1){
		if(now-> value == a){
			_splay_splay(T, now);
			break;
		}
		fa = now;
		now = now-> child[a> now-> value];
		if(now == NULL) return;
	}
	if((T-> root)-> count > 1){
		(T-> root)-> count --;
		_splay_update(T-> root);
	}
	else if((T-> root)-> child[1] == NULL && (T-> root)-> child[0] == NULL){
		_splay_clear(T-> root);
		_splay_destory(T);
	}
	else if((T-> root)-> child[1] == NULL){
		struct _node *tmp = T->root;
		T-> root = (T-> root) -> child[0];
		if(T-> root != NULL) (T-> root)-> parent = NULL;
		_splay_clear(tmp);
	}
	else if((T-> root)-> child[0] == NULL){
		struct _node *tmp = T->root;
		T-> root = (T-> root) -> child[1];
		if(T-> root != NULL) (T-> root)-> parent = NULL;
		_splay_clear(tmp);
	}
	else{
		struct _node *re  = _splay_pre(T);
		struct _node *tmp = T-> root;
		_splay_splay(T, re);
		(T-> root)-> child[1] = tmp-> child[1];
		(tmp-> child[1])-> parent = T-> root;
		_splay_clear(tmp);
		_splay_update(T->root);
	}
}

