#ifndef _MENCI_QSORT_CHECKER_UTILITY_H
#define _MENCI_QSORT_CHECKER_UTILITY_H

#include <cstdio>
#include <functional>

std::string formatPercentage(double x) {
    char buffer[sizeof("100.00%")];
    sprintf(buffer, "%05.2f%%", x * 100);
    std::string str = buffer;
    return str == "100.00%" ? "100.0%" : str;
}

double measureTime(std::function<void ()> function) {
    auto startTime = std::chrono::high_resolution_clock::now();
    function();
    auto endTime = std::chrono::high_resolution_clock::now();

    double timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    return timeElapsed / 1000;
}

#endif // _MENCI_QSORT_CHECKER_UTILITY_H
