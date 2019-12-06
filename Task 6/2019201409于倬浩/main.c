#include <stdio.h>
#include "josephus.h"
int main(int argc, char* argv[]) {
	int n = -1, m = -1, dir = -1;
	if (argc != 4) {
		printf("Arguments invalid.\nPlease input n, m, dir : \n");
		scanf("%d%d%d", &n, &m, &dir);
	} else {
		n = atoi(argv[1]);
		m = atoi(argv[2]);
		dir = atoi(argv[3]);
	}
	if (n <= 0 || m <= 0 || (dir != 1 && dir != -1)) {
		printf("Illegal input.\n");
		return 0;
	}
	printf("Running game with %d people, kicks the %dth ", n, m);
	if (dir == 1)
		puts("in clockwise direction.");
	else
		puts("in anticlockwise direction.");
	runGame(n, m, dir);
	return 0;
}