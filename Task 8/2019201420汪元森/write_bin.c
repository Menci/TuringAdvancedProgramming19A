#include "write_bin.h"

void write_to_bin( void *fm , int sfm , void *fd , int sfd ){
	FILE *fp = fopen( "pressed" , "wb" ) ;
	int N = sfm * 4 , K = sfd / N ;
	fwrite( &N , sizeof( int ) , 1 , fp ) ;
	fwrite( &K , sizeof( int ) , 1 , fp ) ;
	fwrite( fm , 1 , sfm , fp ) ;
	fwrite( fd , 1 , sfd , fp ) ;
	fclose( fp ) ;
}
