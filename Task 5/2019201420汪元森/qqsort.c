#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define size_long 8

typedef struct conti_interval{
	void *L , *R ;
} ci ;
ci seg[2000005] ;
int seg_cnt , gcnt , gsiz , mem2_poll[2000000*100] , swap_poll[400] ;
void *mem1 , *mem2 , *swap_mem , *rk[2000005] , *pt ;

int (*gcmp)( const void * , const void * ) ;
void (*swap)( void * , void * ) ;

void swap_long( void *a , void *b ){
	static uint64_t tmp ;
	tmp = *(uint64_t*)a , *(uint64_t*)a = *(uint64_t*)b , *(uint64_t*)b = tmp ;
}

void swap_char( void *a , void *b ){
	static uint8_t tmp ;
	tmp = *(uint8_t*)a , *(uint8_t*)a = *(uint8_t*)b , *(uint8_t*)b = tmp ;
}

void swap_siz( void *a , void *b ){
	memcpy( swap_mem , a , gsiz ) ;
	memcpy( a , b , gsiz ) ;
	memcpy( b , swap_mem , gsiz ) ;
}

inline void mem2_clear(){ pt = mem2 ; }

bool prework_easysort( void *st , size_t siz , void *aim ){
	if( gcnt == 1 ) return true ;
	seg_cnt = 0 ;
	void *now = st , *Lp , *Rp ;
	int typ = ( gcmp( now , now + siz ) <= 0 ) , L = 0 , i ;
	for( i = 2 , now += 2 * siz ; i < gcnt ; i ++ , now += siz ){
		if( ( gcmp( now - siz , now ) <= 0 ) != typ ){
			if( !typ ) for( Lp = st + L * siz , Rp = now - siz ; Lp < Rp ; Lp += siz , Rp -= siz )
				swap_siz( Lp , Rp ) ;
			seg[++seg_cnt] = ( ci ){ aim + size_long * L , aim + size_long * i } ;
			L = i ;
			if( i < gcnt - 1 ) typ = gcmp( now , now + siz ) <= 0 , i ++ , now += siz ;
		}
	} seg[++seg_cnt] = ( ci ){ aim + size_long * L , aim + size_long * gcnt } ;
	if( !typ ) for( Lp = st + L * siz , Rp = now - siz ; Lp < Rp ; Lp += siz , Rp -= siz )
		swap_siz( Lp , Rp ) ;
	return seg_cnt == 1 ;
}

void merge2( void *st1 , void *ed1 , void *st2 , void* ed2 ){
	while( st1 < ed1 && st2 < ed2 ){
		if( gcmp( *(void**)st1 , *(void**)st2 ) <= 0 ) *(void**)pt = *(void**)(st1) , st1 += size_long ;
		else *(void**)pt = *(void**)(st2) , st2 += size_long ;
		pt += size_long ;
	}
	if( st1 < ed1 ) memcpy( pt , st1 , ed1 - st1 ) , pt += ed1 - st1 ;
	if( st2 < ed2 ) memcpy( pt , st2 , ed2 - st2 ) , pt += ed2 - st2 ;
}

void qqsort(){
	do{
		mem2_clear() ;
		int i , new_seg_cnt = 0 ;
		for( i = 1 ; i + 1 <= seg_cnt ; i += 2 ){
			merge2( seg[i].L , seg[i].R , seg[i+1].L , seg[i+1].R ) ;
			++new_seg_cnt ;
			seg[new_seg_cnt].L = pt - ( seg[i+1].R - seg[i].L ) ; seg[new_seg_cnt].R = pt ;
		} if( i == seg_cnt ){
			memcpy( pt , seg[i].L , seg[i].R - seg[i].L ) , pt += seg[i].R - seg[i].L ;
			++new_seg_cnt ;
			seg[new_seg_cnt].L = pt - ( seg[i].R - seg[i].L ) ; seg[new_seg_cnt].R = pt ;
		}
		swap_long( &mem1 , &mem2 ) ;
		seg_cnt = new_seg_cnt ;
	} while( seg_cnt != 1 ) ;
}

bool vis[2000005] ;
void solve( void *st , size_t cnt , size_t siz , int (*comp)( const void * , const void * ) ){
	static bool init = false ;
	if( !init ){
		swap_mem = swap_poll , mem1 = rk , mem2 = mem2_poll ;
		init = true ;
	}

	gcmp = comp , gcnt = cnt , gsiz = siz ;
	if( siz == 1 ) swap = swap_char ;
	else if( siz == 8 ) swap = swap_long ;
	else swap = swap_siz ;

	if( prework_easysort( st , siz , rk ) == true ) return ;
	rk[0] = st ;
	for( int i = 1 ; i < cnt ; i ++ ) rk[i] = rk[i-1] + siz ;

	qqsort() ;
	if( mem1 != rk ) swap_long( &mem1 , &mem2 ) , memcpy( mem1 , mem2 , cnt * size_long ) ;
	if( siz > 300 ){
		memset( vis , 0 , cnt ) ;
		for( int i = 0 , j , nxt ; i < cnt ; i ++ ) if( !vis[i] ) {
			if( rk[i] != st + i * siz ){
				memcpy( swap_mem , rk[i] , siz ) ;
				for( j = i , nxt = ( rk[j] - st ) / siz ; !vis[nxt] ; j = nxt , nxt = ( rk[j] - st ) / siz ){
					vis[j] = true ;
					memcpy( rk[j] , rk[nxt] , siz ) ;
				} vis[j] = true ; memcpy( rk[j] , swap_mem , siz ) ;
			}
		}
	} else {
		mem2_clear() ;
		for( int i = 0 ; i < cnt ; i ++ ) memcpy( pt , rk[i] , siz ) , pt += siz ;
		memcpy( st , mem2 , siz * cnt ) ;
	}
}
