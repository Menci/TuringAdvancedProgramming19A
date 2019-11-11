#include<stdio.h>

#define N 1010
#define int long long
struct val{
    int type;
    int key;
}b[N];
struct eql{
    struct val x1,x2,x3;
    char op;
}a[N];
char regu[N][N],mo='%';
void init()
{
    regu[1][0]='%';regu[1][1]='r';regu[1][2]='d';regu[1][3]='i';
    regu[2][0]='%';regu[2][1]='r';regu[2][2]='s';regu[2][3]='i';
    regu[3][0]='%';regu[3][1]='r';regu[3][2]='d';regu[3][3]='x';
    regu[4][0]='%';regu[4][1]='r';regu[4][2]='c';regu[4][3]='x';
    regu[5][0]='%';regu[5][1]='r';regu[5][2]='8';
    regu[6][0]='%';regu[6][1]='r';regu[6][2]='9';
    regu[7][0]='%';regu[7][1]='r';regu[7][2]='a';regu[7][3]='x';
    regu[8][0]='%';regu[8][1]='r';regu[8][2]='1';regu[8][3]='5';
}
int tot, reg, mem;
struct val make_val(int type,int key)
{
    return (struct val){type,key};
}

unsigned char is_same(struct val x1,struct val x2)
{
    return x1.key==x2.key && x1.type==x2.type;
}

int read_int()
{
    char c = getchar();
    int x = 0, p = 1;
    while(c<'0'||c>'9')
    {
        if(c=='-')
            p = -1;
        c = getchar();
    }
    while(c>='0'&&c<='9')
    {
        x = x * 10 + c - '0';
        c = getchar();
    }
    return x*p;
}

char read_op()
{
    char c = getchar();
    while(c!='-' && c!='+' && c!='*' && c!='/')
        c = getchar();
    return c;
}

struct val read_val()
{
    char c=getchar();
    while(c!='v'&&c!='a')
    {
        if((c>='0'&&c<='9')||c=='-')
        {
            ungetc(c, stdin);
            return make_val(0, read_int());
        }
        c=getchar();
    }
    int key;
    key = c=='v'? read_int() + 3 : read_int();
    return make_val(1, key);
}

struct eql read_eql()
{
    struct val x1,x2,x3;
    char op;
    x1 = read_val();
    x2 = read_val();
    op = read_op();
    x3 = read_val();
    return (struct eql){x1, x2, x3, op};
}

int times[N];
int pos[N],type[N];
int sta_a[N], pnt_a;
int sta_b[N], pnt_b;
int rev_a[N], rev_b[N];

void mov(int i, int typ,struct val x);

void print(struct val x)
{
    if(!x.type)
    {
        printf("\t$%lld", x.key);
        return;
    }
    if(type[x.key]==0)
        mov(x.key, 2, x);
    if(type[x.key]==1)
        printf("\t%s", regu[pos[x.key]]);
    else
        printf("\t-%lld(%crbp)", pos[x.key]<<3, mo);

}

void rls(int i)
{
    if(!type[i])
        return;
    if(type[i]==1)
    {
        sta_a[++pnt_a] = pos[i];
        rev_a[pos[i]] = 0;
        pos[i] = type[i] = 0;
        return;
    }
    sta_b[++pnt_b] = pos[i];
    rev_b[pos[i]] = 0;
    pos[i] = type[i] = 0;
    return;
}

void put_eql(struct val x1,struct val x2)
{
    printf("movq");
    print(x2);
    putchar(',');
    print(x1);
    puts("");
}

void eql_rax(struct val x1)
{
    printf("movq\t%crax,", mo);
    print(x1);
    puts("");
}

void rax_eql(struct val x1)
{
    printf("movq");
    print(x1);
    printf(",\t%crax", mo);
    puts("");
}

void ans_xor(struct val x1)
{
    printf("xorq");
    print(x1);
    printf(",\t%cr15",mo);
    puts("");
}

void put_cal(struct val x1,struct val x2,char c)
{
    // putchar('\t');
    if(c!='/')
    {
        switch(c)
        {
            case '-':printf("subq");break;
            case '+':printf("addq");break;
            case '*':printf("imulq");break;
        }
        print(x2);
        putchar(',');
        print(x1);
        puts("");
        return ;
    }
    if(rev_a[3])
        mov(rev_a[3],2,make_val(1,rev_a[3]));
    rax_eql(x1);

    puts("cqto");
    if(x2.type)
    {
        printf("idivq\t");
        print(x2);
        puts("");
    }
    else
    {
        put_eql(make_val(1,64),x2);
        printf("idivq\t");
        print(make_val(1,64));
        puts("");
    }
    
    eql_rax(x1);
}

void make_eql(int i)
{
    if(a[i].x1.type)
        --times[a[i].x1.key];
    if(a[i].x2.type)
        --times[a[i].x2.key];
    if(a[i].x3.type)
        --times[a[i].x3.key];
    if(a[i].op=='+' || a[i].op=='*')
    {
        if(is_same(a[i].x1,a[i].x3))
        {
            mov(a[i].x1.key,1,a[i].x1);
            put_cal(a[i].x1, a[i].x2, a[i].op);
            return;
        }
        if(!is_same(a[i].x1,a[i].x2))
        {
            mov(a[i].x1.key,1,a[i].x2);
        }
        put_cal(a[i].x1, a[i].x3, a[i].op);
        return;
    }
    else
    {
        if(is_same(a[i].x1,a[i].x3))
        {
            pos[0] = pos[a[i].x1.key];
            type[0] = type[a[i].x1.key];
            if(type[0]&1)
            {
                mov(a[i].x1.key,2,a[i].x1);
                put_eql(make_val(1,0),a[i].x2);
                put_cal(make_val(1,0),a[i].x1,a[i].op);
                put_eql(a[i].x1, make_val(1, 0));
            }
            else
            {
                mov(a[i].x1.key,1,a[i].x2);
                // put_eql(a[i].x1,a[i].x2);
                put_cal(a[i].x1,make_val(1,0),a[i].op);
            }
            return;
        }
        if(!is_same(a[i].x1,a[i].x2))
        {
            mov(a[i].x1.key,1,a[i].x2);
            // put_eql(a[i].x1, a[i].x2);
        }
        put_cal(a[i].x1, a[i].x3, a[i].op);
        return;
    }
}

int main()
{
    init();
    char c;
    int flag;
    while(1)
    {
        flag = 1;
        c=getchar();
        while(c!='a'&&c!='v')
        {
            if(c==EOF)
            {
                flag = 0;
                break;
            }
            c=getchar();
        }
        if(!flag)
            break;
        ungetc(c, stdin);
        a[++tot]=read_eql();
        // c=getchar();
    }
    puts("pushq\t%rbp");
    puts("pushq\t%r15");
    puts("movq\t%rsp,\t%rbp");
    puts("movq\t$0,\t%r15");
    for (int i = 6; i >= 1;i--)
        sta_a[++pnt_a]=i;
    mov(1, 1, make_val(1,1));
    mov(2, 1, make_val(1,2));
    mov(3, 1, make_val(1,3));
    for (int i = 1000; i >= 1;i--)
        sta_b[++pnt_b] = i;
    for (int i = 1; i <= tot; i++)
    {
        if(a[i].x1.type)
            ++times[a[i].x1.key];
        if(a[i].x2.type)
            ++times[a[i].x2.key];
        if(a[i].x3.type)
            ++times[a[i].x3.key];
    }
    for (int i = 1; i <= tot;i++)
    {
        make_eql(i);
        ans_xor(a[i].x1);
        if(a[i].x1.type)
            if(!times[a[i].x1.key])
                rls(a[i].x1.key);
        if(a[i].x2.type)
            if(!times[a[i].x2.key])
                rls(a[i].x2.key);
        if(a[i].x3.type)
            if(!times[a[i].x3.key])
                rls(a[i].x3.key);
        
    }
    printf("movq\t%cr15,\t%crax\n",mo,mo);
    puts("popq\t%r15");
    puts("popq\t%rbp");
    puts("ret");
    // printf("\tprintf(");
    // putchar('\"');
    // putchar('%');
    // printf("d\\n");
    // putchar('\"');
    // putchar(',');
    // print(make_val(1, 25));
    // printf(");\n");
    // printf("\treturn");
    // printf(";\n}\n");
    // printf("reg=%lld mem=%lld\n", reg, mem);
    return 0;
}

void mov(int i,int typ,struct val x)
{
    if(type[i]==typ)
    {
        if(i!=x.key)
            put_eql(make_val(1,i),x);
        return;
    }
    if(typ==1)
    {
        if(!pnt_a)
            mov(rev_a[1], 2, make_val(1,rev_a[1]));
        if(x.type)
        {
            pos[0] = pos[x.key];
            type[0] = type[x.key];
        }
        rls(i);
        pos[i] = sta_a[pnt_a--];
        type[i] = typ;
        rev_a[pos[i]] = i;
        if(!type[0])
            return;
        if(x.type)
            put_eql(make_val(1,i),make_val(1,0));
        else
            put_eql(make_val(1,i),x);
        return;
    }
    if(x.type)
    {
        pos[0] = pos[x.key];
        type[0] = type[x.key];
    }
    rls(i);
    // if(pnt_b+sta_b[pnt_b]==1001)
    //     puts("subq\t$8,\t%rsp");
    pos[i] = sta_b[pnt_b--];
    type[i] = typ;
    rev_b[pos[i]] = i;
    if(!type[0])
        return;
    if(x.type)
        put_eql(make_val(1,i),make_val(1,0));
    else
        put_eql(make_val(1,i),x);
    return;
}
/*
        x1 = a3 - 92;
        x2 = a1 / a3;
        x3 = a3 * a2;
        x4 = a2 - a2;
        x5 = 73 + x3;
        x6 = x5 - 51;
        x7 = a1 - x5;
        x8 = a1 - x5;
        x9 = x6 + x6;
        x10 = a1 * 32;
        x8 = x1 * a3;
        x8 = 88 - x3;
        x3 = x6 + x3;
        x10 = x7 - x3;
        x4 = x3 * x4;
        x3 = a3 / a2;
        x4 = x4 * a3;
        x9 = x3 / 47;
        x3 = a3 + x6;
        x3 = 98 * a2;
*/