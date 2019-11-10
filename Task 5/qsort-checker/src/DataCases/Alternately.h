#ifndef _MENCI_QSORT_CHECKER_SORTTYPE_ALTETNATELY_H
#define _MENCI_QSORT_CHECKER_SORTTYPE_ALTETNATELY_H

#include <algorithm>

struct DataCaseAlternately {
    inline static const char *NAME = "Small / Large Alternately";

    template <typename DataType>
    static void processData(std::vector<typename DataType::Type> &array) {
        std::sort(array.begin(), array.end(), [] (const typename DataType::Type &a, const typename DataType::Type &b) {
            return DataType::compare(a, b) == -1;
        });

        std::vector<typename DataType::Type> tmp = array;
        size_t m = array.size() / 2;
        for (size_t i = 0; i < m; i++) array[i * 2] = tmp[tmp.size() - i - 1];
        for (size_t i = 0; i < m; i++) array[i * 2 + 1] = tmp[i];
        if (array.size() % 2 == 1) array[array.size() - 1] = tmp[tmp.size() / 2];
    }
};

#endif // _MENCI_QSORT_CHECKER_SORTTYPE_ALTETNATELY_H
