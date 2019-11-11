#ifndef _MENCI_QSORT_CHECKER_DATA_UNALIGNEDSTRUCT_H
#define _MENCI_QSORT_CHECKER_DATA_UNALIGNEDSTRUCT_H

#include <random>
#include <limits>
#include <cassert>

struct DataUnalignedStruct {
    typedef struct {
        char unused[63 - 8];
        unsigned long key;
    } __attribute__ ((packed)) Type;

    inline static const char *NAME = "63 Bytes Unaligned Struct";

    template <typename RNG>
    static Type random(RNG &rng) {
        assert(sizeof(Type) == 63);
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

#endif // _MENCI_QSORT_CHECKER_DATA_UNALIGNEDSTRUCT_H
