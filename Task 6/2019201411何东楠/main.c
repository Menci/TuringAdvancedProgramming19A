#include <stdio.h>
#include <stdlib.h>
#include "Dlinked_list.h"
#include "joseph_worker.h"

int main(int argc, char **argv)
{
	int n = 0, m = 0, type = 1;

	if (argc <= 1) {
		printf("Invalid input!\n");
		return 0;
	}
	if (argc > 4) {
		printf("warning : too many arguments!\n");
	}

	n = atoi(argv[1]);
	if (argc >= 3) {
		m = atoi(argv[2]);
		if (argc >= 4) type = atoi(argv[3]);
	}

	node *head = init(n);
	work(m, type, head);

	return 0;
}

