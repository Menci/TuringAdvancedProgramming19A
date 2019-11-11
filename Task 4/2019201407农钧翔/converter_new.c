#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define N 105
#define LL long long

int n, m;
int state[1024], var[N], reg_cnt = 16; //var: -1:nowhere; state: 0~15: reg 16+:mem 0:empty
int X[N], Y[N], Z[N], op[N];
int cnt[N];
int tot_mem = 15;
LL Z_val[N];

char ans[1000005];

enum _type { IMM = -2, REG = -1, MEM = 0 };
enum _KEY { MOV = 0, ADD, SUB, MUL, DIV, XOR};

char key_name[8][8] = {
    "movq ", "addq ", "subq ", "imulq ", "idivq ", "xorq "
};

char reg_name[16][8] = {
    "%rax", "%rbx", "%rcx", "%rdx", 
    "%rsi", "%rdi", "%rbp", "%rsp",
    "%r8" , "%r9" , "%r10", "%r11",
    "%r12", "%r13", "%r14", "%r15"
};

int read(LL* val) {
    char str[32];
    scanf("%s", str);
    if(('9' >= str[0] && str[0] >= '0') || str[0] == '-') return *val = atoi(str), IMM;
    if(str[0] == 'a') return n + atoi(str + 1);
    return atoi(str + 1);
}

void get_name(char str[], int i, LL val) {
    if(i == IMM) return (void) sprintf(str, "$%lld", val);
    if(i == REG) return (void) strcpy(str, reg_name[val]);
    if(i == MEM) return (void) sprintf(str, "%lld(%%rbp)", -8 * (val - 16 + 1));
    if(var[i] < 16) return (void) strcpy(str, reg_name[var[i]]); 
    return (void) sprintf(str, "%d(%%rbp)", -8 * (var[i] - 16 + 1));
}

void output(int k, int x, LL xval, int y, LL yval) {
    char str[32];
    strcat(ans, key_name[k]);
    switch(k) {
        case DIV:
            get_name(str, x, xval); strcat(ans, str); 
            strcat(ans, "\n");
            break;
        default:
            get_name(str, x, xval); strcat(ans, str);
            strcat(ans, ", ");
            get_name(str, y, yval); strcat(ans, str);
            strcat(ans, "\n");
            break;
    }
}

int new_mem() {
    for(int i = 16; i < 1024; i++) if(state[i] != 0) if(i > tot_mem) tot_mem = i;//, printf("i = %d, state[i] = %d\n", i, state[i]);
    for(int i = 16; i < 1024; i++) if(state[i] == 0) return (i > tot_mem) ? tot_mem = i : i;
}

void resolve(int id) {
    if(var[id] != -1) return ;
    if(reg_cnt == 0) {
        int m_id = new_mem();
        state[m_id] = id;
        var[id] = m_id;
    }
    else {
        for(int i = 0; i < 16; i++) {
            if(state[i] == -1) continue;
            if(state[i] == 0) {
                state[i] = id;
                var[id] = i;
                reg_cnt--;
                return ;
            }
        }
    }
}

void display_mem() {
    printf("\n");
    for(int j = 0; j < 4; j++){
        for(int i = 0; i < 16; i++) printf("%d\t", state[16 * j + i]);
        printf("\n");
    }
    printf("\n");
}

void check_remove(int id, int val) {
    if(id <= 0) return ;
    cnt[id] -= val;
    if(cnt[id] == 0) {
        if(var[id] < 16) reg_cnt++;
        state[var[id]] = 0;
        var[id] = -1;
    }
}

void work() {
    var[n + 1] = 5, state[5] = n + 1;
    var[n + 2] = 4, state[4] = n + 2;
    var[n + 3] = 2, state[2] = n + 3;
    reg_cnt -= 3;
    for(int i = 1; i <= m; i++) {
        resolve(X[i]);
        resolve(Y[i]);
        if(Z[i] > 0) resolve(Z[i]);
        /*
        if(i == 10) printf("#   %d = %d %s %d\n", X[i], Y[i], key_name[op[i]], Z[i]),
        printf("var_state(%d, %d, %d) = (%d, %d, %d)\n", X[i], Y[i], Z[i], var[X[i]], var[Y[i]], var[Z[i]]),
        display_mem();
        */
        if(op[i] == DIV) {
            if(Z[i] == IMM) {
                int m_id = new_mem();
                output(MOV, Y[i], 0, REG, 0);
                output(MOV, IMM, Z_val[i], MEM, m_id);
                state[m_id] = -1; //!!!
                //output(MOV, IMM, 0, REG, 3);
                strcat(ans, "cqto\n");
                output(DIV, MEM, m_id, 0, 0);
                output(MOV, REG, 0, X[i], 0);
            }
            else {
                output(MOV, Y[i], 0, REG, 0);
                //output(MOV, IMM, 0, REG, 3);
                strcat(ans, "cqto\n");
                output(DIV, Z[i], 0, 0, 0);
                output(MOV, REG, 0, X[i], 0);
            }
        }
        else {
            output(MOV, Y[i], 0, REG, 0);
            output(op[i], Z[i], Z_val[i], REG, 0);
            output(MOV, REG, 0, X[i], 0);
        }
        output(XOR, X[i], 0, REG, 1);

        check_remove(X[i], 2);
        check_remove(Y[i], 1);
        check_remove(Z[i], 1);
    }
    output(MOV, REG, 1, REG, 0);
    tot_mem -= 15;

    printf("pushq %s\n", reg_name[1]);
    printf("pushq %s\n", reg_name[6]);
    printf("pushq %s\n", reg_name[12]);
    printf("pushq %s\n", reg_name[13]);
    printf("pushq %s\n", reg_name[14]);
    printf("pushq %s\n", reg_name[15]);
    
    printf("movq %%rsp, %%rbp\n");
    printf("movq %%rdx, %%rcx\n");
    printf("movq $0, %%rbx\n");
    printf("movq $0, %%rdx\n");

    if(tot_mem != -1) printf("subq $%d, %%rsp\n", 8 * (tot_mem + 1));
    printf("%s", ans);
    if(tot_mem != -1) printf("addq $%d, %%rsp\n", 8 * (tot_mem + 1));

    printf("popq %s\n", reg_name[15]);
    printf("popq %s\n", reg_name[14]);
    printf("popq %s\n", reg_name[13]);
    printf("popq %s\n", reg_name[12]);
    printf("popq %s\n", reg_name[6]);
    printf("popq %s\n", reg_name[1]);
    printf("ret\n");
}

void init() {
    char str[8], ch;
    n = m = 60;
    for(int i = 1; i <= m; i++) {

        X[i] = read(&Z_val[i]);
        cnt[X[i]] += 2;

        scanf(" = "); 
        Y[i] = read(&Z_val[i]);
        cnt[Y[i]] += 1;

        scanf(" %c", &ch);
        Z[i] = read(&Z_val[i]);
        if(Z[i] > 0) cnt[Z[i]] += 1;

        switch(ch) {
            case '+': op[i] = ADD; break;
            case '-': op[i] = SUB; break;
            case '*': op[i] = MUL; break;
            case '/': op[i] = DIV; break;
        }
        //printf("%d = %d (%d) %d\n", X[i], Y[i], op[i], Z[i]);
    }
    for(int i = 1; i <= n + 3; i++) var[i] = -1;

    state[0] = state[1] = state[3] = state[6] = state[7] = -1;
    reg_cnt -= 5;
}

int main() {
    init();
    work();
    return 0;
}