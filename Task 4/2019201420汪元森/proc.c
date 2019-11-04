#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#define maxN 60
#define maxX 60 
#define maxA 3 
#define reg_use 7
#define reg_cnt 15
#define type_cnt 3

int call_st[maxX + maxA + 5] , N , head[maxX + maxA + 5] , global_term ;
int id_st[type_cnt + 5] , id_c ;
enum op_num{ Mov , Add , Minus , Mul , Xorq , Div , Push , Pop , Ret , Cqto } ;
enum reg_num{ rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11, r12, r13, r14, r15, rbx, rbp, rsp } ;
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
	if( !call_st[id] ) call_st[id] = call_pt ;// set the first pointer
	if( pre ) call_time[pre].nxt = call_pt ;// set previos' next pointer
	head[id] = call_pt ;
	call_time[call_pt].t = pos ;// set *this information
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
	if( op == '/' ) return 5 ;
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
void print_op_asm( int x ) ;
void print( int op , ... ) ;
int long_size( int x ){ return x << 3 ; }

const int MEM_MX_COST = 30 ;
int mem_rpos[1005] , mem_pos[maxA+maxX+5] , mem_que[1005] , mem_ba , mem_fr , mem_cnt ;
int reg_rpos[reg_cnt + 5] , reg_nxt[reg_cnt + 5] , reg_pos[maxA+maxX+5] , reg_csv[reg_cnt + 5] ;
bool reg_change[maxA+maxX+5] ;

void init(){
	memset( reg_change , 1 , sizeof( reg_change ) ) ;
	memset( mem_pos , -1 , sizeof( mem_pos ) ) ;
	memset( reg_pos , -1 , sizeof( reg_pos ) ) ;

	for( int i = 1 ; i <= reg_use ; i ++ ) reg_nxt[i] = N + 2 ;
	mem_ba = 0 , mem_fr = 1 , mem_cnt = 0 ;

	print( Minus , long_size( MEM_MX_COST ) , 0  , rsp , 1 ) ;
	print( Mov , 0 , 0 , r11 , 1 ) ;
}

void destruc(){
	print( Add , long_size( MEM_MX_COST ) , 0  , rsp , 1 ) ;
	print( Mov , r11 , 1 , rax , 1 ) ;
	print( Ret ) ;
}

int get_free_mem(){
	if( mem_ba >= mem_fr ){
		mem_pos[ mem_rpos[ mem_que[ mem_fr ] ] ] = -1 ;
		mem_fr ++ ; return mem_que[ mem_fr-1 ] ;
	} else {
		mem_cnt ++ ; return mem_cnt ;
	}
}

int find_used_reg( int exc ){
	static int rt ;
	rt = -1 ;
	for( int i = 0 ; i <= reg_use ; i ++ )
		if( reg_nxt[i] > reg_nxt[rt] && i != exc && reg_csv[i] < global_term ) rt = i ;
	if( rt == -1 ) printf( "err, find_used_reg not succeed, rt = -1" ) , exit( 249 ) ;
	return rt ;
}

void del_mem_info( int mem_id ){
	int old_id = mem_rpos[ mem_id ] ;
	if( old_id ){
		mem_rpos[ mem_id ] = 0 , mem_pos[ old_id ] = -1 ;
	} mem_que[++mem_ba] = mem_id ;
}

void wri_reg_info( int reg_id , int new_id ){
	reg_pos[ new_id ] = reg_id ;
	reg_rpos[ reg_id ] = new_id ;
	reg_nxt[ reg_id ] = find_nxt_call( new_id ) ;
}

void wri_mem_info( int mem_id , int new_id ){
	mem_pos[ new_id ] = mem_id ;
	mem_rpos[ mem_id ] = new_id ;
}

void del_reg_info( int reg_id ){
	int old_id = reg_rpos[ reg_id ] ;
	if( old_id ){
		reg_rpos[ reg_id ] = 0 , reg_pos[ old_id ] = -1 ;
		reg_nxt[ reg_id ] = N + 2 ;
	}
}

void del_reg_info_save( int reg_id ){
	int old_id = reg_rpos[ reg_id ] , wor_reg ;
	if( old_id ){
		del_reg_info( reg_id ) ;
		wor_reg = find_used_reg( reg_id ) ;
		if( reg_nxt[ wor_reg ] > N ){
			wri_reg_info( wor_reg , old_id ) ;
			print( Mov , reg_id , 1 , wor_reg , 1 ) ;
		} else {
			if( mem_pos[ old_id ] == -1 ) wri_mem_info( get_free_mem() , old_id ) ;
			if( reg_change[ old_id ] ){
				reg_change[ old_id ] = false ;
				print( Mov , reg_id , 1 , mem_pos[ old_id ] , 2 ) ;
			} // del in register
		}
	}
}

int get_free_reg( int aim ){
	static int rt ;
	rt = ( aim >= 0 ? aim : find_used_reg( -1 ) ) ;
	del_reg_info_save( rt ) ;
	return rt ;
}

void xor_then_del( tnum x ){
	static int id ;
	static bool flag ;
	flag = 0 , id = get_tnum_id( x ) ;

	if( reg_pos[ id ] != -1 ){
		flag = 1 ;
		if( x.typ == 1 ) print( Xorq , reg_pos[id] , 1 , r11 , 1 ) ;
		del_reg_info( reg_pos[ id ] ) ;
	}
	if( mem_pos[ id ] != -1 ){
		if( !flag && x.typ == 1 ) print( Xorq , mem_pos[id] , 2 , r11 , 1 ) ;
		del_mem_info( mem_pos[ id ] ) ;
	}
}

bool check_last_use( tnum x ){
	if( x.typ == 0 ) return 1 ;
	if( find_nxt_call( get_tnum_id( x ) ) > N ){
		xor_then_del( x ) ;
		return 1 ;
	} return 0 ;
}

tnum ext_data_to_reg( tnum x , int aim ){
	static tnum rt ;
	static int xid ;
	rt.typ = 1 ;
	xid = get_tnum_id( x ) ;
	rt.num = reg_pos[xid] ;
	if( mem_pos[xid] == -1 && rt.num == -1 ) puts( "when ext_data_to_reg , fail to find the data" ) , exit( 253 ) ;

	if( aim >= 0 && rt.num >= 0 && aim != rt.num ){
		rt.num = get_free_reg( aim ) ;
		print( Mov , reg_pos[xid] , 1 , aim , 1 ) ;
		del_reg_info( reg_pos[xid] ) ;
		wri_reg_info( rt.num , xid ) ;
	}
	if( rt.num == -1 ) {
		rt.num = get_free_reg( aim ) ;
		wri_reg_info( rt.num , xid ) ;
		print( Mov , mem_pos[xid] , 2 , reg_pos[xid] , 1 ) ;
	} return rt ;
}

tnum find_data_pos( tnum x , int to_save ){
	static tnum rt ;
	static int xid ;

	if( to_save ){ // use mem[0] to save imm_div
		print( Mov , x.num , 0 , 0 , 2 ) ;
		rt.typ = 2 , rt.num = 0 ;
		return rt ;
	}
	if( x.typ == 0 ) return x ;
	xid = get_tnum_id( x ) ;
	if( reg_pos[xid] != -1 ){
		rt.typ = 1 , rt.num = reg_pos[xid] ;
	} else if( mem_pos[xid] != -1 ){
		rt.typ = 2 , rt.num = mem_pos[xid] ;
	} else puts( "can't find the number in find_option!" ) , exit( 252 ) ;
	return rt ;
}

void compute( tnum x , tnum y , tnum z , int op ){
	if( y.typ != 1 ) puts( "op_num Y seems not in register, please check it" ) , exit( 254 ) ;

	del_reg_info_save( y.num ) ;
	reg_change[ get_tnum_id( x ) ] = true ;
	wri_reg_info( y.num , get_tnum_id( x ) ) ;

	print( op , z.num , z.typ , y.num , y.typ ) ;
}

void solve(){
	init() ;
	wri_reg_info( rdi , id_st[2] + 1 ) ;
	wri_reg_info( rsi , id_st[2] + 2 ) ;
	wri_reg_info( rdx , id_st[2] + 3 ) ;

	int op ;
	tnum y_data , z_data , num1 , num2 ;
	for( int i = 1 ; i <= N ; i ++ ){
		num1 = fm[i].num[1] , num2 = fm[i].num[2] , op = fm[i].op ;
		global_term = i ;
		// about data : fir-->in reg or mem , second-->index
		if( fm[i].op == Div ) reg_csv[ rax ] = reg_csv[ rdx ] = global_term ;
		y_data = ext_data_to_reg( num1 , op == Div ? rax : -1 ) ;
		if( fm[i].op == Div ){ // make sure z_data not in rdx
			del_reg_info_save( rdx ) ;
			print( Cqto ) ;
		}
		z_data = find_data_pos( num2 , op == Div && num2.typ == 0 ? 1 : 0 ) ;
		compute( fm[i].num[0] , y_data , z_data , op ) ;
		
		if( !check_last_use( num1 ) )
			goto_nxt_call( get_tnum_id( num1 ) ) ;
		if( tnum_no_equal( num1 , num2 ) && !check_last_use( num2 ) )
			goto_nxt_call( get_tnum_id( num2 ) ) ;
		if( tnum_no_equal( fm[i].num[0] , num1 ) && tnum_no_equal( fm[i].num[0] , num2 ) )
			check_last_use( fm[i].num[0] ) ;
	}
	destruc() ;
}

int main(){
	//freopen( "2.txt" , "r" , stdin ) ;
	//freopen( "my.c" , "w" , stdout ) ;
	read_in() ;
	preWork() ;
	solve() ;
}

void print_op_asm( int x ){
	switch( x ){
		case 0 : printf( "movq " ) ; break ;
		case 1 : printf( "addq " ) ; break ;
		case 2 : printf( "subq " ) ; break ;
		case 3 : printf( "imulq " ) ; break ;
		case 4 : printf( "xorq " ) ; break ;
		case 5 : printf( "idivq " ) ; break ;
		case 6 : printf( "push " ) ; break ;
		case 7 : printf( "pop " ) ; break ;
		case 8 : printf( "ret " ) ; break ;
		case 9 : printf( "cqto " ) ; break ;
	}
}

void print_reg( int x ){
	switch( x ){
		case 0 : printf( "%%rax" ) ; break ;
		case 1 : printf( "%%rdi" ) ; break ;
		case 2 : printf( "%%rsi" ) ; break ;
		case 3 : printf( "%%rdx" ) ; break ;
		case 4 : printf( "%%rcx" ) ; break ;
		case 13 : printf( "%%rbx" ) ; break ;
		case 14 : printf( "%%rbp" ) ; break ;
		case 15 : printf( "%%rsp" ) ; break ;
		default : printf( "%%r%d" , 3 + x ) ; // r8 to r15
	}
}

void print_mem( int x ){
	printf( "%d(%%rsp)" , long_size( x ) ) ;
}

void print_r_m_i( int id , int typ ){
	if( !typ ) printf( "$%d" , id ) ;
	else if( typ == 1 ) print_reg( id ) ;
	else if( typ == 2 ) print_mem( id ) ;
	else puts( "print_r_m_i error : invalid typ" ) , exit( 251 ) ;
}

void print( int op , ... ){
	int dest , typ1 , src , typ2 ;
	va_list arg_list ;
	va_start( arg_list , op ) ;

	printf( "	" ) ; 
	print_op_asm( op ) ;

	if( op <= 7 ){
		src = va_arg( arg_list , int ) ;
		typ1 = va_arg( arg_list , int ) ;
		print_r_m_i( src , typ1 ) ;
	}

	if( op <= 4 ){
		printf( ", " ) ;
		dest = va_arg( arg_list , int ) ;
		typ2 = va_arg( arg_list , int ) ;
		if( typ2 == 0 ) printf( "err, Dest is a number" ) , exit( 250 ) ;
		print_r_m_i( dest , typ2 ) ;
	} 
	
	puts( "" ) ;
}

