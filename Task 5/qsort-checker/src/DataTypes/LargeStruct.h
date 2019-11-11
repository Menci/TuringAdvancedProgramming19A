#ifndef _MENCI_QSORT_CHECKER_DATA_LARGESTRUCT_H
#define _MENCI_QSORT_CHECKER_DATA_LARGESTRUCT_H

#include <random>
#include <limits>

struct DataLargeStruct {
    typedef struct {
        char unused1[256 - 8];
        unsigned long key;
        char unused2[128];
    } Type;

    inline static const char *NAME = "384 Bytes Large Struct";

    template <typename RNG>
    static Type random(RNG &rng) {
        Type data;
        data.key = std::uniform_int_distribution<unsigned long>(0, std::numeric_limits<unsigned long>::max())(rng);
        return data;
    }

    static int compare(const Type &a, const Type &b) {
        if (a.key < b.key) return -1;
        else if (a.key > b.key) return 1;
        else return 0;
    }
};

#endif // _MENCI_QSORT_CHECKER_DATA_LARGESTRUCT_H
