struct node
{
	int num, siz, tim;
	struct node *ls, *rs, *fa;
};

struct splay
{
	struct node* root;
};

extern void init(struct splay *);
extern void insert(struct splay *, int);
extern void del(struct splay *, int);
extern int getkth(struct splay *, int);
extern int getrnk(struct splay *, int);
extern int getrmax(struct splay *, int);
extern int getrmin(struct splay* , int);
