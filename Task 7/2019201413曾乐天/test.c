#include<stdlib.h>
#include<malloc.h>

int main()
{
	int *ptr = malloc(sizeof(int));
	free(ptr);
	//putchar('x');
	return 0;
}
