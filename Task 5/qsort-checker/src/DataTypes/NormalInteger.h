#ifndef _MENCI_QSORT_CHECKER_DATA_NORMALINTEGER_H
#define _MENCI_QSORT_CHECKER_DATA_NORMALINTEGER_H

#include <random>
#include <limits>

struct DataNormalInteger {
    typedef unsigned long Type;

    inline static const char *NAME = "Normal 8 Bytes Integer";

    template <typename RNG>
    static Type random(RNG &rng) {
        return std::uniform_int_distribution<unsigned long>(0, std::numeric_limits<unsigned long>::max())(rng);
    }

    static int compare(const Type &a, const Type &b) {
        if (a < b) return -1;
        else if (a > b) return 1;
        else return 0;
    }
};

#endif // _MENCI_QSORT_CHECKER_DATA_NORMALINTEGER_H
