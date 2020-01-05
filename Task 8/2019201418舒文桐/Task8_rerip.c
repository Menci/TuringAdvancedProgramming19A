
#define K 4
#define uc unsigned char

#include<stdio.h>
#include<stdlib.h>
#define o 3500105
#define W o*(K+10)
struct node{
	int lson,rson;
	uc data[K];
}Tree[o],RealTree[o];
int top,cnt,stop,rttop,hf_root,morebit,rtsz,n,root,realroot;
int  V[o],L[o],R[o],vis[o];
uc Ans[W],S[W];
int getfilesize( FILE *p )
{
	int size = 0;
	if( p == NULL )
	{
		puts("文件打开错误");
		return -1;
	}
	fseek( p , 0 , SEEK_END );
	size = ftell( p );
	fclose( p );
	p = NULL;
	return size;
}
int gt()
{
	int x = top / 8 ;
	if( top % 8 ) x++; 
	int y = 8 - top % 8;
	if( top % 8 == 0 ) y = 0;
	return ( ((int)Ans[x]) >> y ) & 1;
}
int build_Huffman()
{
	top++;
	
	int x = gt() , now = ++cnt;
	V[now] = -1;
	if( x == 0 )
	{
		L[now] = build_Huffman();
		R[now] = build_Huffman();
	}
	else
	{
		int v = 0;
		for( int j = 0 ; j <= 7 ; j++ ) 
		{
			top++;
			v = (v<<1) | gt();
		}
		V[now] = v;
	}
	
	return now;
}
int find( int now )
{
	if( V[now] != -1 ) return V[now];
	top++;
	if( gt() )
		return find( R[now] );
	else
		return find( L[now] );
}
void Data_get()
{
	while( top < n*8 - morebit ) {
		S[ ++stop ] = (uc)find( hf_root );
		
	}
		
}
int RecoverTree()
{
	int now = ++rtsz; 
	Tree[now].lson = Tree[now].rson = -1;
	if( S[rttop] == '(' )
	{
		++rttop;
		Tree[now].lson = RecoverTree();
	}
		
	if( S[rttop] == ')' )
	{
		++rttop;
		Tree[now].rson = RecoverTree();	
	}
		
	for( int i = 0 ; i < K ; i++ ) Tree[now].data[i] = S[++rttop];
	++rttop;
	return now;
}
int CC( int a , int b )
{
	int v = 0 ;
	for( int i = 0 ; i < K ; i++ ) if( RealTree[a].data[i] == Tree[b].data[i] ) v++;
	return v == K ;
} 
int dfs( int a , int b )
{
	if( a == -1 && b == -1 ) return 1;
	if( a == -1 || b == -1 ) return 0;
	if( !CC( a , b ) ) return 0;
	if( !dfs( RealTree[a].lson , Tree[b].lson ) ) return 0;
	if( !dfs( RealTree[a].rson , Tree[b].rson ) ) return 0;
	
	return 1;
}
int Check()
{

	FILE *p;
	p = fopen( "data.in" , "rb" );
	int size = getfilesize( p );
	p = fopen( "data.in" , "rb" );
	int count = size / ( sizeof(struct node) ); 
	int rn = fread( RealTree , sizeof(struct node) , count , p );
	n = rtsz;
	if( n != rn ) return 0;
	for(int i = 0 ; i < n ; i++ ) 
{
	if( RealTree[i].lson != -1 ) vis[ RealTree[i].lson ] = 1;
	if( RealTree[i].rson != -1 ) vis[ RealTree[i].rson ] = 1;
}
	for(int i = 0 ; i < n ; i++ ) 
		if( !vis[i] ) realroot = i;
	return dfs(realroot,root);
}
int main()
{
	FILE *p;
	p = fopen( "input.bin" , "rb" );
	int size = getfilesize( p );
	p = fopen( "input.bin" , "rb" );
	int count = size / ( sizeof(Ans[0]) ); 
	n = fread( &Ans[1] , sizeof(Ans[0]) , count , p );
	top = 4;
	morebit = ((int)Ans[1]) >> 4;
	hf_root = build_Huffman();
	Data_get();
	rttop = 1;
	
	root = RecoverTree();
//	if( Check() ) 
//		puts("YES");
//	else 
//		puts("NO");
	return 0;
}
