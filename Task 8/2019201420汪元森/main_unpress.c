#include "read_bin.h"
#include "unpress.h"

#include <stdio.h>
#include <string.h>

char read_data[7300000] , fir_data[6500000] ;
char *fname = "pressed" ;

int main(){
	read_to_arr( read_data , fname ) ;//test makefile
	unpress( read_data , fir_data ) ;
}

