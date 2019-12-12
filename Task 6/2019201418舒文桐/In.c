#include<stdio.h>
#include "Leaves.h"

void In();
int type , n , k ;




void In()
{
	int i;
	scanf( "%d" , &type );
	scanf( "%d%d" , &n , &k );
	for( i = 1 ; i <= n ; i++) CreatNew( i );
	Endlink();
}
