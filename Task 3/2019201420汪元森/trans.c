#include <stdio.h>
#include <string.h>
int main(){
	freopen( "in.txt" , "r" , stdin ) ;
	freopen( "std.c" , "w" , stdout ) ;
	printf( "\
#include<stdio.h>\n\
#include<string.h>\n\
int main(){\n\
	int " ) ;

	for( int i = 1 ; i <= 60 ; i ++ )
		printf( "v%d " , i ) , printf( i == 60 ? ";\n" : "," ) ;

	printf( "\
	int a1 , a2 , a3 ;\n\
	scanf( \"%%d%%d%%d\" , &a1 , &a2 , &a3 ) ;\n" ) ;

	char ss[1005] ;
	for( int i = 1 ; i <= 60 ; i ++ )
		gets( ss ) , printf( "	" ) , puts( ss ) ;

	for( int i = 1 ; i <= 60 ; i ++ )
		printf( "	printf( \"v[%%d] = %%d\\n\" , %d , v%d ) ;\n" , i , i ) ;
	printf( "\
	return 0 ;\n\
}" ) ;
}
