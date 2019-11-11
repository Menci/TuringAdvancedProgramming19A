#ifndef _MENCI_QSORT_CHECKER_SORTTYPE_ALLSAME_H
#define _MENCI_QSORT_CHECKER_SORTTYPE_ALLSAME_H

#include <algorithm>

struct DataCaseAllSame {
    inline static const char *NAME = "All Same Elements";

    template <typename DataType>
    static void processData(std::vector<typename DataType::Type> &array) {
        if (array.size() == 0) return;
        std::fill(array.begin() + 1, array.end(), array.front());
    }
};

#endif // _MENCI_QSORT_CHECKER_SORTTYPE_ALLSAME_H
