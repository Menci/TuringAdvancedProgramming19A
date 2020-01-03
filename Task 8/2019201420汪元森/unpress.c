#include "unpress.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static int pt , ws , paim , cnt , N , K ;
char *fd , *fm ;

void dfs_opre( void *aim ){
	bool Lson , Rson ;
	int this_id = cnt ++ ;
	Lson = ( ( *( fm + pt ) ) >> ws ) & 1 , ws ++ ;
	Rson = ( ( *( fm + pt ) ) >> ws ) & 1 , ws ++ ;
	if( ws == 8 ) pt ++ , ws = 0 ;
	
//	printf( "L%d\n" , Lson ? 1 : 0 ) ;
//	printf( "R%d\n" , Rson ? 1 : 0 ) ;

	if( Lson ) dfs_opre( aim ) ;
	memcpy( aim + paim , fd + this_id * K , K ) , paim += K ;
	if( Rson ) dfs_opre( aim ) ;
}

void unpress( void *da , void *aim ){
	N = *((int*)da) , K = *( ((int*)da)+1 ) ;
	fm = da + 2 * sizeof( int ) ;
	fd = da + 2 * sizeof( int ) + N / 4 ;

	cnt = pt = paim = ws = 0 ;
	dfs_opre( aim ) ;
	
	FILE *fp = fopen( "pre_order" , "w" ) ;
/*		for( int i = 0 ; i < N ; i ++ ){
		for( int j = 0 ; j < K ; j ++ )
			fprintf( fp , "%03d" , *((char*)aim+i*K+j) ) ;
		fprintf( fp , "\n" ) ;
	}
*/	fwrite( aim , 1 , N * K , fp ) ;
	fclose( fp ) ;
}
