#include<stdio.h>
#define ll long long
ll s[100100];
void test(ll a1,ll a2,ll a3){
    ll ch,b1,b2,c1,c2,fh,rax;
    while(1){
        b1 = b2 = c1 = c2 = 0;
        while((ch = getchar()) != ' '){
            if(ch == EOF) return;
            if(ch == 'a') b1 = 1;
            if(ch <= '9' && ch >= '0') b2 = b2 * 10 + ch - '0';
        }
        while((ch = getchar()) != ' ');
        while((ch = getchar()) != ' '){
            if(ch == 'v') c1 = 1;
            if(ch == 'a') c1 = 2;
            if(ch <= '9' && ch >= '0') c2 = c2 * 10 + ch - '0';
        }
        if(c1 == 1){
            printf("rax = s[%d];\n",c2);
            rax = s[c2];
        }
        if(c1 == 2){
            if(c2 == 1){
                puts("rax = a1;");
                rax = a1;
            }
            if(c2 == 2){
                puts("rax = a2;");
                rax = a2;
            }
            if(c2 == 3){
                puts("rax = a3;");
                rax = a3;
            }
        }
        c2 = c1 = 0;
        while((ch = getchar()) != ' '){
            if(ch == '+') fh = 1;
            if(ch == '-') fh = 2;
            if(ch == '*') fh = 3;
            if(ch == '/') fh = 4;
        }
        while((ch = getchar()) != '\n'){
            if(ch == 'v') c1 = 1;
            if(ch == 'a') c1 = 2;
            if(ch == '-') c1 += 10;
            if(ch <= '9' && ch >= '0') c2 = c2 * 10 + ch - '0';
        }
        if(c1 >= 10) c1 -= 10, c2 *= -1;
        if(c1 == 0){
            printf("c2 = %d;\n",c2);
        }
        if(c1 == 1){
            printf("c2 = s[%d];\n",c2);
            c2 = s[c2];
        }
        if(c1 == 2){
            if(c2 == 1){
                printf("c2 = a1;\n");
                c2 = a1;
            }
            if(c2 == 2){
                printf("c2 = a2;\n");
                c2 = a2;
            }
            if(c2 == 3){
                printf("c2 = a3;\n");
                c2 = a3;
            }
        }
        if(fh == 1){
            puts("rax += c2;");
            rax += c2;
        }
        if(fh == 2){
            puts("rax -= c2;");
            rax -= c2;
        }
        if(fh == 3){
            puts("rax *= c2;");
            rax *= c2;
        }
        if(fh == 4){
            puts("rax /= c2;");
            rax /= c2;
        }
        if(b1 == 1){
            if(b1 == 1){
                puts("a1 = rax;");
                a1 = rax;
            }
            if(b1 == 2){
                puts("a2 = rax;");
                a2 = rax;
            }
            if(b1 == 3){
                puts("a3 = rax;");
                a3 = rax;
            }
        }
        else{
            printf("s[%d] = rax;\n",b2);
            s[b2] = rax;
        }
    }
}
int main(){
    freopen("1.txt","r",stdin);
    freopen("ans.txt","w",stdout);
    puts("include<stdio.h>");
    puts("int test(long long a,long long b,long long c){");
    puts("long long c2,rax,s[62]");
    test(1,1,1);
    puts("}");
    puts("int main(){");
    puts("}");
}
