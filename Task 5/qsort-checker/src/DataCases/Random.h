#ifndef _MENCI_QSORT_CHECKER_SORTTYPE_RANDOM_H
#define _MENCI_QSORT_CHECKER_SORTTYPE_RANDOM_H

#include <algorithm>

struct DataCaseRandom {
    inline static const char *NAME = "Random";

    template <typename DataType>
    static void processData(std::vector<typename DataType::Type> &array) {
    }
};

#endif // _MENCI_QSORT_CHECKER_SORTTYPE_RANDOM_H
