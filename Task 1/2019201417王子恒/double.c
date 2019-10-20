#include <stdint.h>  // uint64_t
#include <stdbool.h> // bool

typedef struct{
	uint8_t bit[8];
} DOUBLE;

const uint64_t BIAS = 1023;
const uint64_t MAX_BIT = 63;
const uint64_t MAX_ADD_BIT = 73; 
const uint64_t MAX_EXP = 11;
const uint64_t MAX_FRAC = 52;
const uint64_t MAX_BIT_NUM= 255;
const uint64_t MAX_EXP_NUM = 2047;
const uint64_t MAX_M_NUM = ((1LL << 53) - 1);

const DOUBLE ZERO = {{0, 0, 0, 0, 0, 0, 0, 0}};
const DOUBLE INF = {{0, 0, 0, 0, 0, 0, 240, 127}};
const DOUBLE S_INF = {{0, 0, 0, 0, 0, 0, 240, 255}};
const DOUBLE _NAN = {{255, 255, 255, 255, 255, 255, 255, 255}};

inline int min(int a, int b) {
	return a < b ? a : b;
}

inline void swap(DOUBLE *a, DOUBLE *b) {
	DOUBLE t = *a; *a = *b; *b = t;
	return;
}

inline DOUBLE convert_to_double_type(uint64_t a) {
	DOUBLE b;
	int i;
	for(i = 0; i < 8; ++i) {
		b.bit[i] = a & MAX_BIT_NUM;
		a >>= 8; 
	}
	return b;
}

inline uint64_t convert_from_double_type(DOUBLE a) {
	uint64_t b = 0;
	int i;
	for(i = 0; i < 8; ++i) {
		b += (uint64_t)a.bit[i] << (i << 3);
	}
	return b;
}

inline uint64_t get_exp(DOUBLE a) {
	return (((uint64_t)a.bit[7] & 127) << 4) | (((uint64_t)a.bit[6] & 240)>>4);
}

inline uint64_t get_frac(DOUBLE a) {
	int i; uint64_t b = a.bit[6] & 15;
	for(i = 5; i >= 0; --i) {
		b <<= 8;
		b |= a.bit[i];
	}
	return b;
}

inline int is_denormalized(DOUBLE a) {
	return get_exp(a) == 0;
}

inline int is_special_inf(DOUBLE a) {
	uint64_t exp = get_exp(a);
	if(exp == MAX_EXP_NUM && !get_frac(a)) return 1;
	return 0;
}

inline int is_special_nan(DOUBLE a) {
	uint64_t exp = get_exp(a);
	if(exp == MAX_EXP_NUM && get_frac(a))return 1;
	return 0;
}

inline int is_normalized(DOUBLE a) {
	if(is_denormalized(a) || is_special_inf(a) || is_special_nan(a))return 0;
	return 1;
}

inline int is_zero(DOUBLE a) {
	int i;
	for(i = 0; i < 6; ++i) {
		if(a.bit[i]) return 0;
	}
	if(a.bit[7] == 0 || a.bit[7] == 128) return 1;
	return 0;
}

inline int get_s(DOUBLE a) {
	return (a.bit[7] & 128) > 0;
}

inline uint64_t get_m(DOUBLE a) {
	return get_frac(a) + (is_normalized(a) ? (1LL << MAX_FRAC) : 0);
}

inline int get_e(DOUBLE a) {
	return is_denormalized(a) ? (1 - BIAS) : (get_exp(a) - BIAS);
}

inline DOUBLE change_sign(DOUBLE a) {
	if(get_s(a)) {
		a.bit[7] &= 127;
	} else {
		a.bit[7] |= 128;
	}
	return a;
}

DOUBLE double_round(uint64_t s, __int128 m, int e) {
	while(m <= MAX_M_NUM && (m<<1) <= MAX_M_NUM && e >= 1 - (int)BIAS) {
		m <<= 1; --e;
	}
	__int128 t = m;
	int always_zero = 1;
	while(m > MAX_M_NUM || e < 1 - (int)BIAS) {
		++e;
		if(m & 1) {
			if((always_zero && (m >> 1 & 1)) || !always_zero) {
				if((m >> 1) + 1 <= MAX_M_NUM && e >= 1 - (int)BIAS) {
					m >>= 1; ++m; break;
				}
			}
			always_zero = 0;
		}
		m >>= 1;
		if(e > (int)BIAS) return s ? S_INF : INF;
	}
	if(e > (int)BIAS) return s ? S_INF : INF;
	uint64_t ans = (s << MAX_BIT) + ((BIAS + e) << MAX_FRAC) + m;
	if(BIAS + e) {
		ans -= (1LL << MAX_FRAC);
	}
	return convert_to_double_type(ans);
}

int cmp(DOUBLE a, DOUBLE b) {
	return get_e(a) == get_e(b) ? get_m(a) > get_m(b) : get_e(a) > get_e(b);
}

DOUBLE add(DOUBLE a, DOUBLE b) {
	if(is_special_nan(a) || is_special_nan(b)) return _NAN;
	if(!cmp(a, b)) swap(&a, &b);
	uint64_t s_a = get_s(a), s_b = get_s(b);
	uint64_t s_c = s_a;
	if(is_special_inf(a) && is_special_inf(b)) {
		if(s_a == s_b) return a;
		else return _NAN;
	}
	if(is_special_inf(a)) return a;
	if(is_special_inf(b)) return b;
	int e_a = get_e(a), e_b = get_e(b), e_c, del_e = e_a - e_b;
	__int128 m_a = get_m(a), m_b = get_m(b), m_c;
	if(s_a == s_b) {
		m_a <<= MAX_ADD_BIT; m_b <<= MAX_ADD_BIT;
		if(del_e) {
			m_b >>= min(del_e, 120);
		}
		m_c = m_a + m_b;
		e_c = e_a - (int)MAX_ADD_BIT;
		return double_round(s_c, m_c, e_c);
	} else {
		m_a <<= MAX_ADD_BIT; m_b <<= MAX_ADD_BIT;
		if(del_e) {
			m_b >>= min(del_e, 120);
		}
		m_c = m_a - m_b;
		e_c = e_a - (int)MAX_ADD_BIT;
		return double_round(s_c, m_c, e_c);
	}
}

DOUBLE minus(DOUBLE a, DOUBLE b) {
	return add(a, change_sign(b));
}

DOUBLE multiply(DOUBLE a, DOUBLE b) {
	if(is_special_nan(a) || is_special_nan(b)) return _NAN;
	if(is_zero(b)) swap(&a, &b);
	if(is_zero(a) && is_special_inf(b)) return _NAN;
	uint64_t s_a = get_s(a), s_b = get_s(b);
	uint64_t s_c = s_a ^ s_b;
	if(is_special_inf(a) || is_special_inf(b)) return s_c ? S_INF : INF;
	int e_a = get_e(a), e_b = get_e(b);
	int e_c = e_a + e_b - MAX_FRAC;
	__int128 m_a = get_m(a), m_b = get_m(b);
	__int128 m_c = m_a * m_b;
	return double_round(s_c, m_c, e_c);
}

DOUBLE divide(DOUBLE a, DOUBLE b) {
	if(is_special_nan(a) || is_special_nan(b)) return _NAN;
	uint64_t s_a = get_s(a), s_b = get_s(b);
	uint64_t s_c = s_a ^ s_b;
	if(is_zero(a)) {
		if(is_zero(b)) return _NAN;
		return ZERO;
	}
	if(is_special_inf(a)) {
		if(is_special_inf(b)) return _NAN;
		return s_c ? S_INF : INF;
	}
	if(is_zero(b)) return s_c ? S_INF : INF;
	if(is_special_inf(b))return ZERO;
	int e_a = get_e(a), e_b = get_e(b);
	int e_c = e_a - e_b - MAX_ADD_BIT + MAX_FRAC;
	__int128 m_a = get_m(a), m_b = get_m(b);
	__int128 m_c = (m_a << MAX_ADD_BIT) / m_b;
	if(m_a % m_b) m_c |= 1;
	return double_round(s_c, m_c, e_c);
}

uint64_t calculate_function(uint64_t a, uint64_t b, char op) {
    DOUBLE x = convert_to_double_type(a),
           y = convert_to_double_type(b),
           result;
    if (op == '+') result = add(x, y);
    if (op == '-') result = minus(x, y);
    if (op == '*') result = multiply(x, y);
    if (op == '/') result = divide(x, y);
    return convert_from_double_type(result);
}

int main() {
	return 0;
}
