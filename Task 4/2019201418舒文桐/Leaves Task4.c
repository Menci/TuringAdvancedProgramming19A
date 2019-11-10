#include<stdio.h> 

#include<stdlib.h>

#include<string.h>

#include<ctype.h>

#define o 2005



const int lim=103;

char A[101];

int tm;

int R[o],Belong[o],L[o],T[o][3],V[o],M[o];

int top,W;

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

void pp(int x){

	if(x==0) printf("%%rcx");

	if(x==1) printf("%%rsi");

	if(x==2) printf("%%rdi");

	if(x==3) printf("%%r8");

}

int su(int a){

	return Belong[a] != a;

}

int finde(int a,int b){
	

	int i;
	if(!(1<=a&&a<=63)) a=100;
	if(!(1<=b&&b<=63)) b=100;
	for( i = 100 ; i <= lim ; i++) if(!R[i]) return i;

	for( i = 100 ; i <= lim ; i++) if( i != Belong[a] && i != Belong[b] && L[ R[i] ] < tm) {

		Belong[R[i]] = R[i]; 

		R[i] = 0; 

		return i;

	}

	for( i = 100 ; i <= lim ; i++) if( i != Belong[a] && i != Belong[b]) {

		printf("movq ");

		pp(i-100);

		printf(" , ");

		printf("%d(%%rsp)\n",R[i]*8);

		Belong[R[i]] = R[i]; 

		R[i] = 0; 

		return i;

	}

} 

int Beifen(int a){

	int i = Belong[a];

	if( a == i ) return 0;

	

	

	Belong[R[i]] = R[i]; 

	R[i] = 0; 

	if( L[a] <= tm) return i;

	printf("movq ");

	pp(i-100);

	printf(" , ");

	printf("%d(%%rsp)\n",R[i]*8);

	return i;

}



void prt(int b){

	if( b >= 61 && b <= 63){

		if(b==61) printf("%%r9");

		if(b==62) printf("%%r10");

		if(b==63) printf("%%r11");

	}

	if( b >= 64 ) printf("$%d",V[b]);

	if( b <= 60 ) {

		if(su(b)) 

			pp(Belong[b]-100);

		else

			printf("%d(%%rsp)",b*8);

	}

}

void init(){

	printf("movq $0 , %%r8\n");

	int i;

	for(i=1;i<=61;i++) printf("pushq %%r8\n");

	printf("movq %%rdi , %%r9\n");

	printf("movq %%rsi , %%r10\n");

	printf("movq %%rdx , %%r11\n");

} 

void PRT(int x,int b,char c){

	if(c=='+'){

		printf("addq ");

		prt(b);

		printf(" , ");

		pp(x);

		printf("\n");

	}

	if(c=='-'){

		printf("subq ");

		prt(b);

		printf(" , ");

		pp(x);

		printf("\n");

	}

	if(c=='*'){

		printf("imulq ");

		prt(b);

		printf(" , ");

		pp(x);

		printf("\n");

	}

	if(c=='/'){

	

		printf("movq ");

		pp(x);

		printf(" , %%rax\n");
		printf("cqto\n");
		if(b<64){
			printf("idivq ");
			prt(b);
			printf("\n");
		}
		else{
			int w=finde(R[x],b);
			printf("movq "); 
			printf("$%d , ",V[b]);
			pp(w-100);printf("\n");
			printf("idivq ");
			pp(w-100);
			printf("\n");
		}
		

		

	}

}

int main(){

	//freopen("1.txt","r",stdin);

	//freopen("ans.out","w",stdout);

	top = 170;

	int i , j;

	char c;

	int ll=60;

	init();

	

	for( i = 1 ; i <= ll ; i++ ) {

		scanf("%c", &c );

	//	printf("%c",c);

		scanf("%d %c ", &T[i][0] , &c );

		if( T[i][0] <= 60 ) L[T[i][0]] = i;

		T[i][1] = check();

		if( T[i][1] <= 60 ) L[T[i][1]] = i;

		scanf("%c ", &F[i]);

		T[i][2] = check();

		if( T[i][2] <= 60 ) L[T[i][2]] = i;

		Belong[i] = i;

		getchar();

	//	printf("%d %d %d\n",T[i][0],T[i][1],T[i][2]);

	}

	L[0] = 61;

	//R: 100~115 R[15]  

	for( i = 1 ; i <= ll ; i++ ) {

		tm = i;

		for( j = 100 ; j <= lim ; j++ )  if(L[ R[j] ] < i ) {

			Beifen(R[j]);

		} 

		int a , b , c;
		W=i;


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

				PRT(x-100,b,F[i]);			

			}

			else{

				x = finde( a , b );

				printf("movq "); 

				printf("%d(%%rsp) , ",a*8);

				pp(x-100);printf("\n");

				PRT(x-100,b,F[i]);	

			}

		}

		if( a > 60 && b <= 60){
			if(Belong[b]!=b){
				R[Belong[b]]=0;
				printf("movq ");
				pp(Belong[b]-100);		
				printf(" , ");
				printf("%d(%%rsp)\n",b*8);
				Belong[b]=b;
			}

			x = finde( a , b );

			printf("movq "); 

			prt(a);

			printf(" , ");

			pp(x-100);printf("\n");

			PRT(x-100,b,F[i]);

		}

		if( a > 60 && b > 60) {
			

			x = finde( a , b );

			printf("movq "); 

			prt(a);

			printf(" , ");

			pp(x-100);printf("\n");

			PRT(x-100,b,F[i]);

		}
		if(F[i]=='/'){
			R[x] = 0;

			Belong[c] = c;

			printf("movq %%rax , ");

			printf("%d(%%rsp)\n",c*8);
			continue;
		}

		R[x] = c;

		Belong[c] = x;

		printf("movq ");

		pp(x-100);

		printf(" , ");

		printf("%d(%%rsp)\n",c*8);

	}
	printf("movq $0 , %%rax\n");

	for(i=1;i<=ll;i++) 

	printf("XORq %d(%%rsp) , %%rax\n",i*8);
	for(i=1;i<=61;i++) printf("popq %%r8\n");
	puts("ret");
	return 0;

}
