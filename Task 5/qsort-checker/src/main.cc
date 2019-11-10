#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>
#include <cstdlib>

#include <dlfcn.h>

#include "TerminalColor/TerminalColor.h"

#include "Arguments.h"
#include "Utility.h"

#include "DataTypes/NormalInteger.h"
#include "DataTypes/OneByteInteger.h"
#include "DataTypes/UnalignedStruct.h"
#include "DataTypes/LargeStruct.h"

#include "DataCases/Random.h"
#include "DataCases/Sorted.h"
#include "DataCases/ReverseSorted.h"
#include "DataCases/Alternately.h"
#include "DataCases/AllSame.h"

typedef decltype(qsort) *qsort_t;

TerminalColor getTimeColor(double timeByLibcQsort, double time) {
    if (time <= timeByLibcQsort) return TerminalColor::ForegroundGreen;
    if (time <= timeByLibcQsort * 2) return TerminalColor::ForegroundYellow;
    if (time <= timeByLibcQsort * 4) return TerminalColor::ForegroundMagenta;
    return TerminalColor::ForegroundRed;
}

std::function<int (const void *, const void *)> qsortCompareFunction;

int qsortCompareFunctionStub(const void *a, const void *b) {
    return qsortCompareFunction(a, b);
}

template <typename DataType, typename DataCase, typename RNG>
inline void checkForType(size_t dataSize, RNG &rng, qsort_t qsortFunction) {
    typedef typename DataType::Type Type;

    std::cout << "    "
              << "● Case: "
              << TerminalColor::Bold
              << DataCase::NAME
              << TerminalColor::Reset
              << std::endl;
    
    std::vector<Type> originData;
    double timeGenerateData = measureTime([&]() {
        originData.reserve(dataSize);
        for (size_t i = 0; i < dataSize; i++) originData.push_back(DataType::random(rng));
        DataCase::template processData<DataType>(originData);
    });
    std::cout << "        "
              << "        Data generated in "
              << TerminalColor::Bold
              << timeGenerateData << "s"
              << TerminalColor::Reset
              << std::endl;

    auto cppCompareFunction = [](const Type &a, const Type &b) {
        return DataType::compare(a, b) == -1;
    };
    qsortCompareFunction = [](const void *pa, const void *pb) {
        return DataType::compare(*reinterpret_cast<const Type *>(pa), *reinterpret_cast<const Type *>(pb));
    };

    std::vector<Type> byLibcQsort = originData;
    double timeByLibcQsort = measureTime([&]() {
        qsort(byLibcQsort.data(), byLibcQsort.size(), sizeof(Type), qsortCompareFunctionStub);
    });

    std::cout << "        "
              << "   The libc qsort() costs "
              << TerminalColor::Bold << getTimeColor(timeByLibcQsort, timeByLibcQsort)
              << timeByLibcQsort << "s"
              << TerminalColor::Reset
              << std::endl;

    std::vector<Type> byStdSort = originData;
    double timeByStdSort = measureTime([&]() {
        std::sort(byStdSort.begin(), byStdSort.end(), cppCompareFunction);
    });

    std::cout << "        "
              << "The C++ std::sort() costs "
              << TerminalColor::Bold << getTimeColor(timeByLibcQsort, timeByStdSort)
              << timeByStdSort << "s"
              << TerminalColor::Reset
              << std::endl;

    std::vector<Type> byYourQsort = originData;
    double timeByYourQsort = measureTime([&]() {
        qsortFunction(byYourQsort.data(), byYourQsort.size(), sizeof(Type), qsortCompareFunctionStub);
    });

    std::cout << "        "
              << "       Your qsort() costs "
              << TerminalColor::Bold << getTimeColor(timeByLibcQsort, timeByYourQsort)
              << timeByYourQsort << "s"
              << TerminalColor::Reset
              << std::endl;

    bool isSorted = std::is_sorted(byYourQsort.begin(), byYourQsort.end(), cppCompareFunction);

    if (!isSorted) {
        std::cout << "        "
                  << TerminalColor::Bold << TerminalColor::ForegroundRed
                  << "✖ Wrong Answer!"
                  << TerminalColor::Reset << TerminalColor::Bold
                  << " Your qsort function can't sort the data correctly."
                  << TerminalColor::Reset
                  << std::endl;
    } else {
        bool isSameData = true;
        for (auto &element : originData) {
            if (!std::binary_search(byYourQsort.begin(), byYourQsort.end(), element, cppCompareFunction)) {
                isSameData = false;
                break;
            }
        }

        if (!isSameData) {
            std::cout << "        "
                      << TerminalColor::Bold << TerminalColor::ForegroundMagenta
                      << "✖ Wrong Answer!"
                      << TerminalColor::Reset << TerminalColor::Bold
                      << " Your qsort function modified the elements in array!"
                      << TerminalColor::Reset
                      << std::endl;
        } else {
            std::cout << "        "
                      << TerminalColor::Bold << TerminalColor::ForegroundGreen
                      << "✔ Right Answer!"
                      << TerminalColor::Reset << TerminalColor::Bold
                      << " Your qsort function sorts the data correctly."
                      << TerminalColor::Reset
                      << std::endl;
        }
    }
}

template <typename DataType, typename RNG>
inline void check(size_t dataSize, RNG &rng, qsort_t qsortFunction) {
    std::cout << std::endl
              << "Running with data type: "
              << TerminalColor::Bold
              << DataType::NAME
              << TerminalColor::Reset
              << std::endl;
    checkForType<DataType, DataCaseRandom>(dataSize, rng, qsort);
    checkForType<DataType, DataCaseSorted>(dataSize, rng, qsort);
    checkForType<DataType, DataCaseReverseSorted>(dataSize, rng, qsort);
    checkForType<DataType, DataCaseAlternately>(dataSize, rng, qsort);
    checkForType<DataType, DataCaseAllSame>(dataSize, rng, qsort);
}

int main(int argc, char *argv[]) {
    Arguments arguments = parseArguments(argc, argv);

    // Load dynamic library
    void *handle = dlopen(arguments.dynamicLibrary.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Can't open dynamic library, error: " << dlerror() << std::endl;
        return 1;
    } else dlerror(); // Reset dlerror on success

    // Get function
    qsort_t qsortFunction = (qsort_t)dlsym(handle, arguments.functionName.c_str());
    if (!qsortFunction) {
        std::cerr << "Can't get function, error: " << dlerror() << std::endl;
        return 1;
    }
    qsortFunction = &qsort;

    // Initialize RNG
    uint64_t seed = arguments.seed;
    if (seed == 0) {
        seed = std::random_device()();
        std::cout << "Running qsort checker with random seed " << seed << "." << std::endl;
    } else {
        std::cout << "Running qsort checker with specfied seed " << seed << "." << std::endl;
    }

    std::mt19937_64 rng(seed);

    check<DataNormalInteger>(arguments.dataSize, rng, qsortFunction);
    check<DataOneByteInteger>(arguments.dataSize, rng, qsortFunction);
    check<DataLargeStruct>(arguments.dataSize, rng, qsortFunction);
    check<DataUnalignedStruct>(arguments.dataSize, rng, qsortFunction);
}
