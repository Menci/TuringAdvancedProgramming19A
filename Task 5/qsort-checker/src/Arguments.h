#ifndef _MENCI_QSORT_CHECKER_ARGUMENTS_H
#define _MENCI_QSORT_CHECKER_ARGUMENTS_H

#include <string>
#include <vector>

struct Arguments {
    std::string dynamicLibrary;
    size_t dataSize;
    unsigned int seed;
    std::string functionName;
};

Arguments parseArguments(int argc, char *argv[]);

#endif // _MENCI_QSORT_CHECKER_ARGUMENTS_H
