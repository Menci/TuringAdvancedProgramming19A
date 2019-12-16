#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "splay.h"

static node *_root;

void initSet(node *cur)
{
	cur -> cnt = cur -> val = cur -> size = 0;
	cur -> ch[0] = cur -> ch[1] = cur -> fa = 0;
}

#define dir(cur) ((cur) -> fa -> ch[1] == (cur))
#define SZ(cur) ((cur) == NULL ? 0 : (cur) -> size)

node* get_root()
{
	return _root;
}

void maintain(node *cur)
{
	cur -> size = SZ(cur -> ch[0]) + SZ(cur -> ch[1]) + cur -> cnt;
}

void rotate(node *cur, int type)
{
	node *anc = cur -> fa;

	anc -> ch[!type] = cur -> ch[type];
	if (cur -> ch[type]) 
		cur -> ch[type] -> fa = anc;
	if (anc -> fa != NULL) 
		anc -> fa -> ch[dir(anc)] = cur;
	cur -> ch[type] = anc;
	cur -> fa = anc -> fa;
	anc -> fa = cur;

	maintain(anc);
	maintain(cur);
}

void splay(node *cur, node *anc)
{
	while (cur -> fa != anc) {
		
		if ((cur -> fa) -> fa == anc) {
			rotate(cur, !dir(cur));
		}
		else {
			if (dir(cur) != dir(cur -> fa)) {
				rotate(cur, !dir(cur));
				rotate(cur, !dir(cur));
			}
			else {
				rotate(cur -> fa, !dir(cur -> fa));
				rotate(cur, !dir(cur));
			}
		}
	}
	if (anc == NULL) 
		_root = cur;
}

node* find(int _val)
{
	if (_root == NULL) {
		fprintf(stderr, "WARNING : EMPTY SPLAY !\n");
		return 0;
	}
	node *cur = _root;
	while (cur -> val != _val) {
		if (cur -> ch[(cur -> val) < _val] == NULL) break;
		cur = cur -> ch[(cur -> val) < _val];
	}
	return cur;
}

//remember to set
void insert(int _val)
{
	if (_root == NULL) {
		node *cur = (node *) malloc(sizeof (node));

		initSet(cur);
		cur -> val = _val;
		cur -> cnt = 1;
		cur -> size = 1;

		_root = cur;
		return ;
	}

	node *cur = find(_val);

	if (cur -> val == _val) {
		cur -> cnt ++;
		splay(cur, 0);
	}
	else {
		node *curNew = (node *) malloc(sizeof (node));

		initSet(curNew);
		curNew -> val = _val;
		curNew -> cnt = 1;
		curNew -> size = 1;

		curNew -> fa = cur;
		cur -> ch[(cur -> val) < _val] = curNew;

		splay(curNew, 0);
	}
}

node* neighbor_ele(int _val, int type)
{
	node *cur = find(_val);
	splay(cur, 0);

	if ((cur -> val < _val) && !type) return cur;
	if ((cur -> val > _val) &&  type) return cur;

	cur = cur -> ch[type];
	if (cur == NULL) return NULL;

	while (cur -> ch[!type] != NULL) {
		cur = cur -> ch[!type];
	}

	if (type) 
		return cur -> val < _val ? NULL : cur;
	else 
		return cur -> val > _val ? NULL : cur;
}

void erase(int _val)
{
	node *cur = find(_val);
	assert(cur -> val == _val);

	if (cur -> cnt > 1) {
		cur -> cnt --;
		splay(cur, 0);
	}
	else {
		node *curLower = neighbor_ele(_val, 0);
		node *curUpper = neighbor_ele(_val, 1);

		if (!curLower && !curUpper) {
			if (cur -> fa != NULL) {
				(cur->fa) -> ch[dir(cur)] = NULL;
			}
			if (_root == cur) _root = NULL;
		}
		else if (curUpper == NULL) {
			splay(cur, 0);
			_root = cur -> ch[0];
			(cur->ch[0]) -> fa = 0;
		}
		else if (curLower == NULL) {
			splay(cur, 0);
			_root = cur -> ch[1];
			(cur->ch[1]) -> fa = 0;
		}
		else {
			splay(curLower, 0);
			splay(curUpper, curLower);
			curUpper -> ch[0] = NULL;
			splay(curUpper, 0);
		}

		free(cur);
	}
}

int find_kth(node *cur, int k)
{
	if (SZ(cur -> ch[0]) < k && SZ(cur -> ch[0]) + (cur -> cnt) >= k) {
		return cur -> val; 
	}
	if (SZ(cur -> ch[0]) >= k) {
		return find_kth(cur -> ch[0], k);
	}
	else {
		return find_kth(cur -> ch[1], k - SZ(cur -> ch[0]) - (cur -> cnt));
	}
}

int get_rank(int _val)
{
	node *cur = find(_val);
	splay(cur, 0);
	return SZ(cur -> ch[0]) + (cur -> val < _val) * cur -> cnt;
}

void cut_tree(node *cur)
{
	if (!cur -> ch[0] && !cur -> ch[1]) {
		free(cur);
		return ;
	}
	if (cur -> ch[0]) cut_tree(cur -> ch[0]);
	if (cur -> ch[1]) cut_tree(cur -> ch[1]);
	free(cur);
}

#undef SZ
#undef dir

