#ifndef _RB_TREE_H
#define _RB_TREE_H

enum COLOR {RED = 0, BLACK = 1};
enum SON_KIND {LEFT = 0, RIGHT = 1};

struct RB_Tree {
	int val, siz, col, sum;
	struct RB_Tree *son[2];
	struct RB_Tree *fa;
};

extern struct RB_Tree *root;

void print(struct RB_Tree *p);
int Get_Id(struct RB_Tree *p, int key);
int Get_Rank(struct RB_Tree *p, int val);
struct RB_Tree *Get_Last(struct RB_Tree *p, int val);
struct RB_Tree *Get_Next(struct RB_Tree *p, int val);
void Insert(struct RB_Tree *p, int val);
void Delete(struct RB_Tree *p, int val);
void Clean();

#endif
