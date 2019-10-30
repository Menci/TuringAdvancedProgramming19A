#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#define myDouble_len 8

typedef unsigned int uint ;
typedef unsigned long long ull ;
typedef unsigned char uchar ;
typedef struct myDouble{
	unsigned char data_[myDouble_len] ;
} md ;

typedef struct bit_x_Data {
	ull data_[4] ;
} bit_x;

const int Bias = 1023 , EXP_BIT = 11 , FRAC_BIT = 52 ;
const int dou_bit = 64 , ULL_BIT = 64 , int_bit = 32 ;
const int BIT_WIDTH = 31 , BIT_X_LEN = 4 ;
const unsigned long long BIT_X_MAX = 0x80000000 ;
const int exp_max = 2047 , E_MIN = -1022 , E_MAX = 1023 ;
const double FORCE_CUT_PREC = 1.5 ;
int PREC_CSV ;

const int SIZE_OF_ULL = 8 ;

/*{{{*/
const uchar __data_oo[8] = { 0 , 0 , 0 , 0 , 0 , 0 , (uchar)0xF0 , (uchar)0x7F } ;
const uchar __data_minus_oo[8] = { 0 , 0 , 0 , 0 , 0 , 0 , (uchar)0xF0 , (uchar)0xFF } ;
const uchar __data_NAN[8] = { 0 , 0 , 0 , 0 , 0 , 0 , (uchar)0xF8 , (uchar)0xFF } ;
const uchar __data_zero[8] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ;
int search_block[10005] ;

md const_NAN , const_oo , const_minus_oo , const_zero ;
int min( int a , int b ){ return a < b ? a : b ; }
int max( int a , int b ){ return a > b ? a : b ; }
	
void _set_zero( ull *a , int pos ){ if( (*a) & ( 1ull << pos ) ) (*a) -= ( 1ull << pos ) ; }

bool extract_ull_byte( const ull x , int pos ){ return (bool) ( x & ( 1ull << pos ) ) ; }

unsigned long long extract_ull_to_ull( const ull x , int lf , int rg ){
	return ( x << ( ULL_BIT - rg - 1 ) ) >> ( ULL_BIT - rg - 1 + lf ) ;
}

unsigned int extract_uint_to_uint( unsigned int x , int lf , int rg ){
	return ( x << ( int_bit - rg - 1 ) ) >> ( int_bit - rg - 1 + lf ) ;
}

unsigned long long extract_md_to_ull( md *x , int lf , int rg ){
	static int L_blo , R_blo , len ;
	static ull rt ;

	L_blo = lf / CHAR_BIT , R_blo = rg / CHAR_BIT ;
	memcpy( &rt , ((char*)x) + L_blo , R_blo - L_blo + 1 ) ;

	len = rg - lf + 1 ;
	lf -= L_blo * CHAR_BIT , rg -= R_blo * CHAR_BIT ;
	return extract_ull_to_ull( rt >> lf , 0 , len - 1 ) ;
}

void bit_x_Rmove( bit_x *x , int ws ){
	while( ws >= BIT_WIDTH ){
		for( int i = 0 ; i < BIT_X_LEN - 1 ; i ++ ) x->data_[i] = x->data_[i+1] ;
		x->data_[ BIT_X_LEN - 1 ] = 0 ;
		ws -= BIT_WIDTH ;
	} if( !ws ) return ;
	
	for( int i = 0 ; i <= BIT_X_LEN - 1 ; i ++ ){
		x->data_[i] >>= ws ;
		if( i < BIT_X_LEN - 1 ) x->data_[i] += extract_ull_to_ull( x->data_[i+1] , 0 , ws - 1 ) << ( BIT_WIDTH - ws ) ;
	}
}

void bit_x_Rmove1( bit_x *x ){
	for( int i = 0 ; i <= BIT_X_LEN - 1 ; i ++ ){
		x->data_[i] >>= 1 ;
		if( i < BIT_X_LEN - 1 ) x->data_[i] += ( x->data_[i+1] & 1 ) << ( BIT_WIDTH - 1 ) ;
	}
}

void bit_x_Lmove( bit_x *x , int ws ){
	while( ws >= BIT_WIDTH ){
		for( int i = BIT_X_LEN - 1 ; i ; i -- ) x->data_[i] = x->data_[i-1] ;
		x->data_[0] = 0 ;
		ws -= BIT_WIDTH ;
	} if( !ws ) return ;

	for( int i = BIT_X_LEN - 1 ; i ; i -- ){
		x->data_[i] <<= ws ;
		x->data_[i] += extract_ull_to_ull( x->data_[i-1] , BIT_WIDTH - ws , BIT_WIDTH - 1 ) ;
		x->data_[i] = extract_ull_to_ull( x->data_[i] , 0 , BIT_WIDTH - 1 ) ;
	} x->data_[0] = extract_ull_to_ull( x->data_[0] , 0 , BIT_WIDTH - 1 - ws ) << ws ;
}

void bit_x_Lmove1( bit_x *x ){
	for( int i = BIT_X_LEN - 1 ; i ; i -- ){
		x->data_[i] = extract_ull_to_ull( x->data_[i] , 0 , BIT_WIDTH - 2 ) << 1 ;
		x->data_[i] += extract_ull_byte( x->data_[i-1] , BIT_WIDTH - 1 ) ;
	} x->data_[0] = extract_ull_to_ull( x->data_[0] , 0 , BIT_WIDTH - 2 ) << 1 ;
}

void bit_x_LmoveWIDTH( bit_x *x ){
	for( int i = BIT_X_LEN - 1 ; i ; i -- ) x->data_[i] = x->data_[i-1] ;
	x->data_[0] = 0 ;
}

bit_x extract_bit_x( bit_x *x , int lf , int rg ){
	static int L_blo , R_blo , wid ;
	static bit_x rt ;

	memset( &rt , 0 , sizeof( rt ) ) ;
	if( lf > rg ) return rt ;
	
	L_blo = search_block[lf] , R_blo = search_block[rg] , wid = R_blo - L_blo + 1 ;
	memcpy( &rt , ((ull*)x) + L_blo , SIZE_OF_ULL * wid ) ;

	lf -= L_blo * BIT_WIDTH , rg -= R_blo * BIT_WIDTH ;
	rt.data_[ wid-1 ] = extract_ull_to_ull( rt.data_[ wid-1 ] , 0 , rg ) ;
	bit_x_Rmove( &rt , lf ) ;
	return rt ;
}

unsigned char extract_bit_x_to_uchar( bit_x *x , int lf , int rg ){
	return (uchar)extract_bit_x( x , lf , rg ).data_[0] ;
}

bool extract_bit_x_byte( bit_x *x , int pos ){
	static int blo ;
	blo = search_block[pos] , pos -= blo * BIT_WIDTH ;
	return extract_ull_byte( x->data_[blo] , pos ) != 0 ;
}

bool bit_x_check_zero( bit_x *x ){/*{{{*/
	static int pt ;
	for( pt = BIT_X_LEN - 1 ; pt >= 0 && !x->data_[pt] ; pt -- ) ;
	return pt == -1 ;
}/*}}}*/

bool bit_x_check_zero_cp( bit_x x ){/*{{{*/
	static int pt ;
	for( pt = BIT_X_LEN - 1 ; pt >= 0 && !x.data_[pt] ; pt -- ) ;
	return pt == -1 ;
}/*}}}*/

void bit_x_set_zero( bit_x *x , int pos ){
	static int blo ;
	blo = search_block[pos] , pos -= blo * BIT_WIDTH ;
	if( x->data_[blo] & ( 1ull << pos ) ) x->data_[blo] -= ( 1ull << pos ) ;
}

void bit_x_set_one( bit_x *x , int pos ){
	static int blo ;
	if( pos < 0 ) return ;
	blo = search_block[pos] , pos -= blo * BIT_WIDTH ;
	x->data_[blo] |= ( 1ull << pos ) ;
}

int bit_x_flo( bit_x *x ){
	static int i ;
	for( i = BIT_X_LEN - 1 ; i >= 0 ; i -- )
		if( x->data_[i] ) return i * BIT_WIDTH + ( ULL_BIT - __builtin_clzll( x->data_[i] ) - 1 ) ;
	return -1 ;
}

int __super_bit_x_compare ;
int bit_x_compare_super( bit_x *a , bit_x *b , int i ){
	for( ; i >= 0 ; i -- )
		if( a->data_[i] != b->data_[i] ) return a->data_[i] > b->data_[i] ? 1 : -1 ;
	return 0 ;
}

void bit_x_borrow( bit_x *x , int pos ){/*{{{*/
	static int pt ;
	pt = pos + 1 ;
	while( !x->data_[pt] && pt < BIT_X_LEN ) pt ++ ;
	if( pt == BIT_X_LEN ) return (void)printf( "bit_x_borrow wrong!  cause: can't borrow from higher" ) ;
	for( ; pt > pos ; pt -- )
		x->data_[pt] -- , x->data_[pt-1] += BIT_X_MAX ;
}/*}}}*/

void bit_x_minus( bit_x *a , bit_x *b ){ // must a bigger than b
	static int pt ;
	for( pt = BIT_X_LEN - 1 ; pt >= 0 ; pt -- ){
		if( a->data_[pt] < b->data_[pt] ) bit_x_borrow( a , pt ) ;
		a->data_[pt] -= b->data_[pt] ;
	}
}

int __super_bit_x_minus ;
void bit_x_minus_super( bit_x *a , bit_x *b , int pt ){ // must a bigger than b
	for( ; pt >= 0 ; pt -- ){
		if( a->data_[pt] < b->data_[pt] ) bit_x_borrow( a , pt ) ;
		a->data_[pt] -= b->data_[pt] ;
	}
}

bool get_sgn( const md *x ){ return x->data_[ myDouble_len-1 ] & ( 0x80 ) ; }

int __fast_get_exp_startpos , __fast_get_exp_size ;
int get_exp( const md *x ){/*{{{*/
	static ull rt ;
	memcpy( &rt , ((char*)x) + __fast_get_exp_startpos , __fast_get_exp_size ) ; // get high bit
	rt >>= ( FRAC_BIT % CHAR_BIT ) ;			// discard frac part
	_set_zero( &rt , EXP_BIT ) ;	// set sgn BITE 0
	if( rt == 0 ) rt = 1 ;
	return (int)rt ;
}/*}}}*/

bit_x get_frac_to_bit_x( md *x ){
	static bit_x rt ;
	static int ws , L , pt ;
	
	memset( &rt , 0 , sizeof( bit_x ) ) ;
	ws = FRAC_BIT , L = 0 , pt = 0 ;
	while( ws >= BIT_WIDTH ){
		rt.data_[pt] = extract_md_to_ull( x , L , L + BIT_WIDTH - 1 ) ;
		L += BIT_WIDTH , ws -= BIT_WIDTH , pt ++ ;
	} if( ws )rt.data_[pt] = extract_md_to_ull( x , L , L + ws - 1 ) ;

	// check exp 0
	if( extract_md_to_ull( x , FRAC_BIT , FRAC_BIT + EXP_BIT - 1 ) ) rt.data_[pt] |= ( 1ull << ws ) ;

	return rt ;
}

md rev_sign( md x ){/*{{{*/
	x.data_[ myDouble_len-1 ] ^= 0x80 ;
	return x ;
}/*}}}*/

void rev_sign_self( md *x ){
	x->data_[ myDouble_len-1 ] ^= 0x80 ;
}

md force_sign( md x , bool sgn ){
	return get_sgn( &x ) == sgn ? x : rev_sign( x ) ;
}

bool check_oo( md *x ){
	bool ok = true ;
	if( get_sgn( x ) ){
		for( int i = myDouble_len - 1 ; i >= 0 && ok ; i -- )
			ok &= ( x->data_[i] == const_minus_oo.data_[i] ) ;
	} else {
		for( int i = myDouble_len - 1 ; i >= 0 && ok ; i -- )
			ok &= ( x->data_[i] == const_oo.data_[i] ) ;
	} return ok ;
}

bool check_0( md *x ){
	int pt = myDouble_len - 2 ;
	for( ; pt >= 0 && !x->data_[pt] ; pt -- ) ;
	return pt == -1 && ( x->data_[ myDouble_len-1 ] & 0x7F ) == 0 ;
}

bool check_NAN( md *x ){ return get_exp( x ) == exp_max && !bit_x_check_zero_cp( get_frac_to_bit_x( x ) ) ; }

void swap( md *A , md *B ){/*{{{*/
	static md C ;
	C = *A , *A = *B , *B = C ;
}/*}}}*/

int A_bigger_B( md A , md B ){/*{{{*/
	static bool A_sgn , B_sgn , fix ;
	static int A_fir , B_fir , pt ;

	// condition NAN
	if( check_NAN( &A ) ) return 1 ;
	if( check_NAN( &B ) ) return -1 ;

	// condition sign not the same
	A_sgn = A.data_[myDouble_len-1] & 0x80, B_sgn = B.data_[myDouble_len-1] & 0x80 ;
	if( A_sgn != B_sgn ) return A_sgn == 0 ? 1 : -1 ;
	fix = A_sgn ;

	A_fir = A.data_[myDouble_len - 1] & 0x7f ;
	B_fir = B.data_[myDouble_len - 1] & 0x7f ;
	if( A_fir != B_fir ) return (( A_fir > B_fir )^fix) ? 1 : -1 ;

	for( pt = myDouble_len - 2 ; pt >= 0 ; pt -- )
		if( A.data_[pt] != B.data_[pt] ) return (( A.data_[pt] > B.data_[pt] ) ^ fix) ?  1 : -1 ;
	return 0 ;
}/*}}}*/

void bit_x_standardize( bit_x *x ){/*{{{*/
	for( int i = 0 ; i < BIT_X_LEN - 1 ; i ++ ) if( x->data_[i] >= BIT_X_MAX ) {
		x->data_[i+1] += x->data_[i] / BIT_X_MAX ;
		x->data_[i] %= BIT_X_MAX ;
	}
}/*}}}*/

void md_out( md x ){/*{{{*/
	for( int i = 7 ; i >= 0 ; i -- )
		printf( "%2x " , (unsigned char)x.data_[i] ) ;
	puts( "" ) ;
}/*}}}*/

void bit_x_out( bit_x x ){
	for( int i = BIT_X_LEN - 1 ; i >= 0 ; i -- )
		printf( "%llu " , x.data_[i] ) ;
	puts( "" ) ;
}

md make_md_bit_x( const bool sgn , uint exp , bit_x *frac ){
	static md tmp ;
	static int pt , ws ;
	
	ws = 0 ;
	for( pt = 0 ; ws <= FRAC_BIT - CHAR_BIT ; pt ++ , ws += CHAR_BIT )
		tmp.data_[pt] = extract_bit_x_to_uchar( frac , ws , ws + CHAR_BIT - 1 ) ;
	if( ws < FRAC_BIT ) tmp.data_[pt] = extract_bit_x_to_uchar( frac , ws , FRAC_BIT - 1 ) , ws = FRAC_BIT ;

	exp -= ( extract_bit_x_byte( frac , FRAC_BIT ) ? 0 : 1 ) ;
	
	ws %= CHAR_BIT ;
	if( ws != 0 ){
		tmp.data_[pt] += extract_uint_to_uint( exp , 0 , CHAR_BIT - ws - 1 ) << ws ;
		ws = CHAR_BIT - ws , pt ++ ;
	}

	for( ; ws <= EXP_BIT - CHAR_BIT ; pt ++ , ws += CHAR_BIT )
		tmp.data_[pt] = extract_uint_to_uint( exp , ws , ws + CHAR_BIT - 1 ) ;
	if( ws < EXP_BIT ) tmp.data_[pt] = extract_uint_to_uint( exp , ws , EXP_BIT - 1 ) , ws = EXP_BIT ;

	return force_sign( tmp , sgn ) ;
}

md bit_x_to_myDouble( bool sgn , int e , bit_x *ori_x , int aclen ){
	static int flo , limit ;
	static bit_x x , frac_use ;

	x = *ori_x , bit_x_standardize( &x ) ;
	flo = bit_x_flo( &x ) ;
	if( flo == -1 ) return const_zero ;
	
	// get e
	
	bit_x_Lmove( &x , 3 ) , aclen += 3 , flo += 3 ;
	
	limit = e - E_MIN ;
	if( limit < 0 ) bit_x_Rmove( &x , -limit ) , flo += limit , e -= limit ;

	if( flo > aclen ) bit_x_Rmove( &x , flo - aclen ) , e += flo - aclen ;
	if( flo < aclen ) {
		limit = min( aclen - flo , (int)( e - E_MIN ) ) ;
		if( limit > 0 ) bit_x_Lmove( &x , limit ) , e -= limit , flo += limit ;
	}

	if( e > E_MAX ) return sgn ? const_minus_oo : const_oo ;

	// round
	frac_use = extract_bit_x( &x , aclen - FRAC_BIT , aclen ) ;
		
	if( extract_bit_x_byte( &x , aclen - FRAC_BIT - 1 ) ){
		if( bit_x_check_zero_cp( extract_bit_x( &x , 0 , aclen - FRAC_BIT - 2 ) ) ){
			if( frac_use.data_[0] & 1 ) frac_use.data_[0] ++ ;
		} else frac_use.data_[0] ++ ;
		if( frac_use.data_[0] >= BIT_X_MAX ) bit_x_standardize( &frac_use ) ;
	}
	
	flo = bit_x_flo( &frac_use ) ;

	if( flo > FRAC_BIT ) bit_x_Rmove1( &frac_use ) , e ++ ;

	if( e > E_MAX ) return sgn ? const_minus_oo : const_oo ;

	return make_md_bit_x( sgn , (unsigned)(e + Bias) , &frac_use ) ;
}

md Multiply( md A , md B ) ;
md Add( md A , md B ) ;
md Minus( md A , md B ) ;
md Divide( md A , md B ) ;

md Add( md A , md B ){/*{{{*/
	static bool A_sgn , B_sgn , sgn ;
	static int A_e , B_e , C_e , e_gap ;
	static bit_x A_frac , B_frac , tmp ;
	static md rt ;

	// process same_signal number only
	if( A_bigger_B( A , B ) == -1 ) swap( &A , &B ) ;
	A_sgn = get_sgn( &A ) , B_sgn = get_sgn( &B ) ;
	if( A_sgn ^ B_sgn ) return Minus( A , rev_sign( B ) ) ;

	// condition : A or B is NAN
	if( check_NAN( &A ) ) return const_NAN ;

	// condition : A is (-)oo
	if( check_oo( &A ) ) return A_sgn ? const_minus_oo : const_oo ;

	// condition : normal
	sgn = A_sgn ;
	if( sgn ) swap( &A , &B ) ; // abstract bigger in first place

	C_e = A_e = get_exp( &A ) - Bias , B_e = get_exp( &B ) - Bias , e_gap = A_e - B_e ;
	if( e_gap > FRAC_BIT + 1 ) return A ;

	memset( &tmp , 0 , sizeof( bit_x ) ) ;
	A_frac = get_frac_to_bit_x( &A ) , B_frac = get_frac_to_bit_x( &B ) ;
	bit_x_Lmove( &A_frac , e_gap ) ;

	for( int i = 0 ; i < BIT_X_LEN ; i ++ )
		tmp.data_[i] = A_frac.data_[i] + B_frac.data_[i] ;

	rt = bit_x_to_myDouble( sgn , C_e , &tmp , FRAC_BIT + e_gap ) ;
	return rt ;
}/*}}}*/

md Minus( md A , md B ){ /*{{{*/
	static bool A_sgn , sgn ;
	static int A_e , B_e , C_e , e_gap ;
	static bit_x A_frac , B_frac ;
	static md rt ;

	// process positive number only
	A_sgn = get_sgn( &A ) , sgn = get_sgn( &B ) ^ A_sgn ;
	if( sgn ){
		if( A_sgn ) rt = Add( rev_sign( A ) , B ) ;
		else rt = Add( A , rev_sign( B ) ) ;
		return force_sign( rt , A_sgn ) ;
	}

	if( A_sgn ) rev_sign_self( &A ) , rev_sign_self( &B ) , sgn = A_sgn ; // turn to positive
	if( A_bigger_B( A , B ) == -1 ) swap( &A , &B ) , sgn ^= 1 ;

	// condition : A or B is NAN
	if( check_NAN( &A ) ) return const_NAN ;

	// condition : A is (-)oo
	if( check_oo( &A ) ){
		if( check_oo( &B ) ) return const_NAN ;
		return sgn ? const_minus_oo : const_oo ;
	}

	// condition : normal
	C_e = A_e = get_exp( &A ) - Bias , B_e = get_exp( &B ) - Bias , e_gap = A_e - B_e ;
	if( e_gap > FRAC_BIT + 1 ) return force_sign( A , sgn ) ;

	A_frac = get_frac_to_bit_x( &A ) , B_frac = get_frac_to_bit_x( &B ) ;
	bit_x_Lmove( &A_frac , e_gap ) ;

	bit_x_minus( &A_frac , &B_frac ) ;

	rt = bit_x_to_myDouble( sgn , C_e , &A_frac , FRAC_BIT + e_gap ) ;
	return rt ;
}/*}}}*/

md Multiply( md A , md B ){/*{{{*/
	static bool sgn , A_oo , A_0 , B_oo , B_0 ;
	static int A_e , B_e , C_e , limit ;
	static bit_x tmp , A_frac , B_frac ;
	static md rt ;

	// condition : A or B is NAN
	if( check_NAN( &A ) || check_NAN( &B ) ) return const_NAN ;

	// condition : spe judge
	sgn = get_sgn( &A ) ^ get_sgn( &B ) ;
	
	
	A_oo = check_oo( &A ) , B_oo = check_oo( &B ) ;
	if( A_oo || B_oo ) return sgn ? const_minus_oo : const_oo ;
	
	A_0 = check_0( &A ) , B_0 = check_0( &B ) ;
	if( A_0 || B_0 ) return const_zero ;

	if( ( A_oo && B_0 ) || ( B_oo && A_0 ) ) return const_NAN ;

	// condition : normal
	A_e = get_exp( &A ) - Bias , B_e = get_exp( &B ) - Bias ;
	C_e = A_e + B_e ;
	if( C_e > E_MAX ) return sgn ? const_minus_oo : const_oo ;
	if( C_e < E_MIN - FRAC_BIT - 2 ) return const_zero ;

	A_frac = get_frac_to_bit_x( &A ) , B_frac = get_frac_to_bit_x( &B ) ;

	limit = search_block[FRAC_BIT] ;
	memset( &tmp , 0 , sizeof( bit_x ) ) ;
	for( int i = 0 ; i <= limit ; i ++ )
		for( int j = 0 ; j <= limit ; j ++ )
			tmp.data_[i+j] += A_frac.data_[i] * B_frac.data_[j] ;

	rt = bit_x_to_myDouble( sgn , C_e , &tmp , FRAC_BIT * 2 ) ;
	return rt ;
}/*}}}*/

md Divide( md A , md B ){/*{{{*/
	static bool sgn , A_oo , B_oo , A_0 , B_0 ;
	static int A_e , B_e , C_e , pt , gap ;
	static bit_x tmp , A_frac , B_frac ;
	static md rt ;

	if( check_NAN( &A ) || check_NAN( &B ) ) return const_NAN ;

	sgn = get_sgn( &A ) ^ get_sgn( &B ) ;
	A_oo = check_oo( &A ) , A_0 = check_0( &A ) ;
	B_oo = check_oo( &B ) , B_0 = check_0( &B ) ;

	// condition : spe judge
	if( A_oo ){
		if( B_oo ) return const_NAN ;
		return sgn ? const_minus_oo : const_oo ;
	}
	if( A_0 ) return B_0 ? const_NAN : const_zero ;

	if( B_oo ) return const_zero ;
	if( B_0 ) return sgn ? const_minus_oo : const_oo ;

	// condition : normal
	A_e = get_exp( &A ) - Bias , B_e = get_exp( &B ) - Bias , C_e = A_e - B_e ;
	if( C_e > E_MAX + 1 ) return sgn ? const_minus_oo : const_oo ;

	A_frac = get_frac_to_bit_x( &A ) , B_frac = get_frac_to_bit_x( &B ) ;
	gap = bit_x_flo( &A_frac ) - bit_x_flo( &B_frac ) ;
	if( gap > 0 ) bit_x_Lmove( &B_frac , gap ) , C_e += gap ;
	else if( gap < 0 ) bit_x_Lmove( &A_frac ,-gap ) , C_e += gap ;

	memset( &tmp , 0 , sizeof( tmp ) ) ;
	for( pt = PREC_CSV ; pt >= 0 ; pt -- ){
		if( bit_x_compare_super( &A_frac , &B_frac , __super_bit_x_compare ) >= 0 ){
			bit_x_minus_super( &A_frac , &B_frac , __super_bit_x_minus ) ;
			bit_x_set_one( &tmp , pt ) ;
		} bit_x_Lmove1( &A_frac ) ;
	}
	
	rt = bit_x_to_myDouble( sgn , C_e , &tmp , PREC_CSV ) ;
	return rt ;

}/*}}}*/

void myDouble_lib_init(){/*{{{*/
	for( int i = 10000 ; i ; i -- ) search_block[i] = i / BIT_WIDTH ;
	__super_bit_x_compare = ( FRAC_BIT - 1 ) / BIT_WIDTH + 1 ;
	__super_bit_x_minus = ( FRAC_BIT - 1 ) / BIT_WIDTH + 1 ;
	__fast_get_exp_startpos = ( FRAC_BIT - 1 ) / CHAR_BIT ;
	__fast_get_exp_size = ( EXP_BIT - 1 ) / CHAR_BIT + 1 ;
	memcpy( const_oo.data_ , __data_oo , sizeof( char ) * myDouble_len ) ;
	memcpy( const_NAN.data_ , __data_NAN , sizeof( char ) * myDouble_len ) ;
	memcpy( const_zero.data_ , __data_zero , sizeof( char ) * myDouble_len ) ;
	memcpy( const_minus_oo.data_ , __data_minus_oo , sizeof( char ) * myDouble_len ) ;
	PREC_CSV = FRAC_BIT * FORCE_CUT_PREC ;
}/*}}}*/

md cal_opt( md A , md B , char opt ){
	md my_ans ;
	switch( opt ){
		case 1 : /*puts( "option : addition" ) , */my_ans = Add( A , B ) ; break ;
		case 2 : /*puts( "option : subtraction" ) , */my_ans = Minus( A , B ) ; break ;
		case 3 : /*puts( "option : multiplication" ) , */my_ans = Multiply( A , B ) ; break ;
		case 4 : /*puts( "option : division" ) , */my_ans = Divide( A , B ) ; break ;
	} return my_ans ;
}
/*}}}*/


