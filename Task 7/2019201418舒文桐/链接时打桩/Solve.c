#include "Init.h"

void Solve()
{
	int n,op,x;
	scanf( "%d" , &n );
	int i;
	for( i = 1 ; i <= n ; i++ )
	{
		scanf( "%d%d" , &op , &x );
		switch( op )
		{
			case 0: insert( x ); break;
			case 1: del( x ); break;
			case 2: printf( "%d\n" , kth(x) ); break;
			case 3: printf( "%d\n" , rank(x) ); break;
			case 4: printf( "%d\n" , pre(x) ); break;
			case 5: printf( "%d\n" , nxt(x) ); break;
		}
	}
}
