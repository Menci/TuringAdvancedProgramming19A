struct _node
{
	int count;
	int size;
	int value;
	struct _node* child[2];
	struct _node* parent;
};
struct _splay
{
	int size;
	struct _node *root;
};
extern struct _splay* _splay_build();
extern void _splay_insert(struct _splay*, int);
extern void _splay_erase (struct _splay*, int);
extern int  _splay_kth   (struct _splay*, int);
extern int  _splay_rank  (struct _splay*, int);
extern struct _node* _splay_pre(struct _splay*);
extern struct _node* _splay_nex(struct _splay*);
