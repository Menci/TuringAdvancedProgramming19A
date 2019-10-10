#include <string.h>
#include <stdint.h>

#ifndef __MY_DOUBLE_H__

#define __MY_DOUBLE_H__

typedef unsigned char       data_t;
typedef unsigned char*      pointer_t;
typedef __uint128_t         uint128_t;

#define DOUBLE_SIZE         __SIZEOF_DOUBLE__
#define EXP_SIZE            (11)
#define FRAC_SIZE           (52)
#define DIV_EXTRA_SIZE      (20)

#define BIAS                ((1 << (EXP_SIZE - 1)) - 1)
#define DOUBLE_EXP_MAX      ((1 << EXP_SIZE) - 2 - BIAS)
#define DOUBLE_EXP_MIN      (-BIAS)

#define SPECIAL_EXP         (0x7FFu)
#define DENORMALIZED_EXP    (0u)

#define VL_1                ((uint128_t)1)

#define swap(a, b)                                                      \
        {                                                               \
            __typeof__(a) tmp = a;                                      \
            a = b;                                                      \
            b = tmp;                                                    \
        }


typedef struct myDouble {
    data_t data[DOUBLE_SIZE];
} double_t;

unsigned getSign(const double_t* _val) {
    return _val -> data[DOUBLE_SIZE - 1] >> 7;
}
void setSign(double_t* _val, unsigned num) {
    num &= 1u;
    _val -> data[DOUBLE_SIZE - 1] = (data_t)((num << 7) | (_val -> data[DOUBLE_SIZE - 1] & 0x7Fu));
}

unsigned getExponent(const double_t* _val) {
    return ((_val -> data[DOUBLE_SIZE - 1] & 0x7Fu) << 4) | (_val -> data[DOUBLE_SIZE - 2] >> 4);
}
void setExponent(double_t* _val, unsigned num) {
    num &= (1u << EXP_SIZE) - 1;
    _val -> data[DOUBLE_SIZE - 1] = (data_t)((getSign(_val) << 7) | (num >> 4));
    _val -> data[DOUBLE_SIZE - 2] = (data_t)((num << 4) | (_val -> data[DOUBLE_SIZE - 2] & 0xFu));
}

uint64_t getFraction(const double_t* _val) {
    uint64_t res = _val -> data[DOUBLE_SIZE - 2] & 0xFu;
    for(int i = DOUBLE_SIZE - 3; i >= 0; i--) {
        res = (res << 8) | _val -> data[i];
    }
    return res;
}
void setFraction(double_t* _val, uint64_t num) {
    num &= (1ul << FRAC_SIZE) - 1;
    for(int i = 0; i < DOUBLE_SIZE - 2; i++) {
        _val -> data[i] = (data_t)(num & 0xFFu);
        num >>= 8;
    }
    _val -> data[DOUBLE_SIZE - 2] = (data_t)((_val -> data[DOUBLE_SIZE - 2] & 0xF0u) | num);
}

int isZero(const double_t _val) {
    if(getExponent(&_val) == 0 && getFraction(&_val) == 0) {
        return getSign(&_val) ? 1 : -1;
    } else {
        return 0;
    }
}
void setZero(double_t* _val) {
    setExponent(_val, 0);
    setFraction(_val, 0);
}

int isNan(const double_t _val) {
    return getExponent(&_val) == SPECIAL_EXP && getFraction(&_val) != 0;
}
void setNan(double_t* _val) {
    setExponent(_val, SPECIAL_EXP);
    setFraction(_val, 1);
}

int isInf(const double_t _val) {
    if(getExponent(&_val) == SPECIAL_EXP && getFraction(&_val) == 0) {
        return getSign(&_val) ? 1 : -1;
    } else {
        return 0;
    }
}
void setInf(double_t* _val) {
    setExponent(_val, SPECIAL_EXP);
    setFraction(_val, 0);
}

void __initialize__(double_t* _val) {
    memset(_val, 0, sizeof(double_t));
}

int isDenormalized(const double_t _val) {
    return getExponent(&_val) == 0;
}

#define Rounding(PRECISION)                                             \
        {                                                               \
            int delta = (PRECISION);                                    \
            uint128_t lowBits = frac_res & ((VL_1 << delta) - 1);       \
            if(lowBits > VL_1 << (delta - 1)) {                         \
                frac_res = (frac_res >> delta) + 1;                     \
            } else if(lowBits < VL_1 << (delta - 1)) {                  \
                frac_res >>= delta;                                     \
            } else {                                                    \
                frac_res >>= delta;                                     \
                frac_res += (frac_res & 1);                             \
            }                                                           \
        }


#define AdjustPrecision(PRECISION)                                      \
        {                                                               \
            while((frac_res >> (PRECISION)) >= 2) {                     \
                exp_res += 1;                                           \
                frac_res >>= 1;                                         \
            }                                                           \
            while((frac_res >> (PRECISION)) == 0) {                     \
                frac_res <<= 1;                                         \
                exp_res -= 1;                                           \
            }                                                           \
        }


#define ProcessAnswer(PRECISION)                                        \
        {                                                               \
            if(exp_res > DOUBLE_EXP_MAX) {                              \
                setInf(&result);                                        \
            } else if(exp_res >= 1 - BIAS) {                            \
                Rounding(PRECISION);                                    \
                setExponent(&result, (unsigned)(exp_res + BIAS));       \
                setFraction(&result, (uint64_t)frac_res);               \
            } else if(exp_res >= 0 - BIAS - FRAC_SIZE) {                \
                Rounding(PRECISION + 1 - BIAS - exp_res);               \
                exp_res = -BIAS;                                        \
                setExponent(&result, 0u);                               \
                setFraction(&result, (uint64_t)frac_res);               \
            } else {                                                    \
                setZero(&result);                                       \
            }                                                           \
        }


double_t __add__(double_t a, double_t b) {
    double_t result;
    __initialize__(&result);

    if(isNan(a) || isNan(b)) {
        setNan(&result);
        return result;
    }
    if(isInf(a) && isInf(b)) {
        if(getSign(&a) != getSign(&b)) {
            setNan(&result);
        } else {
            setSign(&result, getSign(&a));
            setInf(&result);
        }
        return result;
    } else if(isInf(a) || isInf(b)) {
        return isInf(a) ? a : b;
    }

    int exp_a = (isDenormalized(a) ? 1 : (int)getExponent(&a)) - BIAS, 
        exp_b = (isDenormalized(b) ? 1 : (int)getExponent(&b)) - BIAS;
    uint128_t frac_a = (uint128_t)(getFraction(&a) | ((uint64_t)(1 - isDenormalized(a)) << FRAC_SIZE)) << FRAC_SIZE, 
              frac_b = (uint128_t)(getFraction(&b) | ((uint64_t)(1 - isDenormalized(b)) << FRAC_SIZE)) << FRAC_SIZE;

    if(exp_a == exp_b) {
        if(frac_b > frac_a) {
            swap(exp_a, exp_b);
            swap(frac_a, frac_b);
            swap(a, b);
        } else if(frac_b == frac_a) {
            setZero(&result);
            return result;
        }
    }
    if(exp_b > exp_a) {
        swap(exp_a, exp_b);
        swap(frac_a, frac_b);
        swap(a, b);
    }
    setSign(&result, getSign(&a));

    if(exp_a - exp_b >= 128) {
        frac_b = 1;
    } else {
        frac_b = (frac_b >> (exp_a - exp_b)) | (0 != (frac_b & ((VL_1 << (exp_a - exp_b)) - 1)));
    }

    int exp_res = exp_a;
    uint128_t frac_res = (getSign(&a) == getSign(&b)) ? frac_a + frac_b : frac_a - frac_b;

    if(frac_res == 0) {
        setZero(&result);
        return result;
    }

    AdjustPrecision(FRAC_SIZE * 2);
    ProcessAnswer(FRAC_SIZE);

    return result;
}

double_t __min__(double_t a, double_t b) {
    setSign(&b, getSign(&b) ^ 1);
    return __add__(a, b);
}

double_t __mul__(double_t a, double_t b) {
    double_t result;
    __initialize__(&result);
    setSign(&result, getSign(&a) ^ getSign(&b));

    if(isNan(a) || isNan(b) || (isInf(a) && isZero(b)) || (isInf(b) && isZero(a))) {
        setNan(&result);
        return result;
    }
    if(isInf(a) && isInf(b)) {
        setInf(&result);
        return result;
    }
    if(isZero(a) || isZero(b)) {
        setZero(&result);
        return result;
    }

    int exp_a = (isDenormalized(a) ? 1 : (int)getExponent(&a)) - BIAS, 
        exp_b = (isDenormalized(b) ? 1 : (int)getExponent(&b)) - BIAS;
    uint64_t frac_a = getFraction(&a) | ((uint64_t)(1 - isDenormalized(a)) << FRAC_SIZE), 
             frac_b = getFraction(&b) | ((uint64_t)(1 - isDenormalized(b)) << FRAC_SIZE);

    int exp_res = exp_a + exp_b;
    uint128_t frac_res = (uint128_t)frac_a * frac_b;
    
    AdjustPrecision(FRAC_SIZE * 2);
    ProcessAnswer(FRAC_SIZE);

    return result;
}

double_t __div__(double_t a, double_t b) {
    double_t result;
    __initialize__(&result);
    setSign(&result, getSign(&a) ^ getSign(&b));

    if(isNan(a) || isNan(b) || (isZero(a) && isZero(b)) || (isInf(a) && isInf(b))) {
        setNan(&result);
        return result;
    }
    if(isInf(a)) {
        setInf(&result);
        return result;
    }
    if(isZero(a)) {
        setZero(&result);
        return result;
    }

    int exp_a = (isDenormalized(a) ? 1 : (int)getExponent(&a)) - BIAS, 
        exp_b = (isDenormalized(b) ? 1 : (int)getExponent(&b)) - BIAS;
    uint128_t frac_a = getFraction(&a) | ((uint64_t)(1 - isDenormalized(a)) << FRAC_SIZE), 
              frac_b = getFraction(&b) | ((uint64_t)(1 - isDenormalized(b)) << FRAC_SIZE);

    frac_a <<= FRAC_SIZE + DIV_EXTRA_SIZE - 1;

    int exp_res = exp_a - exp_b;
    uint128_t frac_res = ((frac_a / frac_b) << 1) | (frac_a % frac_b != 0);
    
    AdjustPrecision(FRAC_SIZE + DIV_EXTRA_SIZE);
    ProcessAnswer(DIV_EXTRA_SIZE);

    return result;
}

#endif
