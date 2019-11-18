#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const size_t Lim = 15;


void Swap(void *a, void *b, size_t width)

{

	//void *c;

	//c = malloc(width);
	unsigned char tmp;

	if( a != b )

	{
		while( width-- )
		{
			tmp = *(unsigned char *)a;
			*(unsigned char *)a = *(unsigned char *)b;
			*(unsigned char *)b = tmp;
			a++; b++;
		}
		

		//memcpy( c , a , width );

		//memcpy( a , b , width );

		//memcpy( b , c , width );

	}

	//free(c);

}





void SortShort( void *L,void *R,size_t width,

               int (*compar)(const void *, const void *) )

{

	void *p,*Min;

	while( R > L )

	{

		Min=R;

		for( p = R-width; p >= L; p -= width )

			if( compar(p,Min) < 0 ) Min = p;

			Swap(Min,L,width);

			L += width;	

	}

}

void SmallSort( void *L , void *R , void *mid ,size_t width ,

	            int (*compar)(const void *, const void *) )

{

	if( compar( L , mid ) > 0 ) Swap( L , mid , width );

	if( compar( L , R ) > 0 ) Swap( L , R , width );

	if( compar( mid , R ) > 0 ) Swap( mid , R , width );

}
void solve( void *L , void *R , size_t width ,

         	int (*compar)(const void *, const void *) )

{
	void *mid , *ML , *MR; 
	

	size_t size = ( R - L ) / width + 1;
	if( size <= Lim ) 

	{

		SortShort( L , R , width , compar );

		return;

	}

	mid= L + (size / 2 ) * width;

	SmallSort( L , R , mid , width , compar );

	ML = L , MR = R;

	while(1)

	{

		if( mid > ML ) 

		{

			do

			{

				ML += width;

			}while( ML < mid && compar( ML , mid ) <= 0 );

		}

		if( mid <= ML)

		{

			do

			{

				ML += width;

			}while( ML <= R && compar( ML , mid ) <= 0 );

		}

		do

		{

			MR -= width;

		}while( MR > mid && compar( MR , mid ) > 0 ); 

		if( ML > MR ) break;

		Swap( ML , MR , width );

		if( MR == mid ) mid = ML;

	}
	MR += width;

	if( mid < MR )

	{

		do

		{

			MR -= width;

		}while( MR > mid && compar( MR , mid ) == 0 );

	}

	if( mid >= MR )

	{

		do

		{

			MR -= width;

		}while( MR > L && compar( MR , mid ) == 0 );

	} 

	// ри╩С????

	

	if( MR - L >= R - ML )

	{

		if( ML < R ) solve( ML , R , width , compar );

		if( L < MR ) solve( L , MR , width , compar );

	}

	else

	{

		if( L < MR ) solve( L , MR , width , compar );

		if( ML < R ) solve( ML , R , width , compar );

	}

}

int check( void *L, void *R , size_t width ,
           int (*compar)(const void *, const void *) )
{
	if( L == R ) return 1;
	int ans = 1;
	void *N = L + width;
	for(; L != R ; L += width , N += width )
	{
		if( compar( L , N ) > 0 )
		{
			Swap( L , N , width );
			ans = 0;
		}
	}
	return ans;
}

void qsort(void *base, size_t nmemb, size_t width,

           int (*compar)(const void *, const void *))

{

	void *L , *R , *mid;
	
	if( nmemb <= 1 || width == 0 ) return;
	
	L = base;

	R = base + width * (nmemb-1);
	if( check( L , R , width , compar ) ) return;
	solve( L , R , width , compar );	

}




