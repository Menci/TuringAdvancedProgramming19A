#include <stdbool.h>

#include "double.h"

static uint64_t getSign(const double_t* _val) {
    return *_val >> (EXP_SIZE + FRAC_SIZE);
}

static void setSign(double_t* _val, uint64_t num) {
    num &= 1ul;
    *_val = (*_val & NUM_NO_SIGN) | (num << (EXP_SIZE + FRAC_SIZE));
}

static uint64_t getExponent(const double_t* _val) {
    return (*_val & NUM_ONLY_EXP) >> FRAC_SIZE;
}

static void setExponent(double_t* _val, uint64_t num) {
    num &= (1ul << EXP_SIZE) - 1;
    *_val = (*_val & NUM_NO_EXP) | (num << FRAC_SIZE);
}

static uint64_t getFraction(const double_t* _val) {
    return *_val & NUM_ONLY_FRAC;
}

static void setFraction(double_t* _val, uint64_t num) {
    num &= (1ul << FRAC_SIZE) - 1;
    *_val = (*_val & NUM_NO_FRAC) | num;
}

int isZero(const double_t _val) {
    return getExponent(&_val) == 0 && getFraction(&_val) == 0;
}

static void setZero(double_t* _val) {
    setExponent(_val, 0);
    setFraction(_val, 0);
}

int isNan(const double_t _val) {
    return getExponent(&_val) == SPECIAL_EXP && getFraction(&_val) != 0;
}

static void setNan(double_t* _val) {
    setExponent(_val, SPECIAL_EXP);
    setFraction(_val, 1);
}

int isInf(const double_t _val) {
    return getExponent(&_val) == SPECIAL_EXP && getFraction(&_val) == 0;
}

static void setInf(double_t* _val) {
    setExponent(_val, SPECIAL_EXP);
    setFraction(_val, 0);
}

static void doubleInitialize(double_t* _val) {
    *_val = 0;
}

static int isDenormalized(const double_t _val) {
    return getExponent(&_val) == 0;
}

static void AdjustPrecision(int* exp_res, uint128_t* frac_res, int precision) {
    while((*frac_res >> precision) > 1) {
        *exp_res += 1;
        *frac_res >>= 1;
    }
    while((*frac_res >> precision) == 0) {
        *frac_res <<= 1;
        *exp_res -= 1;
    }     
}

static void Rounding(uint128_t* frac_res, int delta) {
    uint128_t lowBits = *frac_res & ((VL_1 << delta) - 1);
    if(lowBits > VL_1 << (delta - 1)) {
        *frac_res = (*frac_res >> delta) + 1;
    } else if(lowBits < VL_1 << (delta - 1)) {
        *frac_res >>= delta;
    } else {
        *frac_res >>= delta;
        *frac_res += (*frac_res & 1);
    }
}

static void ProcessAnswer(double_t* result, int exp_res, uint128_t frac_res, int precision) {
    if(exp_res < 0 - BIAS - FRAC_SIZE) {
        return setZero(result);
    }
    bool rounded = false;
    if(0 - BIAS - FRAC_SIZE <= exp_res && exp_res < 1 - BIAS) {
        Rounding(&frac_res, precision + 1 - BIAS - exp_res);
        rounded = true;
        exp_res = -BIAS;
        if((frac_res >> FRAC_SIZE) > 1) {
            frac_res >>= 1;
            exp_res += 1;
        }
    }
    if(1 - BIAS <= exp_res && exp_res <= DOUBLE_EXP_MAX) {
        if(!rounded) {
            Rounding(&frac_res, precision);
        }
        if((frac_res >> FRAC_SIZE) > 1) {
            frac_res >>= 1;
            exp_res += 1;
        }
    }
    if(exp_res > DOUBLE_EXP_MAX) {
        return setInf(result);
    }
    setExponent(result, (uint64_t)(exp_res + BIAS));
    setFraction(result, (uint64_t)frac_res);
}

int doubleCmp(double_t a, double_t b) {
    if(a == b) {
        return 0;
    }
    unsigned signA = getSign(&a), signB = getSign(&b);
    if(signA == signB) {
        return ((a > b) ? 1 : -1) * (signA ? -1 : 1);
    } else {
        return signA == 1 ? -1 : 1;
    }
}


#define swap(a, b)                          \
        {                                   \
            __typeof__(a) tmp = a;          \
            a = b;                          \
            b = tmp;                        \
        }


double_t doubleAdd(double_t a, double_t b) {
    double_t result;
    doubleInitialize(&result);

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
        } else if(frac_b == frac_a && getSign(&a) != getSign(&b)) {
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

    AdjustPrecision(&exp_res, &frac_res, FRAC_SIZE * 2);
    ProcessAnswer(&result, exp_res, frac_res, FRAC_SIZE);

    return result;
}

double_t doubleMin(double_t a, double_t b) {
    setSign(&b, getSign(&b) ^ 1);
    return doubleAdd(a, b);
}

double_t doubleMul(double_t a, double_t b) {
    double_t result;
    doubleInitialize(&result);
    setSign(&result, getSign(&a) ^ getSign(&b));

    if(isNan(a) || isNan(b) || (isInf(a) && isZero(b)) || (isInf(b) && isZero(a))) {
        setNan(&result);
        return result;
    }
    if(isInf(a) || isInf(b)) {
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
    
    AdjustPrecision(&exp_res, &frac_res, FRAC_SIZE * 2);
    ProcessAnswer(&result, exp_res, frac_res, FRAC_SIZE);

    return result;
}

double_t doubleDiv(double_t a, double_t b) {
    double_t result;
    doubleInitialize(&result);
    setSign(&result, getSign(&a) ^ getSign(&b));

    if(isNan(a) || isNan(b) || (isZero(a) && isZero(b)) || (isInf(a) && isInf(b))) {
        setNan(&result);
        return result;
    }
    if(isInf(a) || isZero(b)) {
        setInf(&result);
        return result;
    }
    if(isZero(a) || isInf(b)) {
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
    
    AdjustPrecision(&exp_res, &frac_res, FRAC_SIZE + DIV_EXTRA_SIZE);
    ProcessAnswer(&result, exp_res, frac_res, DIV_EXTRA_SIZE);

    return result;
}

#undef swap
