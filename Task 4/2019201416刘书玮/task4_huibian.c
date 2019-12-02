#include<stdio.h>
#define ll long long
int main(){
    //freopen("1.txt","r",stdin);
    ll rdi,rdx,rsi;
    ll ch,b1,b2,c1,rax,fh,rcx,rbp,i,r8,r9,r10;
    for(i=0;i<61;i++) puts("pushq $0");
    puts("movq  %rdx,%r8");
    while(1){
        b1 = b2 = c1 = rax = 0;
        while((ch = getchar()) != ' '){
            if(ch == EOF){
                puts("movq  $0,%rax");
                for(i=1;i<=60;i++){
                    printf("xorq  %lld(%%rsp),%%rax\n",i*8-8);
                }
                for(int i=0;i<61;i++) puts("popq  %r8");
                puts("ret");
                return 0;
            }
            if(ch <= '9' && ch >= '0') b2 = b2 * 10 + ch - '0';
        }
        while((ch = getchar()) != ' ');
        while((ch = getchar()) != ' '){
            if(ch == 'v') c1 = 1;
            if(ch == 'a') c1 = 2;
            if(ch <= '9' && ch >= '0') rax = rax * 10 + ch - '0';
        }
        if(c1 == 1){
            printf("movq  %lld(%%rsp),%%rcx\n",8*rax-8);
        }
        if(c1 == 2){
            if(rax == 1){
                puts("movq  %rdi,%rcx");
            }
            else if(rax == 2){
                puts("movq  %rsi,%rcx");
            }
            else if(rax == 3){
                puts("movq  %r8,%rcx");
            }
        }
        rax = c1 = 0;
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
            if(ch <= '9' && ch >= '0') rax = rax * 10 + ch - '0';
        }
        if(c1 >= 10) c1 -= 10, rax *= -1;
        if(c1 == 0){
            printf("movq  $%lld,%%rax\n",rax);
        }
        if(c1 == 1){
            printf("movq  %lld(%%rsp),%%rax\n",8*rax-8);
        }
        if(c1 == 2){
            if(rax == 1){
                puts("movq  %rdi,%rax");
            }
            else if(rax == 2){
                puts("movq  %rsi,%rax");
            }
            else if(rax == 3){
                puts("movq  %r8,%rax");
            }
        }
        if(fh == 1) puts("addq  %rax,%rcx");
        if(fh == 2) puts("subq  %rax,%rcx");
        if(fh == 3) puts("imulq %rax,%rcx");
        if(fh == 4){
            puts("movq  %rax,%r10");
            puts("movq  %rcx,%rax");
            puts("cqto");
            puts("idivq %r10");
            puts("movq  %rax,%rcx");
        }
        printf("movq  %%rcx,%lld(%%rsp)\n",8*b2-8);
    }
    return 0;
}

