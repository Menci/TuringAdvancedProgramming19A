#ifndef _MENCI_QSORT_CHECKER_DATA_ONEBITINTEGER_H
#define _MENCI_QSORT_CHECKER_DATA_ONEBITINTEGER_H

#include <random>

struct DataOneByteInteger {
    typedef unsigned char Type;

    inline static const char *NAME = "One Byte Integer";

    template <typename RNG>
    static Type random(RNG &rng) {
        return std::uniform_int_distribution<int>(0, 255)(rng);
    }

    static int compare(const Type &a, const Type &b) {
        if (a < b) return -1;
        else if (a > b) return 1;
        else return 0;
    }
};

#endif // _MENCI_QSORT_CHECKER_DATA_ONEBITINTEGER_H
