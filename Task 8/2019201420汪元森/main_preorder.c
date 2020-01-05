#include "read_bin.h"
#include "preorder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N , K ;
char *fname = "1.bin" ;
char read_data[7300000] , fdata[6400005] ;

int stoi( char *a ){
	int rt = 0 , pt = 0 ;
	while( ( a[pt] < '0' || a[pt] > '9' ) && a[pt] != '\0' ) pt ++ ;
	if( a[pt] == '\0' ) exit( 252 ) ;
	while( a[pt] >='0' && a[pt] <='9' ) rt = rt * 10 + a[pt] - '0' , pt ++ ;
	return rt ;
}

int main( int argc , char **argv ){
	if( argc == 1 ) puts( "no arg!" ) , exit( 253 ) ;

	N = stoi( argv[2] ) , K = stoi( argv[3] ) ;
	read_to_arr( read_data , argv[1] ) ;
	preorder( N , K , read_data , fdata ) ;
}
