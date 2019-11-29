#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	freopen("input.in", "w", stdout);
	srand(time(0));
	printf("100000\n");
	for (int i = 1; i <= 100000; ++i) {
		printf("%d\n", rand() % 1000 + 1);
	}
}

