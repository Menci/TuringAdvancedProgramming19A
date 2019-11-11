#include <stdio.h>
#include "my_qsort.h"
int example_cmp(const void *a, const void *b) {
	return (*((int *)a)) < (*((int *) b)) ? -1 : 
		   (*((int *)a)) == (*((int *) b)) ? 0 : 1;
}

int main() {
	int s[] = { 9, 9, 8, 2, 4, 4, 3, 5, 3};
	Qsort(s, sizeof(s) / sizeof(s[0]), sizeof(s[0]), example_cmp);
	for(int i = 0; i < sizeof(s) / sizeof(s[0]); ++i) 
		printf("%d\n", s[i]);
	return 0;
}