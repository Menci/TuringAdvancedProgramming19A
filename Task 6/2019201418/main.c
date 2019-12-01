#include<stdio.h>
#include<stdlib.h>
extern void init();
extern void In();
extern void Move();
extern void Prt();
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
