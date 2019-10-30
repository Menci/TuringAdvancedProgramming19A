#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define maxN 60
#define maxX 60 
#define maxA 3 
#define reg_cnt 6
#define type_cnt 3

int call_st[maxX + maxA + 5] , N , head[maxX + maxA + 5] , global_term ;
int id_st[type_cnt + 5] , id_c ;
const char rop[6] = { 0 , '+' , '-' , '*' , '/' , '=' } ;
typedef struct to_nxt_list{
	int t ;
	int nxt ;
} nxt_L ;
nxt_L call_time[100005] ;
int call_pt ;

typedef struct typed_number{
	int typ , num ;
} tnum ;

typedef struct formula{
	tnum num[type_cnt] ;
	int op ;
} fml ;
fml fm[maxN + 5] ;

/*{{{*/
void In_nxt_list( int pos , int id ){
	int pre = head[id] ;
	if( pos == call_time[pre].t ) return ;
	call_pt ++ ;
	// set the first pointer
	if( !call_st[id] ) call_st[id] = call_pt ;
	// set previos' next pointer
	if( pre ) call_time[pre].nxt = call_pt ;
	head[id] = call_pt ;
	// set *this information
	call_time[call_pt].t = pos ;
}

void goto_nxt_call( int id ){
	call_st[id] = call_time[ call_st[id] ].nxt ;
}

int find_nxt_call( int id ){
	if( call_time[ call_st[id] ].t > global_term )
		return call_time[ call_st[id] ].t ;
	return call_time[ call_time[ call_st[id] ].nxt ].t ;
}

bool tnum_no_equal( tnum x , tnum y ){
	return x.typ != y.typ || x.num != y.num ;
}

int get_op_id( char op ){
	if( op == '+' ) return 1 ;
	if( op == '-' ) return 2 ;
	if( op == '*' ) return 3 ;
	if( op == '/' ) return 4 ;
	puts( "invalid calc_option input" ) ; exit( 255 ) ;
}

int get_tnum_id( tnum x ){
	if( !x.typ ) return 0 ;
	return id_st[ x.typ ] + x.num ;
}

tnum s_to_tnum( char *a ){
	tnum rt ;
	int pt = 1 , flag = 1 ;
	if( a[pt] == 'v' ) rt.typ = 1 , pt ++ ;
	else if( a[pt] == 'a' ) rt.typ = 2 , pt ++ ;
	else rt.typ = 0 ;
	
	if( a[pt] == '-' ) flag = -1 , pt ++ ;
	rt.num = a[pt] - '0' , pt ++ ;
	while( '0' <= a[pt] && a[pt] <= '9' ) rt.num = rt.num * 10 + a[pt] - '0' , pt ++ ;
	rt.num *= flag ;
	return rt  ;
}

fml make_fml( tnum x , tnum y , int op , tnum z ){
	static fml rt ;
	rt.num[0] = x , rt.num[1] = y , rt.num[2] = z , rt.op = op ;
	return rt ;
}

void read_in(){
	char X[15] , Y[15] , Z[15] , op[15] ;
	while( scanf( "%s" , X + 1 ) != EOF ){
		scanf( "%*3c%s" , Y + 1 ) ;
		scanf( "%*c%s" , op + 1 ) ;
		scanf( "%*c%s" , Z + 1 ) ;
		fm[++N] = make_fml( s_to_tnum( X ) , s_to_tnum( Y ) , get_op_id( op[1] ) , s_to_tnum( Z ) ) ;
	}
}

void preWork(){
	id_st[1] = id_c = 0 ;
	id_st[2] = id_c = id_c + maxX ;
	for( int i = 1 ; i <= N ; i ++ )
		for( int j = 1 ; j <= 2 ; j ++ )
			if( fm[i].num[j].typ ) In_nxt_list( i , get_tnum_id( fm[i].num[j] ) ) ;
	call_time[0].t = N + 1 ;
}
/*}}}*/

void print_reg( int x ) ;
void print_mem( int x ) ;
void print_r_m_i( int id , int typ ) ;
void print( int id1 , int typ1 , int id2 , int typ2 , char op ) ;

int mem[100005] , mem_rpos[100005] , mem_pos[maxA+maxX+5] , mem_que[100005] , mem_ba , mem_fr , mem_cnt ;
int reg[reg_cnt + 5] , reg_rpos[reg_cnt + 5] , reg_csv[reg_cnt + 5] , reg_pos[maxA+maxX+5] ;
bool reg_change[maxA+maxX+5] ;

void mem_init(){
	mem_ba = 0 , mem_fr = 1 , mem_cnt = 0 ;
}
void reg_init(){
	for( int i = 1 ; i <= reg_cnt ; i ++ )
		reg_csv[i] = N + 2 ;
}

int get_free_mem(){
	if( mem_ba >= mem_fr ){
		mem_pos[ mem_rpos[ mem_que[ mem_fr ] ] ] = 0 ;
		mem_fr ++ ; return mem_que[ mem_fr-1 ] ;
	} else {
		mem_cnt ++ ; return mem_cnt ;
	}
}

int find_used_reg( int exc ){
	static int rt ;
	rt = 0 ;
	for( int i = 1 ; i <= reg_cnt ; i ++ )
		if( reg_csv[i] > reg_csv[rt] && i != exc ) rt = i ;
	return rt ;
}

void del_mem_info( int mem_id ){
	int old_id = mem_rpos[ mem_id ] ;
	if( old_id ){
		// del in mem
		mem_rpos[ mem_id ] = mem_pos[ old_id ] = 0 ;
	} mem_que[++mem_ba] = mem_id ;
}

void wri_reg_info( int reg_id , int new_id ){
	reg_pos[ new_id ] = reg_id ;
	reg_rpos[ reg_id ] = new_id ;
	reg_csv[ reg_id ] = find_nxt_call( new_id ) ;
}

void wri_mem_info( int mem_id , int new_id ){
	mem_pos[ new_id ] = mem_id ;
	mem_rpos[ mem_id ] = new_id ;
}

void del_reg_info( int reg_id ){
	int old_id = reg_rpos[ reg_id ] ;
	if( old_id ){
		reg_rpos[ reg_id ] = reg_pos[ old_id ] = 0 ;
		reg_csv[ reg_id ] = N + 2 ;
	}
}

void del_reg_info_save( int reg_id ){
	int old_id = reg_rpos[ reg_id ] , wor_reg ;
	if( old_id ){
		del_reg_info( reg_id ) ;
		// copy to  reg\memory
		wor_reg = find_used_reg( reg_id ) ;
		if( reg_csv[ wor_reg ] > N ){
			reg[ wor_reg ] = reg[ reg_id ] ;
			wri_reg_info( wor_reg , old_id ) ;
			print( wor_reg , 1 , reg_id , 1 , '=' ) ;
		} else {
			if( !mem_pos[ old_id ] ) wri_mem_info( get_free_mem() , old_id ) ;
		//	fprintf( stderr , "mem at %d , val = %d\n" , global_term , mem[ mem_pos[old_id] ] ) ;
			if( mem[ mem_pos[old_id] ] != reg[ reg_id ] || reg_change[ old_id ] ){
				mem[ mem_pos[old_id] ] = reg[ reg_id ] ;
				reg_change[ old_id ] = false ;
				print( mem_pos[ old_id ] , 2 , reg_id , 1 , '=' ) ;
			} // del in register
		} 
	}
}

int get_free_reg(){
	static int rt ;
	rt = find_used_reg( 0 ) ;
	del_reg_info_save( rt ) ;
	return rt ;
}

void check_print( tnum x , int typ , int id ){
	if( x.typ == 2 ){
		printf( "	printf( \"v[%%d] = %%d\\n\" , %d ," , x.num ) ;
		print_r_m_i( id , typ ) ;
		printf( " ) ;\n" ) ;
	}
	if( x.typ == 1 ){
		printf( "	printf( \"a[%%d] = %%d\\n\" , %d ," , x.num ) ;
		print_r_m_i( id , typ ) ;
		printf( " ) ;\n" ) ;
	}
}

void find_print_then_del( tnum x ){
	static int id ;
	static bool flag ;
	flag = 0 , id = get_tnum_id( x ) ;
	
//	printf( "final value ------|" ) ;
//	printf( x.typ == 1 ? "v" : "a" ) ;
	
	if( reg_pos[ id ] ){
		flag = 1 ;
	//	check_print( x , 1 , reg_pos[id] ) ;
	//	printf( "%d = %d\n" , x.num , reg[ reg_pos[id] ] ) ;
		del_reg_info( reg_pos[ id ] ) ;
	}
	if( mem_pos[ id ] ){
	//	if( !flag ) check_print( x , 2 , mem_pos[id] ) ;
	//	if( !flag ) printf( "%d = %d\n" , x.num , mem[ mem_pos[id] ] ) ;
		del_mem_info( mem_pos[ id ] ) ;
	}
}

void check_last_use( tnum x ){
	if( x.typ == 0 ) return ;
	if( find_nxt_call( get_tnum_id( x ) ) > N )
		find_print_then_del( x ) ;
}

tnum ext_data_to_reg( tnum x ){
	static tnum rt ;
	static int xid ;
	rt.typ = 1 ;
	
	xid = get_tnum_id( x ) ;
	rt.num = reg_pos[xid] ;
	if( !rt.num ) {
		if( !mem_pos[xid] ) puts( "when ext_data_to_reg , fail to find the data" ) , exit( 253 ) ;
		// write to reg
		rt.num = get_free_reg() ;
		reg[ rt.num ] = mem[ mem_pos[xid] ] ;
		wri_reg_info( rt.num , xid ) ;
		print( reg_pos[xid] , 1 , mem_pos[xid] , 2 , '=' ) ;
	} return rt ;
}

tnum find_data_pos( tnum x ){
	static tnum rt ;
	static int xid ;

	if( x.typ == 0 ) return x ;//Á¢¼´Êý

	xid = get_tnum_id( x ) ;
	if( reg_pos[xid] ){
		rt.typ = 1 , rt.num = reg_pos[xid] ;
	} else if( mem_pos[xid] ){
		rt.typ = 2 , rt.num = mem_pos[xid] ;
	} else puts( "can't find the number in find_option!" ) , exit( 252 ) ;
	return rt ;
}

void compute( tnum x , tnum y , tnum z , int op ){
	static int znum ;
	if( !z.typ ) znum = z.num ;
	else if( z.typ == 1 ) znum = reg[z.num] ;
	else znum = mem[z.num] ;

	if( y.typ == 1 ){
		del_reg_info_save( y.num ) ;
		reg[ y.num ] += znum ;
		reg_change[ get_tnum_id( x ) ] = true ;
		wri_reg_info( y.num , get_tnum_id( x ) ) ;
	} else puts( "op_num Y seems not in register, please check it" ) , exit( 254 ) ;

	print( y.num , y.typ , z.num , z.typ , rop[op] ) ;
}

void solve(){
	memset( reg_change , 1 , sizeof( reg_change ) ) ;
	mem_init() , reg_init() ;

	wri_reg_info( 1 , id_st[2] + 1 ) ;
	wri_reg_info( 2 , id_st[2] + 2 ) ;
	wri_reg_info( 3 , id_st[2] + 3 ) ;

	tnum y_data , z_data ;
	for( int i = 1 ; i <= N ; i ++ ){
		global_term = i ;
		// about data : fir-->in reg or mem , second-->index
		y_data = ext_data_to_reg( fm[i].num[1] ) ;

		z_data = find_data_pos( fm[i].num[2] ) ;
		
		compute( fm[i].num[0] , y_data , z_data , fm[i].op ) ;
		
		check_last_use( fm[i].num[1] ) ;
		goto_nxt_call( get_tnum_id( fm[i].num[1] ) ) ;
		if( tnum_no_equal( fm[i].num[1] , fm[i].num[2] ) ){
			check_last_use( fm[i].num[2] ) ;
			goto_nxt_call( get_tnum_id( fm[i].num[2] ) ) ;
		}
		if( tnum_no_equal( fm[i].num[0] , fm[i].num[1] ) && tnum_no_equal( fm[i].num[0] , fm[i].num[2] ) )
			check_last_use( fm[i].num[0] ) ;

		check_print( fm[i].num[0] , 1 , y_data.num ) ;

	}
}

int main(){
	freopen( "in.txt" , "r" , stdin ) ;
	freopen( "my.c" , "w" , stdout ) ;
	printf( "\
#include<stdio.h>\n\
#include<string.h>\n\
int main(){\n\
	freopen( \"my_out.txt\" , \"w\" , stdout ) ;\n\
	int " ) ;

	for( int i = 1 ; i <= 6 ; i ++ ) print_reg( i ) , printf( i < 6 ? ", " : "" ) ;
	printf( ";\n" ) ;

	printf( "\
	int a1 , a2 , a3 , mem[100] ;\n\
	memset( mem , 0 , sizeof( mem ) ) ;\n\n" ) ;

	printf( "\
	scanf( \"%%d%%d%%d\" , &a1 , &a2 , &a3 ) ;\n\
	rdi = a1 , rsi = a2 , rdx = a3 ;\n\n" ) ;

	read_in() ;
	preWork() ;
	solve() ;

	printf( "\
	return 0;\n\
}" ) ;

}

void print_reg( int x ){
	if( x == 1 ) printf( "rdi" ) ;
	else if( x == 2 ) printf( "rsi" ) ;
	else if( x == 3 ) printf( "rdx" ) ;
	else if( x == 4 ) printf( "rcx" ) ;
	else if( x == 5 ) printf( "r8" ) ;
	else if( x == 6 ) printf( "r9" ) ;
	else printf( "r%d" , 3 + x ) ;
}

void print_mem( int x ){
	printf( "mem[%d]" , x ) ;
}

void print_r_m_i( int id , int typ ){
	if( !typ ) printf( "%d" , id ) ;
	else if( typ == 1 ) print_reg( id ) ;
	else if( typ == 2 ) print_mem( id ) ;
	else puts( "print_r_m_i error : invalid typ" ) , exit( 251 ) ;
}

void print( int id1 , int typ1 , int id2 , int typ2 , char op ){
	printf( "	" ) ; 

	print_r_m_i( id1 , typ1 ) ;
	printf( " %c" , op ) ;
	printf( op == '=' ? " " : "= " ) ;
	print_r_m_i( id2 , typ2 ) ;

	printf( " ;" ) ;

	puts( "" ) ;
}

