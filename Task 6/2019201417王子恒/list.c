//op = 0 Ë³Ê±Õë£¬op = 1 ÄæÊ±Õë 
#include <stdlib.h>

typedef struct list {
	int v;
	struct list *l, *r;
} list;
typedef list* node;
node def_node(int v) {
	node x = (node) malloc(sizeof(list));
	x -> v = v;
	x -> l = x;
	x -> r = x;
	return x;
}
node def_list(int n) {
	int i;
	node start = def_node(1);
	node pre = start;
	for(i = 2; i <= n; ++i) {
		node x = def_node(i);
		x -> l = pre;
		pre -> r = x;
		pre = x;
	}
	start -> l = pre;
	pre -> r = start;
	return start;
}
node del_node(node x, int op) {
	node pre = x -> l, nxt = x -> r;
	pre -> r = nxt;
	nxt -> l = pre;
	free(x);
	return (!op) ? nxt : pre;
}
node find(node s, int k, int op){
	if(!op)
		while(k--) s = s -> r; 
	else
		while(k--) s = s -> l;
	return s;
}
int get_v(node x){
	return x -> v;
}
