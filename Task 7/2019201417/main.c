#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "splay.h"

int read() {
    int X = 0, w = 0;
    char ch = 0;
    while (!isdigit(ch)) {
        w |= ch == '-';
        ch = getchar();
    }
    while (isdigit(ch)) X = (X << 3) + (X << 1) + (ch ^ 48), ch = getchar();
    return w ? -X : X;
}
int main() {
	int n = read();
	while (n--) {
		int op = read(), k = read();
		if (op == 0) ins_node(k);
		if (op == 1) del_node(k);
		if (op == 2) printf("%d\n", find_kth(k));
		if (op == 3) printf("%d\n", smaller_than_x(k));
		if (op == 4) printf("%d\n", max_num_smaller_than_x(k));
		if (op == 5) printf("%d\n", min_num_bigger_than_x(k));
	}
	del_balanced_tree();
	return 0;
}
