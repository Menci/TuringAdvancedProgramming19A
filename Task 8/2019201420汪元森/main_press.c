#include "read_bin.h"
#include "form_press.h"
#include "write_bin.h"

#include <stdio.h>
#include <string.h>

int N , K ;
char *fname = "1.bin" ;
char read_data[7300000] , form[600005] , fdata[6400005] ;

int stoi( char *a ){
	int rt = 0 , pt = 0 ;
	while( ( a[pt] < '0' || a[pt] > '9' ) && a[pt] != '\0' ) pt ++ ;
	if( a[pt] == '\0' ) exit( 252 ) ;
	while( a[pt] >='0' && a[pt] <='9' ) rt = rt * 10 + a[pt] - '0' , pt ++ ;
	return rt ;
}

int main( int argc , char **argv ){
	if( argc == 1 ) puts("no arg!") , exit( 253 ) ;

	N = stoi( argv[2] ) , K = stoi( argv[3] ) ;
//	fprintf( stderr , "%d %d" , N , K ) ;
	read_to_arr( read_data , argv[1] ) ;
	press_go( N , K , read_data , form , fdata ) ;
	write_to_bin( form , N / 4 , fdata , N * K ) ;
}
