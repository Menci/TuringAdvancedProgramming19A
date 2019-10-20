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

#define highbit(x) (((x) == 0) ? -1 : (63 - (int)__builtin_clzll(x))) //x == 0 -> return -1; ULL only.
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

my_db calc(my_db x, my_db y, char w) {
    switch(w) {
        case '+': return addition(x, y);
        case '-': return addition(x, opp(y));
        case '*': return multiplication(x, y);
        case '/': return division(x, y);
    }
}

int main() {
    return 0;
}