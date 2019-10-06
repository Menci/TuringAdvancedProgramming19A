#ifndef _MENCI_DOUBLE_CHECKER_ARGUMENTS_H
#define _MENCI_DOUBLE_CHECKER_ARGUMENTS_H

#include <string>
#include <vector>

struct Arguments {
    std::string dynamicLibrary;
    std::vector<char> operators;
    size_t checkRounds;
    unsigned int seed;
    std::string functionName;
};

Arguments parseArguments(int argc, char *argv[]);

#endif // _MENCI_DOUBLE_CHECKER_ARGUMENTS_H
