#ifndef _MENCI_QSORT_CHECKER_SORTTYPE_SORTED_H
#define _MENCI_QSORT_CHECKER_SORTTYPE_SORTED_H

#include <algorithm>

struct DataCaseSorted {
    inline static const char *NAME = "Sorted";

    template <typename DataType>
    static void processData(std::vector<typename DataType::Type> &array) {
        std::sort(array.begin(), array.end(), [] (const typename DataType::Type &a, const typename DataType::Type &b) {
            return DataType::compare(a, b) == -1;
        });
    }
};

#endif // _MENCI_QSORT_CHECKER_SORTTYPE_SORTED_H
