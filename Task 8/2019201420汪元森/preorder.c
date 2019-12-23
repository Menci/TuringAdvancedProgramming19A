#include "preorder.h"
#include <stdio.h>
#include <string.h>

static int N , K ;
static void *fd , *da ;

void preorder( int n , int k , void *data , void *aim ){
	N = n , K = k ;
	fd = aim , da = data ;
	dfs( 0 ) ;
	FILE *fp = fopen( "preorder" , "wb" ) ;
/*	for( int i = 0 ; i < N ; i ++ ){
		for( int j = 0 ; j < K ; j ++ )
			fprintf( fp , "%03d" , *((char*)aim+i*K+j) ) ;
		fprintf( fp , "\n" ) ;
	}
*/	fwrite( aim , 1 , N * K , fp ) ;
	fclose( fp ) ;
}

static void dfs( int u ){
	void *pu = da + u * ( 2 * sizeof( int ) + K ) ;

	int Lson = *((int*)pu) , Rson = *((int*)( pu + sizeof(int) )) ;

//	printf( "L%d\n" , Lson != -1 ? 1 : 0 ) ;
//	printf( "R%d\n" , Rson != -1 ? 1 : 0 ) ;

	if( Lson != -1 ) dfs( Lson ) ;

	memcpy( fd , pu + 2 * sizeof( int ) , K ) , fd += K ;

	if( Rson != -1 ) dfs( Rson ) ;

}
