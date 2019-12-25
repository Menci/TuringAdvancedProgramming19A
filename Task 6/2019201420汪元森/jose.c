#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "head_lst.h"

Node *find_kpre( Node *nd , int k ) ;
Node *find_knxt( Node *nd , int k ) ;
void LST_init( LST *x , int len ) ;
void Node_del( Node *nd ) ;

static int N , K , tow ;

int vis[1005] ;
static void solve( Node *nd ){
	memset( vis , 0 , sizeof( vis ) ) ;
	for( int i = 1 ; i <= N ; i ++ ){
		if( !tow ) nd = find_kpre( nd , K ) ;
		else nd = find_knxt( nd , K ) ;
		Node_del( nd ) ;
		printf( "%d\n" , nd->id ) ;
		if( vis[nd->id] ) puts( "zhengzai rsndm" ) , exit( 265 ) ;
		vis[nd->id] = true ;
		
		nd = tow ? nd->nxt : nd->pre ;
	}
}

int main(){
	LST A ;
	scanf( "%d%d%d" , &N , &K , &tow ) ;
	LST_init( &A , N ) ;
	solve( A.st[1] ) ;
}
