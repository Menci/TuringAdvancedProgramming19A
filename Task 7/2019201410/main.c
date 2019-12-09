#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "splay.h"


void clean(node *operant) {
    if(operant == NULL) return;
    if(operant -> left != NULL)
        clean(operant -> left);
    if(operant -> right != NULL)
        clean(operant -> right);
    free(operant);
}

void print(node *operant) {
    if(operant == NULL) return;
    print(operant -> left);
    printf("%d\n", operant -> value);
    print(operant -> right);
}


int main() {

#ifndef ONLINE_JUDGE
	//freopen("splay.in", "r", stdin);
	//freopen("splay.out", "w", stdout);
#endif
	int n;
	scanf("%d", &n);
    

	int option = 0, operant;
	for(int i = 1; i <= n; i++) {
		scanf("%d%d", &option, &operant);
		switch(option) {

			case INSERT:
				Insert(operant, Head, NULL);
				break;
			case DELETE:
				delete(operant, Head);
				break;
			case FIND_KTH:
				printf("%d\n", find_kth(operant, Head));
				break;
			case FIND_TOTAL_MIN:
				printf("%d\n", find_total_min(operant, Head));
				break;
			case FIND_MIN:
				printf("%d\n", find_min(operant, Head));
				break;
			case FIND_MAX:
				printf("%d\n", find_max(operant, Head));
				break;

		}
	}

	clean(Head);
	return 0;
}
