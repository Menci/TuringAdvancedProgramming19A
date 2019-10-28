#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define N 105

int n, m, C;
long long X[N], Y[N], Z[N], Z_type[N];
char op[N], tmp_str[N];

long long reg_state[16], reg_cnt; //reg_state: empty=0, else=varid
long long var_state[N]; //adress.
long long mem_state[1024]; //mem_state... same.

long long cnt[N];

enum _op {
    ADD = 0, SUB, MUL, DIV
}; 

enum _type {
    REG = 0, MEM, IMM
};

char var_name[2048][10];
char str[3][105];

//#define swap(x, y) x ^= y ^= x ^= y

void swap(long long *p, long long *q) {
    long long t;
    t = *p;
    *p = *q;
    *q = t;
}

int get_type(int id) {
    if(var_state[id] >= 16) return MEM;
    return REG;
}

void init() {
    n = 60, m = 60;
    for(int i = 1; i <= m; i++) {
        scanf("%s = %s %c %s;", str[0], str[1], &op[i], str[2]);
        if(str[0][0] == 'a') X[i] = n + atoi(str[0] + 1);
        else X[i] = atoi(str[0] + 1);
        
        if(str[1][0] == 'a') Y[i] = n + atoi(str[1] + 1);
        else Y[i] = atoi(str[1] + 1);
        
        if(str[2][0] == 'a') Z[i] = n + atoi(str[2] + 1);
        else if(('9' >= str[2][0] && str[2][0] >= '0') || str[2][0] == '-') Z[i] = atoi(str[2]), Z_type[i] = 1;
        else Z[i] = atoi(str[2] + 1);

        cnt[X[i]] += 2;
        cnt[Y[i]]++;
        if(Z_type[i] == 0) cnt[Z[i]]++;
    }
    for(int i = 1; i <= n + 3; i++) var_state[i] = -1; // -1: UNUSED
    C = 15; //?
    for(int i = 0; i <= C; i++) sprintf(var_name[i], "reg[%d]", i);
    for(int i = 0; i < 1024; i++) sprintf(var_name[i + 16], "mem[%d]", i);
}

int new_mem() {for(int i = 0; i < 1024; i++) if(mem_state[i] == 0) return i;}

void resolve(int id) {
    if(reg_cnt == C) {
        int minpos = 0, minval = 0x3f3f3f3f;
        for(int i = 1; i <= C; i++)
            if(cnt[reg_state[i]] < minval) 
                minval = cnt[reg_state[i]], minpos = i;

        if(cnt[id] > 10 + minval) {
            if(var_state[id] == -1) {
                //send reg[minpos] to mem
                int m_id = new_mem();
                printf("\tmem[%d] = reg[%d];\n", m_id, minpos);
                mem_state[m_id] = reg_state[minpos];
                var_state[mem_state[m_id]] = 16 + m_id;

                //send id to reg
                reg_state[minpos] = id;
                var_state[id] = minpos;
            }
            else if(var_state[id] >= 16) {
                //swap.
                int m_id = var_state[id] - 16;
                printf("\treg[0] = mem[%d];\n", m_id);
                printf("\tmem[%d] = reg[%d];\n", m_id, minpos);
                printf("\treg[%d] = reg[0];\n", minpos);
                swap(&var_state[id], &var_state[reg_state[minpos]]);
                swap(&mem_state[m_id], &reg_state[minpos]);
            }
        }
        else if(var_state[id] == -1) {
            //send it to mem
            int m_id = new_mem();
            mem_state[m_id] = id;
            var_state[id] = 16 + m_id;
        }
    }
    else {
        for(int i = 1; i <= C; i++) {
            if(reg_state[i] == 0) {
                if(var_state[id] == -1) {
                    reg_state[i] = id;
                    reg_cnt++;
                    var_state[id] = i;
                    //output...
                }
                else if(var_state[id] >= 16) {
                    int m_id = var_state[id] - 16;
                    mem_state[m_id] = -1;
                    printf("\treg[%d] = mem[%d];\n", i, m_id);
                    reg_state[i] = id;
                    reg_cnt++;
                    var_state[id] = i;
                }
                return ;
            }
        }
    }
}

char* my_itoa(long long x) {
    sprintf(tmp_str, "%lld", x);
    return tmp_str;
}

#define get_name(id, type) (type == 0 ? var_name[var_state[id]] : my_itoa(id))

void work() {
    resolve(n + 1);
    resolve(n + 2);
    resolve(n + 3);

    for(int i = 1; i <= m; i++) {
        // X = Y op Z 
        //想办法把 X, Y, Z 放进去。 如果空了直接加；如果满了看能不能替换一个出来。 >= (5 * 2)
        resolve(X[i]);
        resolve(Y[i]);
        if(Z_type[i] == 0) resolve(Z[i]);

        int x_type, y_type, z_type;
        x_type = get_type(X[i]);
        y_type = get_type(Y[i]);
        if(Z_type[i] == 0) z_type = get_type(Z[i]);
        else z_type = IMM;

        if(x_type == REG) {
            if(X[i] == Y[i]) {
                printf("\t%s %c= %s;\n", get_name(X[i], 0), op[i], get_name(Z[i], Z_type[i]));
            }
            else {
                printf("\t%s = %s;\n", get_name(X[i], 0), var_name[var_state[Y[i]]]);
                printf("\t%s %c= %s;\n", get_name(X[i], 0), op[i], get_name(Z[i], Z_type[i]));
            }
        }
        else if(y_type != MEM && z_type != MEM) {
            printf("\t%s = %s;\n", get_name(X[i], 0), var_name[var_state[Y[i]]]);
            printf("\t%s %c= %s;\n", get_name(X[i], 0), op[i], get_name(Z[i], Z_type[i]));
        }
        else {
            printf("\t%s = %s;\n", var_name[0], var_name[var_state[Y[i]]]);
            printf("\t%s %c= %s;\n", var_name[0], op[i], get_name(Z[i], Z_type[i]));
            printf("\t%s = %s;\n", get_name(X[i], 0), var_name[0]);
        }

        cnt[X[i]] -= 2;
        if(cnt[X[i]] == 0) if(var_state[X[i]] <= C) reg_state[var_state[X[i]]] = 0, reg_cnt--;

        cnt[Y[i]]--;
        if(cnt[Y[i]] == 0) if(var_state[Y[i]] <= C) reg_state[var_state[Y[i]]] = 0, reg_cnt--;
        
        if(Z_type[i] == 0) {
            cnt[Z[i]]--;
            if(cnt[Z[i]] == 0) if(var_state[Z[i]] <= C) reg_state[var_state[Z[i]]] = 0, reg_cnt--;
        }
    } 
}

int main() {   
    init();
    work();
    return 0;
}
