#ifndef __MY_DOUBLE_H__
#define __MY_DOUBLE_H__

#include <stdint.h>

typedef __uint64_t          double_t;
typedef __uint128_t         uint128_t;

#define DOUBLE_SIZE         __SIZEOF_DOUBLE__
#define EXP_SIZE            (11)
#define FRAC_SIZE           (52)
#define DIV_EXTRA_SIZE      (22)

#define VL_1                ((uint128_t)1)

#define NUM_ONLY_SIGN       (1ul << (EXP_SIZE + FRAC_SIZE))
#define NUM_NO_SIGN         (~(NUM_ONLY_SIGN))
#define NUM_ONLY_EXP        (((1ul << EXP_SIZE) - 1) << FRAC_SIZE)
#define NUM_NO_EXP          (~(NUM_ONLY_EXP))
#define NUM_ONLY_FRAC       ((1ul << FRAC_SIZE) - 1)
#define NUM_NO_FRAC         (~(NUM_ONLY_FRAC))

#define BIAS                ((1 << (EXP_SIZE - 1)) - 1)
#define DOUBLE_EXP_MAX      ((1 << EXP_SIZE) - 2 - BIAS)
#define DOUBLE_EXP_MIN      (-BIAS)

#define SPECIAL_EXP         (0x7FFu)
#define DENORMALIZED_EXP    (0u)


int isZero(const double_t);
int isNan(const double_t);
int isInf(const double_t);

int doubleCmp(double_t, double_t);

double_t doubleAdd(double_t, double_t);
double_t doubleMin(double_t, double_t);
double_t doubleMul(double_t, double_t);
double_t doubleDiv(double_t, double_t);

#endif
