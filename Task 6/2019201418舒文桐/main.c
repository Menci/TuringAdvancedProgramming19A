#include<stdio.h>
#include<stdlib.h>
#include "Leaves.h"

int main()
{
	init();
	In();
	extern int n ;
	int i;
	for( i = 1 ; i <= n-1  ; i++ )
	{
		Move();
	}
	Prt();
	return 0;
}
