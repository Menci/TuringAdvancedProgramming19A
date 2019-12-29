

//写个宏定义K 


#define K 4
#define uc unsigned char 

#include<stdio.h>
#include<stdlib.h>
#define o 3500105
#define W o*(K+10)
struct node{
	int lson,rson;
	uc data[K];
}Tree[o];

int root,top,m,hp_top,ez,fd,qtop,Anstop,hf_root;
int cnt[o],heap[o*4],L[o],R[o],Q[W],FD[o],father[o];
int vis[o];
uc S[W],Ans[W];
int cmp( int a , int b )
{
	return cnt[a] < cnt[b]; 
}
void push( int now )
{
	int x = hp_top++;
	while( x > 0 )
	{
		int fa = ( x - 1 ) >> 1;
		if( !cmp( now , heap[fa] ) ) break;
		heap[x] = heap[fa];
		x = fa;
	}
	heap[x] = now;
}
int pop()
{
	int ans = heap[0];
	int now = heap[ --hp_top ];
	heap[hp_top] = 0;
	int x = 0;
	while( (x<<1|1) < hp_top )
	{
		int a = (x<<1|1) , b = a + 1;
		if( b < hp_top && cmp( heap[b] , heap[a] ) ) 
			a = b;
		if( cmp( now , heap[a] ) ) break;
		heap[x] = heap[a];
		x = a;
	}
	heap[x] = now;
	return ans;
}
void gt (int x )
{
	Q[++qtop] = x;
}
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
 

//先试着把括号看做字母一块哈夫曼- 
void Solve( int now ) //左右根 根后打结束标记 
{
	//printf("** %d\n" , now);
	if( Tree[now].lson != -1 )
	{
		S[ ++top ] = '(';
		Solve( Tree[now].lson );
	} 
	if( Tree[now].rson != -1 )
	{
		S[ ++top ] = ')';
		Solve( Tree[now].rson );
	}
	S[ ++top ] = '*'; 
	for( int i = 0 ; i < K ; i++ ) S[ ++top ] = Tree[now].data[i];
	
}
void build_Huffman()
{
	for( int i = 1 ; i <= top ; i++ )
		cnt[ (int)S[i] ]++;
	for( int i = 0 ; i <= 300 ; i++ ) if( cnt[i] ) push(i);
	ez = 310;
	for( int i = 0 ; i <= 300 ; i++ )
	 father[i] = -1 , L[i] = R[i] = -1;
	while( hp_top >= 2 )
	{
		int a = pop() , b = pop();
		ez++;
		father[a] = father[b] = ez;
		father[ez] = -1;
		L[ez] = a; R[ez] = b; cnt[ez] = cnt[a] + cnt[b];
		push(ez);
	}
	hf_root = pop();
}
void Print_Huffman( int now )
{
	if( now > 300 ) 
	{
		gt(0);
		Print_Huffman( L[now] );
		Print_Huffman( R[now] );
	}
	else
	{
		gt(1);
		for( int i = 7 ; i >= 0 ; i-- ) gt( ( now >> i ) & 1 );
	}
}

void Data_gt()
{
	int now;
	for( int i = 1 ; i <= top ; i ++ )
	{
		fd = 1; FD[1] = (int)S[i];
		now = FD[1];
		while( father[now] != -1)
		{
			now = father[now];
			FD[++fd] = now;
		}
		for( int j = fd-1 ; j >=1 ; j-- )
		{
			now = FD[j];
			if( L[father[now]] == now )
				gt(0);
			else
				gt(1);
		}
	} 
}
void PrintAns()
{
	int morebit = 8 - qtop % 8;
	if( morebit == 8 ) morebit = 0;
	for( int i = 0 ; i <= 3 ; i ++ ) Q[ 4-i ] = (morebit >> i) & 1;
	qtop += morebit;
	Anstop = 0;
	int x;
	for( int i = 1 ; i <= qtop ; i += 8 )
	{
		x = 0;
		for( int j = 0 ; j <= 7 ; j++ ) x = (x<<1) | Q[i+j];
		Ans[++Anstop] = (uc)x;
	}
	
	FILE *p;
	p = fopen( "input.bin" , "wb" );
	fwrite( &Ans[1] , sizeof(Ans[0]) , Anstop , p );
}
void Check()
{
	printf("\n");
	for(int i=1;i<=3;i++){
		printf("%d %d ",Tree[i].lson,Tree[i].rson);
		for(int j=0;j<K;j++)printf("%d",(int)Tree[i].data[j]);
		printf("\n");
	} 
}
int main()
{
	FILE *p;
	p = fopen( "data.in" , "rb" );
	int size = getfilesize( p );
	p = fopen( "data.in" , "rb" );
	int count = size / ( sizeof(struct node) ); 
	int n = fread( Tree , sizeof(struct node) , count , p );
	
	for(int i = 0 ; i < n ; i++ ) 
{
	if( Tree[i].lson != -1 ) vis[ Tree[i].lson ] = 1;
	if( Tree[i].rson != -1 ) vis[ Tree[i].rson ] = 1;
}
	for(int i = 0 ; i < n ; i++ ) 
		if( !vis[i] ) root = i;
	Solve(root);
	build_Huffman();
	qtop = 4;
	Print_Huffman(hf_root);
	Data_gt();
	PrintAns();
	return 0;
}
 
