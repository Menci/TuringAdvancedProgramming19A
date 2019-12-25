#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "head_lst.h"

int min_int( int a , int b ){
	return a < b ? a : b ;
}

void LST_init( LST *x , int len ){
	x->cnt = sqrt( len ) , x->siz = len / x->cnt ;
	if( x->siz * x->cnt < len ) x->siz ++ ;

	x->nd = malloc( len * sizeof( Node ) ) ;
	x->st = malloc( ( x->cnt + 5 ) * sizeof( void* ) ) ;
	x->Lpos = malloc( ( x->cnt + 5 ) * sizeof( int ) ) ;
	x->Rpos = malloc( ( x->cnt + 5 ) * sizeof( int ) ) ;
	x->nd[0] = ( Node ){ &x->nd[len-1] , &x->nd[1] , 1 } ;
	for( int i = 1 ; i < len - 1 ; i ++ )
		x->nd[i] = ( Node ){ &x->nd[i-1] , &x->nd[i+1] , i + 1 } ;
	x->nd[len-1] = ( Node ){ &x->nd[len-2] , &x->nd[0] , len } ;

	x->Lpos[0] = 0 ;
	for( int i = 1 ; i <= x->cnt ; i ++ ){
		x->Lpos[i] = x->Lpos[i-1] + 1 ;
		x->Rpos[i] = min_int( x->Lpos[i] + x->siz - 1 , len ) ;
		x->st[i] = &x->nd[ x->Lpos[i]-1 ] ;
	}
}

void kill_LST( LST *x ){
	free( x->nd ) ;
	free( x->st ) ;
	free( x->Lpos ) ;
	free( x->Rpos ) ;
}

void Node_del( Node *nd ){
	nd->pre->nxt = nd->nxt ;
	nd->nxt->pre = nd->pre ;
}

Node *find_knxt( Node *nd , int k ){
	while( k ) nd = nd->nxt , k -- ;
	return nd ;
}

Node *find_kpre( Node *nd , int k ){
	while( k ) nd = nd->pre , k -- ;
	return nd ;
}
