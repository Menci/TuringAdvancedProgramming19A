#include "form_press.h"
#include <stdio.h>

static int N , K ;
static void *fm , *fd , *da ;

void press_go( int n , int k , void *data , void *form , void *fdata ){
	N = n , K = k ;
	fm = form , fd = fdata , da = data ;
	dfs( 0 ) ;
	char *cf = form ;
	for( int i = 0 , pt = 0 , cnt = 0 ; i < 2 * n ; i ++ ){
		*( cf + pt ) |= ( ( *( cf + i ) ) & 1 ) << cnt ;
		cnt ++ ;
		if( cnt == 8 ){
			pt ++ , *( cf + pt ) = 0 ;
			cnt = 0 ;
		}
	}
}

static void dfs( int u ){
	void *pu = da + u * ( 2 * sizeof( int ) + K ) ;

	int Lson = *((int*)pu) , Rson = *((int*)( pu + sizeof(int) )) ;
	*((char*)fm) = ( Lson != -1 ) , fm ++ ;
	*((char*)fm) = ( Rson != -1 ) , fm ++ ;
	memcpy( fd , pu + 2 * sizeof( int ) , K ) , fd += K ;

//	printf( "L%d\n" , Lson != -1 ? 1 : 0 ) ;
//	printf( "R%d\n" , Rson != -1 ? 1 : 0 ) ;

	if( Lson != -1 ) dfs( Lson ) ;
	if( Rson != -1 ) dfs( Rson ) ;

}
