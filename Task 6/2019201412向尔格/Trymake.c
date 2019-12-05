#include "Trymake.h"

struct node * Init(int n) {
	struct node * Head, * Last;
	Head = malloc ( sizeof(struct node *) );
	Head -> Pre = Head, Head -> Next = Head, Head -> id = 1;
	Last = Head;

	for (int i = 2; i <= n; ++i) {
		struct node * Temp = malloc ( sizeof(struct node *) );
		Temp -> Pre = Last;
		Temp -> Next = Last -> Next;
		Last -> Next = Temp;
		Temp -> id = i;
		Last = Temp;
	}
	Head -> Pre = Last;

	return Head;
}

void Delete (struct node * Temp) {
	printf ("%d ", Temp -> id);
	Temp -> Pre -> Next = Temp -> Next;
	Temp -> Next -> Pre = Temp -> Pre;
	free (Temp); Temp = NULL;
}
