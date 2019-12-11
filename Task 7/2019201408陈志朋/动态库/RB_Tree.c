#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include "RB_Tree.h"

struct RB_Tree *root = NULL;
static struct RB_Tree *EMPTY = NULL;

void print(struct RB_Tree *p) {
	if (p == NULL || p == EMPTY) return;
	print(p->son[LEFT]);
	if (p != root) printf ("%d %d %d %d %d\n", p->val, p->sum, p->siz, p->col, (p->fa)->val);
	else printf ("%d %d %d %d\n", p->val, p->sum, p->siz, p->col);
	print(p->son[RIGHT]);
	if (p == root) puts("------------------------------------");
}
static void Update(struct RB_Tree *p) {
	(p->son[LEFT])->fa = (p->son[RIGHT])->fa = p;
	p->siz = ((p->son[LEFT])->siz) + ((p->son[RIGHT])->siz) + (p->sum);
}
static void Rotate_Right(struct RB_Tree *p) {
	struct RB_Tree *kid = p->son[LEFT];
	p->son[LEFT] = kid->son[RIGHT];
	kid->son[RIGHT] = p;
	kid->fa = p->fa;
	if (root != p) {
		if ((p->fa)->son[LEFT] == p) (p->fa)->son[LEFT] = kid;
		else (p->fa)->son[RIGHT] = kid;
	}
	Update(p); Update(kid);
	if (root == p) {
		kid->col = BLACK;
		root = kid;
	}
}
static void Rotate_Left(struct RB_Tree *p) {
	struct RB_Tree *kid = p->son[RIGHT];
	p->son[RIGHT] = kid->son[LEFT];
	kid->son[LEFT] = p;
	kid->fa = p->fa;
	if (root != p) {
		if ((p->fa)->son[LEFT] == p) (p->fa)->son[LEFT] = kid;
		else (p->fa)->son[RIGHT] = kid;
	}
	Update(p); Update(kid);
	if (root == p) {
		kid->col = BLACK;
		root = kid;
	}
}
static void Adjust_Add(struct RB_Tree *p) {
	if (p->fa == NULL) return (void)(p->col = BLACK);
	else {
		struct RB_Tree *fa = p->fa;
		if (fa->col == BLACK) return;
		else if (fa->col == RED) {
			struct RB_Tree *pfa = fa->fa;
			struct RB_Tree *ucl = pfa->son[(bool)(pfa->son[LEFT] == fa)];
			if (ucl->col == RED) {
				pfa->col = RED;
				fa->col = ucl->col = BLACK;
				Adjust_Add(pfa);
			} else if (ucl->col == BLACK) {
				if (pfa->son[LEFT] == fa) {
					if (fa->son[LEFT] == p) {
						fa->col = BLACK; pfa->col = RED;
						Rotate_Right(pfa);
					} else if (fa->son[RIGHT] == p) {
						Rotate_Left(fa);
						Adjust_Add(fa);
					}
				} else if (pfa->son[RIGHT] == fa) {
					if (fa->son[RIGHT] == p) {
						fa->col = BLACK; pfa->col = RED;
						Rotate_Left(pfa);
					} else if (fa->son[LEFT] == p) {
						Rotate_Right(fa);
						Adjust_Add(fa);
					}
				}
			}
		}
	}
}
static void New_Node(struct RB_Tree *fa, struct RB_Tree *p, int val) {
	static bool flag = 0;
	if (!flag) {
		flag = 1;
		EMPTY = malloc(sizeof(struct RB_Tree));
		EMPTY->col = BLACK;
		EMPTY->sum = EMPTY->siz = 0;
		EMPTY->son[LEFT] = EMPTY->son[RIGHT] = NULL;
	}
	p->val = val; p->siz = 1; p->col = RED; p->sum = 1;
	p->son[LEFT] = p->son[RIGHT] = EMPTY;
	p->fa = fa;
	Adjust_Add(p);
}
void Insert(struct RB_Tree *p, int val) {
	if (root == NULL) {
		root = malloc(sizeof(struct RB_Tree));
		New_Node(NULL, root, val);
		return;
	} else if (root != NULL) {
		if (p->val == val) {
			p->sum += 1;
			p->siz += 1;
			return;
		} else if (p->val < val) {
			p->siz += 1;
			if (p->son[RIGHT] == EMPTY) {
				p->son[RIGHT] = malloc(sizeof(struct RB_Tree));
				New_Node(p, p->son[RIGHT], val);
			} else Insert(p->son[RIGHT], val);
		} else if (p->val > val) {
			p->siz += 1;
			if (p->son[LEFT] == EMPTY) {
				p->son[LEFT] = malloc(sizeof(struct RB_Tree));
				New_Node(p, p->son[LEFT], val);
			} else Insert(p->son[LEFT], val);
		}
	}
}
int Get_Rank(struct RB_Tree *p, int val) {
	if (p == EMPTY) return 1;
	if (p->val == val) return ((p->son[LEFT])->siz) + 1;
	else if (p->val < val) return ((p->son[LEFT])->siz) + (p->sum) + Get_Rank(p->son[RIGHT], val);
	else if (p->val > val) return Get_Rank(p->son[LEFT], val);
}
int Get_Id(struct RB_Tree *p, int key) {
	if ((p->son[LEFT])->siz >= key) return Get_Id(p->son[LEFT], key);
	else if ((p->son[LEFT])->siz + p->sum >= key) return p->val;
	else return Get_Id(p->son[RIGHT], key - ((p->son[LEFT])->siz + p->sum));
}
struct RB_Tree *Get_Last(struct RB_Tree *p, int val) {
	struct RB_Tree *ret = NULL;
	if (p == EMPTY) return NULL;
	if (p->val > val) ret = Get_Last(p->son[LEFT], val);
	else if (p->val < val) ret = Get_Last(p->son[RIGHT], val);
	
	if (ret == NULL) {
		if (p->val == val) {
			if (p->son[LEFT] != EMPTY) {
				ret = p->son[LEFT];
				while (ret->son[RIGHT] != EMPTY)
					ret = ret->son[RIGHT];
			}
		} else if (p->val < val) ret = p;
	}
	return ret;
}
struct RB_Tree *Get_Next(struct RB_Tree *p, int val) {
	struct RB_Tree *ret = NULL;
	if (p == EMPTY) return NULL;
	if (p->val > val) ret = Get_Next(p->son[LEFT], val);
	else if (p->val < val) ret = Get_Next(p->son[RIGHT], val);
	if (ret == NULL) {
		if (p->val == val) {
			if (p->son[RIGHT] != EMPTY) {
				ret = p->son[RIGHT];
				while (ret->son[LEFT] != EMPTY)
					ret = ret->son[LEFT];
			}
		} else if (p->val > val) ret = p;
	}
	return ret;
}
static void Copy(struct RB_Tree *p, struct RB_Tree *q) {
	p->val = q->val;
	p->sum = q->sum;
	do {
		Update(p);
	}while ((p = p->fa) != NULL);
}
static void Adjust_Del(struct RB_Tree *p) {
	if (p == root) return;
	if (p->col == RED) return;
	else if (p->col == BLACK) {
		struct RB_Tree *fa = p->fa;
		struct RB_Tree *bro;
		if (fa->son[LEFT] == p) {
			bro = fa->son[RIGHT];
			if (bro->col == RED) {
				fa->col = RED; bro->col = BLACK;
				Rotate_Left(fa);
				Adjust_Del(p);
			} else if (bro->col == BLACK) {
				if ((bro->son[RIGHT])->col == RED) {
					bro->col = fa->col;
					fa->col = BLACK;
					(bro->son[RIGHT])->col = BLACK;
					Rotate_Left(fa);
				} else if ((bro->son[LEFT])->col == RED) {
					(bro->son[LEFT])->col = BLACK;
					bro->col = RED;
					Rotate_Right(bro);
					Adjust_Del(p);
				} else {
					bro->col = RED;
					if (fa->col == RED) fa->col = BLACK;
					else Adjust_Del(fa);
				}
			}
		} else if (fa->son[RIGHT] == p) {
			bro = fa->son[LEFT];
			if (bro->col == RED) {
				fa->col = RED; bro->col = BLACK;
				Rotate_Right(fa);
				Adjust_Del(p);
			} else if (bro->col == BLACK) {
				if ((bro->son[LEFT])->col == RED) {
					bro->col = fa->col; 
					fa->col = BLACK;
					(bro->son[LEFT])->col = BLACK;
					Rotate_Right(fa);
				} else if ((bro->son[RIGHT])->col == RED) {
					(bro->son[RIGHT])->col = BLACK;
					bro->col = RED;
					Rotate_Left(bro);
					Adjust_Del(p);
				} else {
					bro->col = RED;
					if (fa->col == RED) fa->col = BLACK;
					else Adjust_Del(fa);
				}
			}
		}
	}
}
void Delete(struct RB_Tree *p, int val) {
	if (p->val < val) Delete(p->son[RIGHT], val);
	else if (p->val > val) Delete(p->son[LEFT], val);
	else {
		struct RB_Tree *t1 = p, *t2;
		if (p->sum > 1) {
			p->sum -= 1; p->siz -= 1;
			while (t1 != root) Update(t1 = t1->fa);
		} else {
			if (p->son[LEFT] != EMPTY && p->son[RIGHT] != EMPTY) {
				t2 = Get_Next(root, val);
				Copy(p, t2);
			} else t2 = p;
			Adjust_Del(t2);
			if (t2->son[LEFT] != EMPTY) {
				t1 = t2->son[LEFT];
				t1->fa = t2->fa;
				if (t2 != root) {
					if ((t2->fa)->son[LEFT] == t2) (t2->fa)->son[LEFT] = t1;
					else if ((t2->fa)->son[RIGHT] == t2) (t2->fa)->son[RIGHT] = t1;
				} else {
					t1->col = BLACK;
					root = t1;
				}
			} else if (t2->son[RIGHT] != EMPTY){
				t1 = t2->son[RIGHT];
				t1->fa = t2->fa;
				if (t2 != root) {
					if ((t2->fa)->son[LEFT] == t2) (t2->fa)->son[LEFT] = t1;
					else if ((t2->fa)->son[RIGHT] == t2) (t2->fa)->son[RIGHT] = t1;
				} else {
					t1->col = BLACK;
					root = t1;
				}
			} else {
				if (t2 != root) {
					if ((t2->fa)->son[LEFT] == t2) (t2->fa)->son[LEFT] = EMPTY;
					else if ((t2->fa)->son[RIGHT] == t2) (t2->fa)->son[RIGHT] = EMPTY;
					t1 = t2->fa;
				} else if (t2 == root) root = NULL;
			}
			free(t2);
			do {
				Update(t1);
			}while ((t1 = t1->fa) != NULL);
		}
	}
}
static void clean(struct RB_Tree *p) {
	if (p == NULL || p == EMPTY) return;
	clean(p->son[LEFT]);
	clean(p->son[RIGHT]);
	free(p);
}
void Clean() {
	clean(root);
	free(EMPTY);
}