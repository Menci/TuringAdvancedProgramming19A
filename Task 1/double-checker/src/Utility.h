#ifndef _MENCI_DOUBLE_CHECKER_UTILITY_H
#define _MENCI_DOUBLE_CHECKER_UTILITY_H

#include <cstdint>

double i2f(uint64_t i) {
    union {
        uint64_t i;
        double f;
    } u;
    u.i = i;
    return u.f;
}

uint64_t f2i(double f) {
    union {
        uint64_t i;
        double f;
    } u;
    u.f = f;
    return u.i;
}

#endif // _MENCI_DOUBLE_CHECKER_UTILITY_H
