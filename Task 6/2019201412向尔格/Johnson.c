#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Trymake.h"

int main() {
	int n, k;
	scanf ("%d%d", &n, &k);
	struct node * Head = Init(n), * Temp;

	while (n) {
		Temp = Head;
		for (int i = 1; i <= k - 1; ++i) {
			Temp = Temp -> Next;
		}

		Head = Temp -> Next;
		Delete (Temp);
		-- n;
	}
	printf ("\n");

	return 0;
}
