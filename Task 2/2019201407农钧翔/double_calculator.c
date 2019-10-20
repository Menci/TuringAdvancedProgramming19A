#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

int n;
unsigned int table[65536];

typedef long long LL;
typedef unsigned long long my_db;
typedef unsigned __int128 I128;

const I128 _1 = 1ull;
#define bias ((1 << 10) - 1)
#define e_overflow ((1 << 11) - 1 - bias) //e == e_overflow. (bias inclueded)

#define mod2_w(x, w) ((x) & ((1ull << (w)) - 1))
#define mod2_w_128(x, w) ((x) & ((_1 << (w)) - 1))

#define highbit(x) (((x) == 0) ? -1 : (63 - (int)__builtin_clzll(x))) //x == 0 -> return 0; ULL only.
#define highbit_128(x) (highbit((my_db)((x) >> 64)) == -1 ? highbit((my_db)(mod2_w_128((x), 64))) : highbit((my_db)((x) >> 64)) + 64)

#define opp(x) ((x) ^ (1ull << 63))
#define multi_s(x, _s) ((_s == 0) ? (x) : opp(x))
#define db_abs(x) ((get_s(x) == 0) ? (x) : opp(x))

#define get_s(x) ((x) >> 63)
#define get_e(x) (((x) << 1) >> 53)
#define get_m(x) mod2_w(x, 52)

#define set_s(x, val) x ^= (get_s(x) ^ (val)) << 63
#define set_e(x, val) x ^= (get_e(x) ^ (val)) << 52
#define set_m(x, val) x ^= (get_m(x) ^ (val))

#define get_real_e(x) ((get_e(x) == 0) ? (1 - bias) : (get_e(x) - bias)) 
#define get_real_m(x) ((get_e(x) == 0) ? (get_m(x)) : (get_m(x) ^ (1ull << 52)))

#define my_double(_s, _e, _m) ((((my_db)_s) << 63) ^ (((my_db)_e + bias) << 52) ^ (_m)) // e += bias here

#define swap(x, y) (x ^= y ^= x ^= y)

my_db my_db_trivial[8] = {
    0x0000000000000000ull, 0, 0x7FF0000000000000ull, 0x7FF8000000000000ull,
    0x8000000000000000ull, 0, 0xFFF0000000000000ull, 0xFFF8000000000000ull
};

#define my_db_NaN(x) my_db_trivial[3 + ((x) << 2)]
#define my_db_INF(x) my_db_trivial[2 + ((x) << 2)]
#define my_db_0(x) my_db_trivial[(x) << 2]

int add_trivial[8][8] = {
    {0, 1, 2, 3, 0, 5, 6, 7},
    {1, 1, 2, 3, 1, 5, 6, 7},
    {2, 2, 2, 3, 2, 2, 7, 7},
    {3, 3, 3, 3, 3, 3, 3, 7},
    {0, 1, 2, 3, 4, 5, 6, 7},
    {5, 5, 2, 3, 5, 5, 6, 7},
    {6, 6, 7, 3, 6, 6, 6, 7},
    {7, 7, 7, 3, 7, 7, 7, 7}
};

int mul_trivial[8][8] = {
    {0, 0, 7, 3, 4, 4, 7, 7},
    {0, 1, 2, 3, 4, 5, 6, 7},
    {7, 2, 2, 3, 7, 6, 6, 7},
    {3, 3, 3, 3, 3, 3, 3, 7},
    {4, 4, 7, 3, 0, 0, 7, 7},
    {4, 5, 6, 3, 0, 1, 2, 7},
    {7, 6, 6, 3, 7, 2, 2, 7},
    {7, 7, 7, 3, 7, 7, 7, 7}
};

int div_trivial[8][8] = {
    {7, 0, 0, 3, 7, 4, 4, 7},
    {2, 1, 0, 3, 6, 5, 4, 7},
    {2, 2, 7, 3, 6, 6, 7, 7},
    {3, 3, 3, 3, 3, 3, 3, 3},
    {7, 4, 4, 3, 7, 0, 0, 7},
    {6, 5, 4, 3, 2, 1, 0, 7},
    {6, 6, 7, 3, 2, 2, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7}
};

int shift(I128* px, int w) {
    if(w <= 0) return (w > -128) ? (*px <<= -w, 0) : (*px = 0);
    if(w > 128) return (*px == 0) ? (*px = 0) : (*px = 0, 1);
    I128 x;
    if(w == 128) x = *px, *px = 0;
    else x = mod2_w_128(*px, w), *px >>= w;
    if(x == 0) return 0;
    if(x < (_1 << (w - 1))) return 1;
    else if(x == (_1 << (w - 1))) return 2;
    return 3;
}

#define shift_1(x) (((x) & 1) ? (x >>= 1, 2) : (x >>= 1, 0))

#define merge_eps(x, y) ((x != 0) ? ((y) | 1) : (y)) 
#define carry_eps(x, y) (((y | (x & 1)) == 3) ? (x+1) : (x)) 

int get_type(my_db x) {
    x = db_abs(x);
    if(x == 0) return 0;
    if(get_real_e(x) != e_overflow) return 1;
    if(get_m(x) == 0) return 2; //INF
    return 3; //NaN
}

my_db carry_m(my_db m_0, int eps, int e_1, int s_1) { //函数可支持：判断overflow，进至多1位: 要求round(m + eps) < (1 << 54)
    my_db m_0_backup = m_0;
    if(m_0 < (1ull << 53)) m_0 = carry_eps(m_0, eps);
    if(m_0 >= (1ull << 53)) {
        e_1++;
        m_0 = m_0_backup;
        eps = merge_eps(eps, (m_0 & 1) << 1);
        m_0 >>= 1; //shift_1 here.
        m_0 = carry_eps(m_0, eps);
    }
    if(e_1 >= e_overflow) return my_db_INF(s_1);
    if(m_0 < (1ull << 52)) return my_double(s_1, 0 - bias, m_0);
    return my_double(s_1, e_1, mod2_w(m_0, 52));
}

my_db addition(my_db x_1, my_db x_2) {
    int t_1 = get_type(x_1), t_2 = get_type(x_2); //t: type
    int s_1 = get_s(x_1), s_2 = get_s(x_2);

    t_1 += s_1 << 2, t_2 += s_2 << 2;
    if((add_trivial[t_1][t_2] & 3) != 1) return my_db_trivial[add_trivial[t_1][t_2]];

    int e_1 = get_real_e(x_1), e_2 = get_real_e(x_2);
    my_db m_1 = get_real_m(x_1), m_2 = get_real_m(x_2);
    if((e_1 == e_2 && m_1 < m_2) || (e_1 < e_2)) swap(e_1, e_2), swap(m_1, m_2), swap(s_1, s_2);
    s_2 ^= s_1; //while calculating, we assume that s_1 > 0. calc it when everything done.

    I128 m_0 = m_2;
    int eps = shift(&m_0, e_1 - e_2 - 64);
    if(s_2 == 1) {
        if(eps == 0) m_0 = ((I128)m_1 << 64) - m_0;
        else m_0 = ((I128)m_1 << 64) - m_0 - 1, eps = 4 - eps; // -eps -> +eps
    }
    else m_0 += (I128)m_1 << 64;
    
    int Hbit = highbit_128(m_0);
    if(Hbit == -1) return my_db_0(s_1); //SPJ: 0. eps = 3? impossible. (so what does eps do anyway)...  
    if(e_1 - 64 - 52 + Hbit >= 1 - bias) eps = merge_eps(eps, shift(&m_0, Hbit - 52)), e_1 = e_1 - 64 - 52 + Hbit; //check if e==0
    else eps = merge_eps(eps, shift(&m_0, 65 - e_1 - bias)), e_1 = 1 - bias; 

    return carry_m(m_0, eps, e_1, s_1);
}

my_db multiplication(my_db x_1, my_db x_2) {    
    int t_1 = get_type(x_1), t_2 = get_type(x_2); //t: type
    int s_1 = get_s(x_1), s_2 = get_s(x_2);

    t_1 += s_1 << 2, t_2 += s_2 << 2;
    if((mul_trivial[t_1][t_2] & 3) != 1) return my_db_trivial[mul_trivial[t_1][t_2]];

    int e_1 = get_real_e(x_1), e_2 = get_real_e(x_2);
    my_db m_1 = get_real_m(x_1), m_2 = get_real_m(x_2);    
    e_1 += e_2, s_1 ^= s_2;

    I128 m_0 = (I128)m_1 * m_2; //Hbit <= 105
    int Hbit = highbit_128(m_0);
    if(Hbit < 104) shift(&m_0, Hbit - 104), e_1 += Hbit - 104;

    int eps = shift(&m_0, 52);
    if(e_1 < 1 - bias) eps = merge_eps(eps, shift(&m_0, 1 - bias - e_1)), e_1 = 1 - bias;//, printf("underflow.\n");

    return carry_m(m_0, eps, e_1, s_1); 
}

my_db division(my_db x_1, my_db x_2) {
    int t_1 = get_type(x_1), t_2 = get_type(x_2); //t: type
    int s_1 = get_s(x_1), s_2 = get_s(x_2);

    t_1 += s_1 << 2, t_2 += s_2 << 2;
    if((div_trivial[t_1][t_2] & 3) != 1) return my_db_trivial[div_trivial[t_1][t_2]];

    int e_1 = get_real_e(x_1), e_2 = get_real_e(x_2);
    my_db m_1 = get_real_m(x_1), m_2 = get_real_m(x_2);
    e_1 -= e_2, s_1 ^= s_2;

    I128 m_0 = m_1, m_r;
    int Hbit = highbit_128(m_0), eps;
    if(Hbit < 52) m_0 <<= 52 - Hbit, e_1 += Hbit - 52;

    m_0 <<= 53;
    m_r = m_0 % m_2, m_0 = m_0 / m_2;
    e_1 --; //for "<<53" <==> "m*=2"

    if(m_r == 0) eps = 0;
    else if((m_r << 1) < m_2) eps = 1;
    else if((m_r << 1) == m_2) eps = 2;
    else eps = 3;

    Hbit = highbit_128(m_0);
    if(Hbit > 52) eps = merge_eps(eps, shift(&m_0, Hbit - 52)), e_1 += Hbit - 52;
    if(e_1 < 1 - bias) eps = merge_eps(eps, shift(&m_0, 1 - bias - e_1)), e_1 = 1 - bias;

    return carry_m(m_0, eps, e_1, s_1);
}

my_db simple_calc(my_db x, my_db y, char w) {
    switch(w) {
        case '+': return addition(x, y);
        case '-': return addition(x, opp(y));
        case '*': return multiplication(x, y);
        case '/': return division(x, y);
    }
}

// new code goes below here : 

enum char_type {
    ERR = 0, SPACE, NUM, OP1, OP2, ENDL, DOTS, LBRA, RBRA //OP1: +- OP2: */
};

#define C_10T2 308
#define C_2T10 1000000000000000000ull //1e18

int C_type[256];
my_db exp_10T2[2 * C_10T2 + 2];
my_db my_db_10 = my_double(0, 3, 1ll << 50);

I128 exp_2T10[2048];

#define _2T10(x) exp_2T10[(x) + bias]
#define _10T2(x) exp_10T2[(x) + C_10T2]

#define my_science(x, y) (((I128)(x) << 64) + (y))
#define get_e10(x) ((x) >> 64)
#define get_m10(x) mod2_w_128(x, 64) 

my_db read_db() {
    int s = 0, dot_count = 0, e = -1; // dot_count helps to check whether e gets fixed.
    char cur;
    I128 res = 0;
    while(C_type[cur = getchar()] == SPACE);
    if(C_type[cur] == OP1) {
        if(cur == '+') s = 0;
        else s = 1;
    }
    else ungetc(cur, stdin);
    if(C_type[cur = getchar()] != NUM) return my_db_NaN(0); // NaN. If get any NaN while cacluating, return NaN instantly. If there's nothing, still return NaN.
    if(cur == '0'){
        if(C_type[cur = getchar()] != DOTS) {
            ungetc(cur, stdin);
            return my_db_trivial[0];
        }
        else {
            while((cur = getchar()) == '0') e--;
            ungetc(cur, stdin);
            while(C_type[cur = getchar()] == NUM) if(res < C_2T10) res *= 10, res += cur - '0';
        }
    }
    else {
        ungetc(cur, stdin);
        while((C_type[cur = getchar()] & 3) == 2) { // NUM or DOTS
            if(C_type[cur] == DOTS) {
                dot_count++;
                if(dot_count == 2) return my_db_NaN(0); // ret NaN here.
            }
            else {
                if(res < C_2T10) res *= 10, res += cur - '0';
                if(dot_count == 0) e++;
            }
        }
    }
    if(C_type[cur] == ERR) return my_db_NaN(0);
    ungetc(cur, stdin);=
    while(res < C_2T10) res *= 10;
    int Hbit = highbit_128(res);
    shift(&res, Hbit - 52);
    my_db ans = my_double(s, Hbit, mod2_w_128(res, 52)); 
    return multiplication(_10T2(e - 18), ans); // possible array overflow here
}

struct my_pair {
    my_db num;
    int type;
}_S[65536], _S2[65536];

int _top, _top2;

void display_stack() {
    printf("Current Stack: len = %d\n", _top);
    for(int i = 0 ; i <= _top; i++) {
        if(_S[i].type == NUM) {
            my_db num = _S[i].num;
            printf("{ (-1)^(%lld) * (2)^(%lld) * (%llx)_2 }\n", get_s(num), get_real_e(num), get_real_m(num));
        }
        else {
            char op = 'E';
            switch(_S[i].type) {
                case 0 + OP1: op = '+'; break;
                case 0 - OP1: op = '-'; break;
                case 0 + OP2: op = '*'; break;
                case 0 - OP2: op = '/'; break;
                case LBRA: op = '('; break;
            }
            if(op != 'E') printf("{ %c }\n", op);
            else printf("{ERROR: %c}\n", _S[i].type);
        }
    }
    printf("\n\n");
}

struct my_pair mk_pair(my_db _num, int _type) {
    struct my_pair res;
    res.num = _num;
    res.type = _type;
    return res;
}

int check_NaN(my_db num) {
    char cur;
    if(num == my_db_NaN(0) || num == my_db_NaN(1)) {
        do {
            cur = getchar();
        }while(cur != EOF && cur != '\n');
        ungetc(cur, stdin);
        return 1;
    }
    return 0;
}

my_db solve_expression() {
    my_db ans = my_db_0(0);
    my_db res = _S2[0].num;
    for(int i = 2; i <= _top2; i += 2) {
        switch(_S2[i - 1].type) {
            case 0 + OP1: ans = addition(ans, res), res = _S2[i].num; break;
            case 0 - OP1: ans = addition(ans, opp(res)), res = _S2[i].num; break;
            case 0 + OP2: res = multiplication(_S2[i].num, res); break; 
            case 0 - OP2: res = division(_S2[i].num, res); break;
        }
    }
    _top2 = -1;
    return addition(ans, res);
}

my_db add_RBRA() {
    while(_S[_top].type != LBRA) _S2[++_top2] = _S[_top--];
    _top--;
    my_db num = solve_expression();
    if(check_NaN(num) == 1) return my_db_NaN(0);
    _S[++_top] = mk_pair(num, NUM);
    char cur;
    while(C_type[cur = getchar()] == SPACE);
    ungetc(cur, stdin);
    return num;
}

my_db calc() {
    char cur;
    my_db num = my_db_NaN(0);
    _top = _top2 = -1;
    _S[++_top] = mk_pair(my_db_NaN(0), LBRA);
    while(1) {
        while(C_type[cur = getchar()] == SPACE);
        ungetc(cur, stdin);
        while((cur = getchar()) == '(') _S[++_top] = mk_pair(my_db_NaN(0), LBRA);
        ungetc(cur, stdin);

        num = read_db();
        if(check_NaN(num) == 1) return my_db_NaN(0);
        else _S[++_top] = mk_pair(num, NUM);
        while((cur = getchar()) == ')') if(add_RBRA() == my_db_NaN(0)) return my_db_NaN(0);
        ungetc(cur, stdin);
        while(C_type[cur = getchar()] == SPACE);

        if(cur == '\n' || cur == EOF) {
            ungetc(cur, stdin);
            add_RBRA();
            
            if(_top == 0) return _S[0].num;
            else return my_db_NaN(0);
        }
        if(C_type[cur] != OP1 && C_type[cur] != OP2) return check_NaN(my_db_NaN(0)), my_db_NaN(0);
        else {
            switch(cur) {
                case '+': _S[++_top] = mk_pair(my_db_NaN(0), 0 + OP1); break;
                case '-': _S[++_top] = mk_pair(my_db_NaN(0), 0 - OP1); break;
                case '*': _S[++_top] = mk_pair(my_db_NaN(0), 0 + OP2); break;
                case '/': _S[++_top] = mk_pair(my_db_NaN(0), 0 - OP2); break;
            }
        }
    }
}

int highbit10(I128 x) {
    int res = -1;
    if(x == 0) return 0;
    while(x != 0) {
        x /= 10;
        res++;
    }
    return res;
}

void work() {
    char cur;
    do {
        my_db res = calc();
        if(res == my_db_NaN(0)) printf("NaN\n");
        else if(res == my_db_INF(0)) printf("inf\n");
        else if(res == my_db_INF(1)) printf("-inf\n");
        else {
            //2 ^(e - 52) * (2^52 * m) *(-1)^s  
            int e10 = get_e10(_2T10(get_real_e(res) - 52)); 
            I128 m10 = get_m10(_2T10(get_real_e(res) - 52)); // cuz -52, e represent the lowest bit here

            m10 *= get_real_m(res); 
            e10 += highbit10(m10); // now e represent the highest bit(demical)
            //(-1) ^ s * 10 ^ e10 * m10

            char str[C_10T2 + 1];
            int len = -1;
            while(m10) {
                str[++len] = m10 % 10 + '0';
                m10 /= 10;
            }
            if(len == -1) printf("0\n");
            else {
                if(get_s(res) == 1) printf("-");
                if(e10 < 0) {
                    printf("0.");
                    for(int i = -1; i > e10; i--) printf("0");
                    for(int i = len; i >= 0; i--) printf("%c", str[i]);
                }
                else {
                    for(int i = e10; i >= e10 - len; i--) {
                        printf("%c", str[i + len - e10]);
                        if(i == 0 && e10 - len < 0) printf(".");
                    }
                    for(int i = e10 - len - 1; i >= 0; i--) printf("0");
                }
                printf("\n");
            }
        }
        fflush(stdout);
        cur = getchar();
    }while(cur != EOF); // easier output version.
}

void init() { //get_table here.

    for(int i = 0; i < 256; i++) C_type[i] = ERR;
    C_type[' '] = SPACE;
    for(int i = 0; i < 10; i++) C_type['0' + i] = NUM;
    C_type['+'] = C_type['-'] = OP1;
    C_type['*'] = C_type['/'] = OP2;
    C_type['\n'] = ENDL; //ENDL -> the last ')'
    C_type['.'] = DOTS;
    C_type['('] = LBRA;
    C_type[')'] = RBRA;

    _10T2(0) = my_double(0, 0, 0);
    for(int i = 1; i <= C_10T2; i++) _10T2(i) = multiplication(_10T2(i - 1), my_db_10);
    for(int i = -1; i >= -C_10T2; i--) _10T2(i) = division(my_double(0, 0, 0), _10T2(-i));

    _2T10(0) = my_science(0, 1);
    for(int i = 1; i <= bias; i++) {
        if(2 * get_m10(_2T10(i - 1)) < C_2T10) _2T10(i) = my_science(get_e10(_2T10(i - 1)), get_m10(_2T10(i - 1)) * 2);
        else _2T10(i) = my_science(get_e10(_2T10(i - 1)) + 1, (get_m10(_2T10(i - 1)) * 2 + 5)/ 10); //rounding here
    }

    for(int i = -1; i >= 1 - bias; i--) {
        if(5 * get_m10(_2T10(i + 1)) < C_2T10) _2T10(i) = my_science(get_e10(_2T10(i + 1)) - 1, get_m10(_2T10(i + 1)) * 5);
        else _2T10(i) = my_science(get_e10(_2T10(i + 1)), (get_m10(_2T10(i + 1)) + 1) / 2); //and here
    }
}

int main() {
    init();
    work();
    return 0;
}