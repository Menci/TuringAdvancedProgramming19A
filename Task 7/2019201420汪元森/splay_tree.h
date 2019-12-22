// define sth

#ifndef aaaa_a_aa_aaa
//#define aaaa_a_aa_aaa

typedef struct Node{
	struct Node *ch[2] , *fa ;
	int cnt , siz , val ;
} Node ;


typedef struct {
	Node *root ;
	int N ;
} splay ;

Node *newNode( Node **nd , Node *fa , int val ) ;

void update( Node *this ) ;

void Rotate( Node *nd , bool aim ) ;

void Splay( Node *nd , Node *aim ) ;

Node *Insert_root( Node *nd , int val ) ;
void Insert_r( int val ) ;
void Insert( splay *x , int val ) ;

Node *Find( Node *nd , int val ) ;

Node *Find_pre( Node *nd , int val ) ;

Node *Find_nxt( Node *nd , int val ) ;

void Erase_r( int val ) ;
void Erase( splay *x , int val ) ;

Node *Kth_r( int K ) ;
Node *Kth( splay *x , int K ) ;

int Less_cnt_r( int val ) ;
int Less_cnt( splay *x , int val ) ;

void dfs_clear( Node *nd ) ;
void print_tree( Node *nd ) ;
#endif
