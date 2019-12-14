#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main() {
	void *p = malloc(233);
	p -= 8;
	printf("%lld\n", (*(size_t *)(p)));
}