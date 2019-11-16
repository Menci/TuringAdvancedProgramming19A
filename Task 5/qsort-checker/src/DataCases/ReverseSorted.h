#ifndef _MENCI_QSORT_CHECKER_SORTTYPE_REVERSESORTED_H
#define _MENCI_QSORT_CHECKER_SORTTYPE_REVERSESORTED_H

#include <algorithm>

struct DataCaseReverseSorted {
    inline static const char *NAME = "Reverse Sorted";

    template <typename DataType>
    static void processData(std::vector<typename DataType::Type> &array) {
        std::sort(array.begin(), array.end(), [] (const typename DataType::Type &a, const typename DataType::Type &b) {
            return DataType::compare(a, b) == -1;
        });

        std::reverse(array.begin(), array.end());
    }
};

#endif // _MENCI_QSORT_CHECKER_SORTTYPE_REVERSESORTED_H
