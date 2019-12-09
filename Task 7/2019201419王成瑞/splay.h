#ifndef SPLAY_H_

#define SPLAY_H_

typedef struct splayNode {
	struct splayNode *son[2], *fa;
	int val, siz, cnt;
} splayNode;

splayNode* newNode(int val);
void insert(splayNode **root, splayNode *o, int val);
void erase(splayNode **root, int val);
int getKth(splayNode *o, int rank);
int getRank(splayNode *o, int val);
int getNumber(splayNode **root, int val, int f);
void clear(splayNode *o);

#endif