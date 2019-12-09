// fill sth
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "splay_tree.h"

static Node *root ;
const int Node_size = sizeof( Node ) ;
void setRoot( Node *x ){ root = x ; }

Node *newNode( Node **nd , Node *fa , int val ){
	*nd = malloc( Node_size ) ;
	(**nd) = ( Node ){ {NULL , NULL} , fa , 1 , 1 , val } ;
	return *nd ;
}

void update( Node *this ){
	this->siz = this->cnt ;
	if( this->ch[0] ) this->siz += this->ch[0]->siz ;
	if( this->ch[1] ) this->siz += this->ch[1]->siz ;
}

void Rotate( Node *nd , bool aim ){
	Node *x = nd->ch[aim] ;
	if( nd->fa ){
		if( nd->fa->ch[0] == nd ) nd->fa->ch[0] = x ;
		else if( nd->fa->ch[1] == nd ) nd->fa->ch[1] = x ;
	} x->fa = nd->fa ;
	if( x->ch[aim^1] ){
		x->ch[aim^1]->fa = nd ;
	} nd->ch[aim] = x->ch[aim^1] ;
	x->ch[aim^1] = nd , nd->fa = x ;
	update( nd ) ; update( x ) ;
}

void Splay( Node *nd , Node *aim ){
	while( nd->fa != aim ){
		Node *fa = nd->fa , *gdfa = fa->fa ;
		bool pn = ( fa->ch[1] == nd ) , pf ;
		if( gdfa && fa->fa != aim ){
			pf = ( gdfa->ch[1] == fa ) ;
			if( pn == pf ) Rotate( gdfa , pf ) , Rotate( fa , pn ) ;
			else Rotate( fa , pn ) , Rotate( gdfa , pf ) ;
		} else Rotate( fa , pn ) ;
	} if( !aim ) root = nd ;
}

Node *Insert_root( Node *nd , int val ){
	while( nd ){
		nd->siz ++ ;
		if( nd->val == val ){ nd->cnt ++ ; return nd ; }
		bool aim = ( nd->val < val ) ;
		if( nd->ch[aim] ) nd = nd->ch[aim] ;
		else return newNode( &( nd->ch[aim] ) , nd , val ) ;
	}
}

void Insert_r( int val ){
	if( !root ) root = newNode( &root , NULL , val ) ;
	else Splay( Insert_root( root , val ) , NULL ) ;
}

void Insert( splay *x , int val ){
	setRoot( x->root ) ;
	Insert_r( val ) ;
	x->root = root ;
}

Node *Find( Node *nd , int val ){
	while( nd ){
		if( nd->val == val ) return nd ;
		nd = nd->ch[ nd->val<val ] ;
	}
}

Node *Find_pre( Node *nd , int val ){
	Node *rt = NULL ;
	while( nd ){
		if( nd->val < val ) rt = nd , nd = nd->ch[1] ;
		else nd = nd->ch[0] ;
	} return rt ;
}

Node *Find_nxt( Node *nd , int val ){
	Node *rt = NULL ;
	while( nd ){
		if( nd->val > val ) rt = nd , nd = nd->ch[0] ;
		else nd = nd->ch[1] ;
	} return rt ;
}

void Erase_r( int val ){
	Node *nd = Find( root , val ) , *pre ;
	Splay( nd , NULL ) ;
	if( nd->cnt > 1 ) { nd->cnt -- , nd->siz -- ; return ; }
	
	if( ( pre = Find_pre( root , val ) ) != NULL ){
		Splay( pre , nd ) , root = pre ;
		if( nd->ch[1] ) {
			nd->ch[1]->fa = root ;
			root->ch[1] = nd->ch[1] ;
			root->siz += root->ch[1]->siz ;
		}
	} else root = nd->ch[1] ;
	if( root ) root->fa = NULL ;
	free( nd ) ;
}

void Erase( splay *x , int val ){
	setRoot( x->root ) ;
	Erase_r( val ) ;
	x->root = root ;
}

Node *Kth_r( int K ){
	Node *nd = root ;
	while( nd ){
		int Lsiz = ( nd->ch[0] ? nd->ch[0]->siz : 0 ) ;
		if( K <= Lsiz ) nd = nd->ch[0] ;
		else if( K <= Lsiz + nd->cnt ) return nd ;
		else K -= Lsiz + nd->cnt , nd = nd->ch[1] ;
	}
}

Node *Kth( splay *x , int K ){
	setRoot( x->root ) ;
	Node *rt = Kth_r( K ) ;
	x->root = root ;
	return rt ;
}

int Less_cnt_r( int val ){
	Node *nd = Find_pre( root , val ) ;
	if( !nd ) return 0 ;
	Splay( nd , NULL ) ;
	return ( nd->ch[0] ? nd->ch[0]->siz : 0 ) + nd->cnt ;
}

int Less_cnt( splay *x , int val ){
	setRoot( x->root ) ;
	int rt = Less_cnt_r( val ) ;
	x->root = root ;
	return rt ;
}

void dfs_clear( Node *nd ){
	if( !nd ) return ;
	dfs_clear( nd->ch[0] ) ;
	dfs_clear( nd->ch[1] ) ;
	free( nd ) ;
}


void print_tree( Node *nd ){
	if( !nd ) return ;
	printf( "%d(%d)[%d]-->" , nd->val , nd->cnt , nd->siz ) ;
	if( nd->ch[0] ) printf( "L %d " , nd->ch[0]->val ) ;
	if( nd->ch[1] ) printf( "R %d " , nd->ch[1]->val ) ;
	puts( "" ) ;
	print_tree( nd->ch[0] ) ;
	print_tree( nd->ch[1] ) ;
}

