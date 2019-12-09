#include<stdio.h>

#include<malloc.h>

const int INF = 1e9;
struct node
{
	int value,size,cnt;
	struct node *pre,*ch[2];
}*null,*root;

int max( int a , int b )
{
	return a > b ? a : b;
}
int min( int a , int b )
{
	return a < b ? a : b;
}
void update(struct node *now)
{
	now->size = now->cnt + now->ch[0]->size + now->ch[1]->size;
}
int getlr(struct node *now)
{
	return now->pre->ch[0] == now  ? 0 : 1 ;
}
void setchild(int lr,struct node *now,struct node *child)
{
	now->ch[lr] = child;
	if(child != null) child->pre = now;
	update(now);
}
struct node *getnew(int value)
{
	struct node *now = malloc(sizeof(struct node));
	now->value = value;
	now->size = 1;
	now->cnt = 1;
	now->pre = now->ch[1] = now->ch[0] = null;
	return now;
}
void rotate(struct node *now)
{
	struct node *dad = now->pre,*grandfa = now->pre->pre;
	int lr = getlr(now);
	setchild(lr , dad , now->ch[lr^1]);
	setchild(lr^1 , now , dad);
	now->pre = grandfa;
	if(grandfa != null)
		grandfa->ch[grandfa->ch[0] == dad ? 0 : 1] = now;
}
void splay(struct node *now,struct node *tar)
{
	for( ; now->pre != tar ; rotate(now))
		if( now->pre->pre != tar )
			getlr(now) == getlr(now->pre) ? rotate(now->pre) : rotate(now);
	if( tar == null ) root = now;
}
void insert(int value)
{
	struct node *last = null , *now = root;
	struct node *newone = getnew( value );
	while( now != null )
	{
		last = now;
		if( newone->value == now->value )
		{
			now->size++;
			now->cnt++;
			splay(now,null);
			return;
		}
		if( newone->value < now->value )
			now = now->ch[0];
		else
			now = now->ch[1];
	}
	if( last == null )
		root = newone;
	else
	{
		if( newone->value < last->value )
			setchild( 0 , last , newone );
		else
			setchild( 1 , last , newone );
		splay( newone , null );
	}
}

struct node *find( int value )
{
	struct node *now = root;
	while( now != null )
	{
		if( now->value == value )
			break;
		if( value < now->value )
			now = now->ch[0];
		else 
			now = now->ch[1];
	}
	if( now != null ) splay( now , null );
	return now; 
} 
void del( int value )
{
	struct node *now = find( value );
	if( now == null ) return;
	if( now->cnt > 1 )
	{
		now->cnt--;
		now->size--;
		return;
	}
	if( now->ch[0] == null && now->ch[1] == null )
		root = null;
	else
	{
		if( now->ch[0] == null)
		{
			now->ch[1]->pre = null;
			root = now->ch[1];
		}
		else
		{
			if( now->ch[1] == null)
			{
				now->ch[0]->pre = null;
				root = now->ch[0];
			}
			else
			{
				struct node *A = now->ch[0];
				while( A->ch[1] != null ) A = A->ch[1];
				splay( A , now );
				setchild( 1 , A , now->ch[1]);
				A->pre = null;
				root = A;
			}
		}
	}
	free( now );
}
int rank( int value )
{
	struct node *now = root;
	int l_size = 0;
	while( now != null )
	{
		if( value == now->value )
		{
			int ans = l_size + now->ch[0]->size;
			splay( now , null );
			return ans; 
		}
		if( value < now->value )
			now = now->ch[0];
		else
		{
			l_size += now->ch[0]->size + now->cnt;
			now = now->ch[1]; 
		}
	}
	return l_size;
}
int pre( int value )
{
	int ans = -INF;
	struct node *now =root;
	while( now != null )
	{
		if( now->value < value )
		{
			ans = max( ans , now->value );
			now = now->ch[1];
		}
		else
			now = now->ch[0];
	}
	if( ans == -INF ) ans = -1;
	return ans;
}
int nxt( int value )
{
	int ans = INF;
	struct node *now = root;
	while( now != null )
	{
		if( now->value > value)
		{
			ans = min( ans , now->value );
			now = now->ch[0];
		}
		else
			now = now->ch[1];
	}
	if( ans == INF ) ans = -1;
	return ans;
}
int kth( int k )
{
	struct node *now = root;
	int l_size = 0;
	while( now != null )
	{
		int A = l_size + now->ch[0]->size;
		if( A + 1 <= k && k <= A + now->cnt )
		{
			splay( now , null );
			return now->value;	
		} 
		if( k <= A ) 
			now = now->ch[0];
		else 
			l_size = A + now->cnt ,
			now = now->ch[1];
	}
	return -1;
}
void D( struct node *now )
{
	if( now == null ) return ;
	D( now->ch[1] );
	D( now->ch[0] );
	free(now);
}
void clean()
{
	free( null ); 
	D( root );
}
void init()
{
	null = malloc( sizeof( struct node ) );
	null->value = 0;
	null->size = 0;
	null->cnt = 0;
	null->pre = null->ch[1] = null->ch[0] = null;
	root = null;
}

