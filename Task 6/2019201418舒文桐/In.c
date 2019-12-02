#ifndef In_p
#define In_p

#include<stdio.h>

void In();
int type , n , k ;
extern void CreatNew( int data );
extern void Endlink();
#endif


void In()
{
	int i;
	scanf( "%d" , &type );
	scanf( "%d%d" , &n , &k );
	for( i = 1 ; i <= n ; i++) CreatNew( i );
	Endlink();
}
