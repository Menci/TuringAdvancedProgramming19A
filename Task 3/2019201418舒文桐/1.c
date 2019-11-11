#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define o 2005

const int lim=106;
char A[101];

int R[o],Belong[o],L[o],T[o][3],V[o],M[o];
int top;
char F[o];
int check(){
	
	int len = 0;
	char c;
	while(1){
		c = getchar();
		if(c == ' ' || c == ';') break;
		A[len++] = c;
	}
	int h = 0,s = 1;
	if(A[0]=='-') h++,s = -1; 
	if( isdigit( A[h] ) ) {
		int i , v = 0;
		for( i = h ; i < len ; i++) v = v * 10 + A[i]-'0';
		V[ ++top ] = s*v;
		return top;
	}
	else {
		if( A[h] == 'a') 
			return 60 + A[h+1] - '0' ;
		else {
			int i , v = 0;
			for( i = h+1 ; i < len ; i++) v = v * 10 + A[i]-'0';
			return v;
		}
		
	}
}
int su(int a){
	return Belong[a] != a;
}
int find(int a,int b){
	int i;
	for( i = 100 ; i <= lim ; i++) if(!R[i]) return i;
	for( i = 100 ; i <= lim ; i++) if( i != Belong[a] && i != Belong[b]) {
		printf("M[%d] = R[%d];\n",R[i],i-100);
		Belong[R[i]] = R[i]; 
		R[i] = 0; 
		return i;
	}
} 
int Beifen(int a){
	int i = Belong[a];
	if( a == i ) return 0;
	printf("M[%d] = R[%d];\n",R[i],i-100);
	Belong[R[i]] = R[i]; 
	R[i] = 0; 
	return i;
}
int prt(int b){
	if( b >= 61 && b <= 63) printf("a%d;\n",b-60); 
	if( b >= 64 ) printf("%d;\n",V[b]);
	if( b <= 60 ) {
		if(su(b)) 
			printf("R[%d];\n",Belong[b]-100);
		else
			printf("M[%d];\n",b);
	}
}
int main(){
	freopen("2.in","r",stdin);
	freopen("ans.out","w",stdout);
	top = 170;
	int i , j;
	char c;
	//printf("*****\n");
	for( i = 1 ; i <= 60 ; i++ ) {
		scanf("%c", &c );
		scanf("%d %c ", &T[i][0] , &c );
		if( T[i][0] <= 60 ) L[T[i][0]] = i;
		T[i][1] = check();
		if( T[i][1] <= 60 ) L[T[i][1]] = i;
		scanf("%c ", &F[i]);
		T[i][2] = check();
		if( T[i][2] <= 60 ) L[T[i][2]] = i;
		Belong[i] = i;
		getchar();
		//printf("%d %d %d\n",T[i][0],T[i][1],T[i][2]);
	}
	L[0] = 61;
	//return 0;
	//R: 100~115 R[15]  
	for( i = 1 ; i <= 60 ; i++ ) {
		for( j = 100 ; j <= lim ; j++ )  if(L[ R[j] ] < i ) {
			Beifen(R[j]);
		} 
		int a , b , c;

		a = T[i][1];
		b = T[i][2];
		c = T[i][0];
		//if(F[i] == '+') V[c] = V[a] + V[b];
		//if(F[i] == '*') V[c] = V[a] * V[b];
		//if(F[i] == '/') V[c] = V[a] / V[b];
		//if(F[i] == '-') V[c] = V[a] - V[b];
		int x;
		if( a <= 60 ){
			if( su(a) ) {
				x = Beifen(a);
				printf("R[%d] %c= ",x-100,F[i]);
				prt(b);
			}
			else{
				x = find( a , b );
				printf("R[%d] = M[%d];\n" , x-100 , Belong[a] );
				printf("R[%d] %c= ",x-100,F[i]);
				prt(b);
			}
		}
		if( a > 60 && b <= 60){
			x = find( a , b );
			printf("R[%d] = " , x-100 );
			prt(a);
			printf("R[%d] %c= ",x-100,F[i]);
			prt(b);
			
		}
		if( a > 60 && b > 60) {
			x = find( a , b );
			printf("R[%d] = " , x-100 );
			prt(a);
			printf("R[%d] %c= ",x-100,F[i]);
			prt(b);
		}
		
		R[x] = c;
		Belong[c] = x;
			
		
	}
	for( i = 1 ; i <= 60 ; i++ ) if(su(i)) Beifen(i);
}
