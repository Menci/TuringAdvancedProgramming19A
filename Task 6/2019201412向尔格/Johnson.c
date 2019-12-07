#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Trymake.h"

int main(int argc, char **argv) {
	int n, k, tp;
	n = atoi(argv[1]), k = atoi(argv[2]), tp = atoi(argv[3]);
	struct node * Head = Init(n), * Temp;

	while (n) {
		Temp = Head;
		for (int i = 1; i <= k; ++i) {
			if (tp) Temp = Temp -> Next;
			else Temp = Temp -> Pre;
		}

		if (tp) Head = Temp -> Next;
		else Head = Temp -> Pre;
		Delete (Temp);
		-- n;
	}
	printf ("\n");

	return 0;
}
