#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "linkedlist.h"

struct Data *newData(int id);
struct linkedList *newLinkedList(struct Data *data);
void initMem(int n,size_t size);
void clearMem();

struct linkedList *root;
void init(int n) {
	root=newLinkedList(newData(1));
	struct linkedList* cur=root;
	for(int i=2;i<=n;i++) {
		cur=cur->add(cur, newData(i));
	}
}
void solveClockwise(int n,int k) {
	struct linkedList *cur=root;
	while(cur) {
		for(int i=1;i<=k;i++,cur=cur->nxt);
		printf("%d ",cur->data->id);
		cur=cur->delete(cur);
	}
}
void solveAntiClockwise(int n,int k) {
	struct linkedList *cur=root;
	while(cur) {
		for(int i=1;i<=k;i++,cur=cur->pre);
		printf("%d ",cur->data->id);
		cur=cur->delete(cur);
		if(cur) cur=cur->pre;
	}
}
int main(int argc, char *argv[]) {
	int n,k,type;
	scanf("%d%d%d",&n,&k,&type);
	initMem(n,sizeof(struct linkedList)+sizeof(struct Data));
	init(n);
	if(type) solveClockwise(n, k);
	else solveAntiClockwise(n, k);
	clearMem();
}