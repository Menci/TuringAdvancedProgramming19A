#include<stdio.h>
#include<string.h>
int main(){
    double shi=10.0,yi=1.0;
    unsigned long long s,y;
    memcpy(&s,&shi,sizeof(s));
    memcpy(&y,&yi,sizeof(y));
    printf("%llx\n",s);
    printf("%llx\n",y);
}
