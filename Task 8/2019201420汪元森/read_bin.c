#include "read_bin.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

long long get_file_bytes( int fid ){ // file descriptor
	static struct stat stat_temp ;
	fstat( fid , &stat_temp ) ;
	return stat_temp.st_size ;
}

void read_to_arr( char *aim , char *fname ){
	static FILE *fp ;
	static long long fbytes , rbytes ;//asdkjhas

	fp = fopen( fname , "r" ) ;
	fbytes = get_file_bytes( fileno( fp ) ) ;
	rbytes = fread( (void*)aim , 1 , fbytes , fp ) ;

	if( rbytes != fbytes ){
		fprintf( stderr , "error : file read : read len %s file len" , rbytes > fbytes ? "bigger than" : "smaller than" ) ;
		exit( 255 ) ;
	}

}

