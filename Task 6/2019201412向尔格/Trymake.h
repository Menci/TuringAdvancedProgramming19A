#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct node {
	int id;
	struct node * Pre;
	struct node * Next;
};

struct node * Init(int n);
void Delete (struct node * Temp);
