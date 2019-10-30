#include "myDouble.h"

int buf_len , global_buf_pt ;
const unsigned int B10_MAX = 1000000000 ;
const int B10_WIDTH = 9 , B10_LEN = 36 ;
char buf[400000005] , char_in[330] , char_out[330] ;
const char cNAN[10] = " NAN" , cINF[10] = " INF" , c_INF[10] = " -INF" ;

typedef struct b10_Data{
	ull data_[36] ;
} b10 ;

int len_2K[1050] , b10_search_blo[1005] ;
b10 CONST_2K[1050] ;

int read_in_line(){
	fflush( stdout ) ;
	static char c ;

	global_buf_pt = buf_len = 0 ;
	c = getchar() ;
	while( c != '\n' && c != '\r' && c != EOF ) buf[ ++buf_len ] = c , c = getchar() ;
	buf[ buf_len+1 ] = '\0' ;
	return c == EOF ? 255 : 0 ;
}

int data_kind( char c ){
	if( c == '\0' ) return -1 ;
	if( ( '0' <= c && c <= '9' ) || c == '.' ) return 0 ;
	if( c == '+' ) return 1 ;
	if( c == '-' ) return 2 ;
	if( c == '*' ) return 3 ;
	if( c == '/' ) return 4 ;
	if( c == '(' ) return 5 ;
	if( c == ')' ) return 6 ;
	return 255 ;
}

int read_in(){
	static char c ;
	static int d_kind , pt ;
	
	c = buf[ ++global_buf_pt ] ;
	while( data_kind( c ) == 255 ) c = buf[ ++global_buf_pt ] ;

	d_kind = data_kind( c ) ;
	if( d_kind != 0 ) return d_kind ;
	
	pt = 0 ;
	while( data_kind( c ) == 0 ) char_in[ ++pt ] = c , c = buf[ ++global_buf_pt ] ;
	global_buf_pt -- , char_in[ pt+1 ] = '\0' ;
	return 0 ;
}

const uint pow10_[10] = { 1 , 10 , 100 , 1000 , 10000 , 100000 , 1000000 , 10000000 , 100000000 , 1000000000 } ;
int b10_flo_10( b10 *x , int len ){
	static int blo ;
	blo = b10_search_blo[len] ;

	for( int i = blo ; i >= 0 ; i -- ) if( x->data_[i] ){
		for( int j = 0 ; j < B10_WIDTH ; j ++ )
			if( x->data_[i] < pow10_[j+1] ) return i * B10_WIDTH + j ;
	} return -1 ;
}

int b10_div_2( b10 *a , int len ){
	static int rt ;
	for( int i = b10_search_blo[len] ; i > 0 ; i -- ){
		if( a->data_[i] & 1 ) a->data_[i-1] += B10_MAX ;
		a->data_[i] >>= 1 ;
	} rt = ( a->data_[0] & 1 ) , a->data_[0] >>= 1 ;
	return rt ;
}

void b10_add( b10 *a , b10 b ){
	for( int i = 0 ; i < B10_LEN ; i ++ ){
		a->data_[i] += b.data_[i] ;
		if( a->data_[i] >= B10_MAX ) a->data_[i+1] ++ , a->data_[i] -= B10_MAX ;
	}
}

bool b10_mul_2( b10 *x , int len ){
	static int blo ;
	static bool rt ;
	rt = true ;
	blo = b10_search_blo[len] ;
	for( int i = 0 ; i <= blo ; i ++ ) x->data_[i] <<= 1 ;
	for( int i = 0 ; i <= blo ; i ++ ) if( x->data_[i] ){
		if( x->data_[i] >= B10_MAX ) x->data_[i] -= B10_MAX , x->data_[i+1] ++ ;
		rt = false ;
	} return rt ;
}

b10 b10_mul_x( b10 *x , int len , ull fac ){
	static int blo ;
	static b10 rt ;

	rt = *x , blo = b10_search_blo[len] ;
	for( int i = 0 ; i <= blo ; i ++ ) rt.data_[i] *= fac ;
	for( int i = 0 ; i <= blo ; i ++ ) if( rt.data_[i] >= B10_MAX )
		rt.data_[i+1] += rt.data_[i] / B10_MAX , rt.data_[i] %= B10_MAX ;
	return rt ;
}

void b10_mul_2k( b10 *x , int exp ){
	static int blo , tmp_len ;
	static b10 res , tmp ;
	blo = b10_search_blo[ b10_flo_10( x , B10_WIDTH * B10_LEN - 1 ) ] ;
	
	memset( &res , 0 , sizeof( res ) ) ;
	tmp = CONST_2K[exp] , tmp_len = len_2K[exp] ;
	for( int i = 0 ; i <= blo ; i ++ ){
		b10_add( &res , b10_mul_x( &tmp , tmp_len , x->data_[i] ) ) ;
		tmp = b10_mul_x( &tmp , tmp_len , B10_MAX ) , tmp_len += B10_WIDTH ;
	} *x = res ;
}

void b10_mul_2base( b10 *x , int len , ull fac ){
	int blo = search_block[len] ;
	for( int i = 0 ; i <= blo ; i ++ ) x->data_[i] *= fac ;
	for( int i = 0 ; i <= blo ; i ++ ) if( x->data_[i] >= BIT_X_MAX ){
		x->data_[i+1] += x->data_[i] / BIT_X_MAX , x->data_[i] %= BIT_X_MAX ;
	}
}

bool b10_del_highest_one( b10 *x , int pos ){
	static int blo ;
	blo = b10_search_blo[pos] , pos -= blo * B10_WIDTH ;
	if( x->data_[blo] >= pow10_[pos] ){
		x->data_[blo] -= pow10_[pos] ;
		return true ;
	} return false ;
}

ull b10_del_highest_num__2base( b10 *x , int pos ){
	static int blo ;
	static ull rt ;
	blo = search_block[pos] , pos -= BIT_WIDTH * blo ;
	if( blo >= B10_LEN ) return 0 ;
	if( pos < BIT_WIDTH - 3 ){
		rt = extract_ull_to_ull( x->data_[blo] , pos , pos + 3 ) ;
		x->data_[blo] -= ( rt << pos ) ;
	} else {
		int delta = pos + 3 - BIT_WIDTH ;
		rt = extract_ull_to_ull( x->data_[blo] , pos , BIT_WIDTH ) ;
		x->data_[blo] -= ( rt << pos ) ;
		rt += ( extract_ull_to_ull( x->data_[blo+1] , 0 , delta ) << ( 3 - delta ) ) ;
		x->data_[blo+1] = 0 ;
	} return rt ;
}

void b10_add_pos_num( b10 *x , int pos , ull num ){
	static int blo ;
	blo = b10_search_blo[pos] , pos -= blo * B10_WIDTH ;
	x->data_[blo] += num * pow10_[pos] ;
}

b10 string_to_b10( char *a , int lf , int rg ){
	static b10 rt ;
	static int len , blo , num , pt , i , j ;

	memset( &rt , 0 , sizeof( b10 ) ) ;
	// here len is length
	len = rg - lf + 1 , blo = ( len - 1 ) / B10_WIDTH , num = 0 , pt = rg ;
	for( i = 0 ; i < blo ; i ++ ){
		for( j = 0 ; j < B10_WIDTH ; j ++ , pt -- ) num += ( a[pt] - '0' ) * pow10_[j] ;
		rt.data_[i] = num , num = 0 ;
	} for( j = 0 ; pt >= lf ; j ++ , pt -- ) num += ( a[pt] - '0' ) * pow10_[j] ;
	rt.data_[blo] = num , num = 0 ;
	return rt ;
}

void b10_to_string( b10 *int_ , int frac_len , b10 *frac_ , char *a , bool sgn ){
	static int pt , int_len , blo ;
	
	pt = 0 , int_len = max( 0 , b10_flo_10( int_ , B10_WIDTH * B10_LEN - 1 ) ) ;
	if( sgn ) a[++pt] = '-' ;
	
	blo = int_len / B10_WIDTH ;
	for( int i = int_len , p10 = int_len % B10_WIDTH + 1 ; i >= 0 ; i -- , p10 -- ){
		a[++pt] = ( int_->data_[blo] % pow10_[p10] ) / pow10_[p10-1] + '0' ;
		if( i % 9 == 0 ) blo -- , p10 = 10 ;
	}
	if( frac_len >= 0 ){
		a[++pt] = '.' ;
		blo = frac_len / B10_WIDTH ;
		for( int i = frac_len , p10 = frac_len % B10_WIDTH + 1 ; i >= 0 ; i -- , p10 -- ){
			a[++pt] = ( frac_->data_[blo] % pow10_[p10] ) / pow10_[p10-1] + '0' ;
			if( i % 9 == 0 ) blo -- , p10 = 10 ;
		}
	} a[++pt] = '\0' ;
}

const uint fast_div = 65536 ;
const int prec10_csv = 23 , out_prec = 20 , fast_div_base = 16 ;
int b10_standard_prec( b10 *a , int *ori_len ){
	static int rt , blo , len ;
	static ull tmp ;

	rt = 0 , len = (*ori_len ) ;
	while( ( len = b10_flo_10( a , len ) ) > prec10_csv ){
		blo = len / B10_WIDTH ;
		for( int i = blo ; i > 0 ; i -- ){
			tmp = a->data_[i] / fast_div ;
			a->data_[i-1] += ( a->data_[i] - tmp * fast_div ) * B10_MAX ;
			a->data_[i] = tmp ;
		} a->data_[0] /= fast_div , rt += fast_div_base ;
	} (*ori_len ) = len ;
	return rt ;
}

int b10_to_bit_x( b10 *x , int len , bit_x *aim ){
	static int pt ;
	pt = 0 ;
	while( ( len = b10_flo_10( x , len ) ) >= 0 ){
		if( b10_div_2( x , len ) )bit_x_set_one( aim , pt ) ;
		pt ++ ;
	} return pt - 1 ;
}

b10 bit_x_copy_b10( bit_x *x , int lf , int rg ){
	static b10 rt ;
	static bit_x tmp ;

	memset( &rt , 0 , sizeof( rt ) ) ;
	tmp = extract_bit_x( x , lf , rg ) ;
	for( int i = search_block[ rg-lf ] ; i >= 0 ; i -- ) rt.data_[i] = tmp.data_[i] ;
	return rt ;
}

b10 bit_x_to_b10( bit_x *x , int lf , int rg ){
	static int blo ;
	static b10 rt ;
	static bit_x tmp ;

	memset( &rt , 0 , sizeof( rt ) ) ;
	if( lf <= rg ){
		tmp = extract_bit_x( x , lf , rg ) ;
		blo = search_block[ rg-lf ] ;
		for( int i = 0 ; i <= blo ; i ++ )
			b10_add( &rt , b10_mul_x( &CONST_2K[ i*BIT_WIDTH ] , len_2K[ i*BIT_WIDTH ] , tmp.data_[i] ) ) ;
	} return rt ;
}

/*void b10_out( b10 *x , int len ){
	len = len / B10_WIDTH ;
	for( ; len >= 0 ; len -- )
		printf( "%llu " , x->data_[len] ) ;
	puts( "" ) ;
}*/

md string_to_myDouble( char *a , bool sgn ){
	static b10 int_ , float_ ;
	static int len , point_pos , int_len , float_len , e , aclen , pt ;
	static bit_x frac ;

	memset( &frac , 0 , sizeof( frac ) ) , e = 0 ;

	len = strlen( a + 1 ) , point_pos = len + 1 ;
	for( int i = 1 ; i <= len ; i ++ )
		if( a[i] == '.' ){ point_pos = i ; break ; }
	//note here: len is max_index
	int_len = point_pos - 2 , float_len = len - point_pos - 1 ;
	int_ = string_to_b10( a , 1 , point_pos - 1 ) ;
	if( int_len > prec10_csv ) e += b10_standard_prec( &int_ , &int_len ) ;

	// extract integer
	aclen = b10_to_bit_x( &int_ , int_len , &frac ) , e += aclen ;
	if( aclen < FRAC_BIT ){
		pt = FRAC_BIT - aclen - 1 ; bit_x_Lmove( &frac , pt + 1 ) ;
		aclen = FRAC_BIT ;
	} if( float_len < 0 ) return bit_x_to_myDouble( sgn , e , &frac , aclen ) ;

	// extract float 
	bool pre_zero = bit_x_check_zero( &frac ) ;
	float_ = string_to_b10( a , point_pos + 1 , len ) ;

	if( pre_zero ){
		while( 1 ){
			int t_len = b10_flo_10( &float_ , float_len ) ;
			if( t_len + 5 >= float_len + 1 ) break ;
			float_ = b10_mul_x( &float_ , t_len , 8192 ) ;
			e -= 13 ;
		}
		while( 1 ){
			int t_len = b10_flo_10( &float_ , float_len ) ;
			if( t_len + 2 >= float_len + 1 ) break ;
			float_ = b10_mul_x( &float_ , t_len , 8 ) ;
			e -= 3 ;
		}
	}

	while( pt ){
		if( b10_mul_2( &float_ , float_len ) ) break ;
		if( b10_del_highest_one( &float_ , float_len + 1 ) ){
			bit_x_set_one( &frac , pt ) ;
			pre_zero = false ; pt -- ;
		} else {
			if( !pre_zero ) pt -- ;
			else e -- ;
		}
	} return bit_x_to_myDouble( sgn , e , &frac , FRAC_BIT ) ;
}

void myDouble_to_string( md x ){
	static bool sgn ;
	static int e , int_pos , check_pos , left_prec , float_len ;
	static bit_x bit_x_frac ;
	static b10 int_ , float_ , bi_float;

	if( check_NAN( &x ) ){ memcpy( char_out , cNAN , sizeof( cNAN ) ) ; return ; }
	sgn = get_sgn( &x ) ;
	if( check_oo( &x ) ){ memcpy( char_out , sgn ? c_INF : cINF , sizeof( c_INF ) ) ; return ; }

	e = get_exp( &x ) - Bias ;
	bit_x_frac = get_frac_to_bit_x( &x ) , int_pos = FRAC_BIT ;

	// Case : integer without float
	if( e >= int_pos ){
		e -= int_pos , int_pos = 0 ;
		int_ = bit_x_to_b10( &bit_x_frac , int_pos , FRAC_BIT ) ;
		b10_mul_2k( &int_ , e ) ;
		b10_to_string( &int_ , -1 , &float_ , char_out , sgn ) ;
		return ;
	}

	// Case : float
	int_pos -= e , int_ = bit_x_to_b10( &bit_x_frac , int_pos , FRAC_BIT ) ;
	left_prec = out_prec - ( b10_flo_10( &int_ , out_prec ) + 1 ) ;
	
	memset( &float_ , 0 , sizeof( float_ ) ) ;
	bi_float = bit_x_copy_b10( &bit_x_frac , 0 , min( int_pos , FRAC_BIT + 1 ) - 1 ) ;
	check_pos = int_pos , float_len = left_prec - 1 ;

	while( left_prec ){
		b10_mul_2base( &bi_float , check_pos , 5 ) ;
		check_pos -- ;
		b10_add_pos_num( &float_ , left_prec - 1 , b10_del_highest_num__2base( &bi_float , check_pos ) ) ;
		left_prec -- ;
	} b10_to_string( &int_ , float_len , &float_ , char_out , sgn ) ;

}

void print( md x ){
	myDouble_to_string( x ) ;
	printf( "%s" , char_out + 1 ) ;
}

void myIO_init(){
	for( int i = 0 ; i <= 1000 ; i ++ ) b10_search_blo[i] = i / B10_WIDTH ;
	memset( CONST_2K , 0 , sizeof( CONST_2K ) ) ;
	CONST_2K[0].data_[0] = 1 , len_2K[0] = 0 ;
	for( int i = 1 ; i <= Bias ; i ++ ){
		CONST_2K[i] = CONST_2K[i-1] , len_2K[i] = len_2K[i-1] + 1 ;
		b10_mul_2( &CONST_2K[i] , len_2K[i] ) ;
		len_2K[i] = b10_flo_10( &CONST_2K[i] , len_2K[i] ) ;
	}
}
