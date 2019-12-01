//op = 0 À≥ ±’Î£¨op = 1 ƒÊ ±’Î 
#include <stdio.h>

typedef struct list* node;
extern node find(node, int, int);
extern int get_v(node);
extern node def_list(int);
extern node del_node(node, int);

int main() {
	int n, k, op;
	scanf("%d%d%d", &n, &k, &op);
	node s = def_list(n);
	while(n--) {
		s = find(s, k, op);
		printf("%d", get_v(s));
		if(n)putchar(' ');
		s = del_node(s, op); 
	}
	putchar('\n');
	return 0;
}
