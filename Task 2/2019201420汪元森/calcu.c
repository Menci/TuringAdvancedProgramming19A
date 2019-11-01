#include "myDouble_io.h"
#include <time.h>

const int prior[7] = { 0 , 1 , 1 , 2 , 2 , -1 , -2 } ;
md num_sta[10000005] ;
int op_sta[10000005] , num_topp , op_topp , sgn_topp , st_sta[10000005] , st_topp ;
bool sgn_LB[10000005] ;

void lib_init(){
	myDouble_lib_init() ;
	myIO_init() ;
}

void op_stack_init(){
	st_topp = sgn_topp = num_topp = op_topp = 0 ;
}

void push_number( md x ){
	num_sta[++num_topp] = x ;
}

void push_option( int op ){
	static md tmp;
	int now_prior = prior[op] ;
	if( op == 5 ){
		while( op_topp && op_sta[op_topp] <= 4 ){
			if( num_topp - st_sta[st_topp] == 1 ) break ;
			tmp = cal_opt( num_sta[num_topp-1] , num_sta[num_topp] , op_sta[op_topp] ) ;
			num_sta[--num_topp] = tmp , op_topp -- ;
		} op_sta[ ++op_topp ] = op ;
		st_sta[++st_topp] = num_topp ;
	}
	
	else if( op == 6 ){
		while( op_topp && op_sta[op_topp] <= 4 ){
			tmp = cal_opt( num_sta[num_topp-1] , num_sta[num_topp] , op_sta[op_topp] ) ;
			num_sta[--num_topp] = tmp , op_topp -- ;
		} op_topp -- , st_topp -- ;
		if( sgn_LB[ sgn_topp ] ) num_sta[num_topp] = rev_sign( num_sta[num_topp] ) ;
		sgn_topp -- ;
	}

	else {
		while( op_topp && prior[ op_sta[op_topp] ] >= now_prior ){
			tmp = cal_opt( num_sta[num_topp-1] , num_sta[num_topp] , op_sta[op_topp] ) ;
			num_sta[--num_topp] = tmp , op_topp -- ;
		} op_sta[++op_topp] = op ;
	}
}

void solve(){
	static bool sgn , prev_opt ;
	static int read_code ;
	static md double_x ;

	prev_opt = 1 , sgn = 0 ;
	push_option( 5 ) ;

	while( 1 ){
		read_code = read_in() ;
		if( read_code == -1 ) break ;
		if( read_code == 0 ){
			double_x = string_to_myDouble( char_in , sgn ) ;
			push_number( double_x ) ;
			prev_opt = 0 , sgn = 0 ;
		} else {
			if( prev_opt ){
				if( read_code > 2 ){
					push_option( read_code ) ;
					if( read_code == 5 ) sgn_LB[++sgn_topp] = sgn , sgn = 0 ;
				} else sgn ^= ( read_code == 2 ) ; 
			} else push_option( read_code ) ;
			prev_opt = ( read_code != 6 ) ;
		}
	} push_option( 6 ) ;
	print( num_sta[1] ) ; puts( "" ) ;
}

int main(){
	lib_init() ;

	while( read_in_line() != 255 ){
			op_stack_init() ;
			solve() ;
	}
	fprintf( stderr , "%ld" , clock() ) ;
}
