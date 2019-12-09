#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "splay_tree.h"

splay *qwq ;

void solve(){
	Node *nd ;
	for( int i = 1 , typ , x ; i <= qwq->N ; i ++ ){
		scanf( "%d%d" , &typ , &x ) ;
		if( typ == 0 ) Insert( qwq , x ) ;
		else if( typ == 1 ) Erase( qwq , x ) ;
		else if( typ == 2 ) printf( "%d\n" , Kth( qwq , x )->val ) ;
		else if( typ == 3 ) printf( "%d\n" , Less_cnt( qwq , x ) ) ;
		else if( typ == 4 ){
			nd = Find_pre( qwq->root , x ) ;
			printf( "%d\n" , nd ? nd->val : -1 ) ;
		} else if( typ == 5 ){
			nd = Find_nxt( qwq->root , x ) ;
			printf( "%d\n" , nd ? nd->val : -1 ) ;
		}
	} dfs_clear( qwq->root ) ;
}

int main(){
	qwq = malloc( sizeof( splay ) ) ;
	scanf( "%d" , &qwq->N ) ;
	solve() ;
}
