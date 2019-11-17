#include "Arguments.h"

#include "ArgumentParser/ArgumentParser.h"

Arguments parseArguments(int argc, char *argv[]) {
    Arguments arguments;
    ArgumentParser(argc, argv)
        .setProgramDescription("Checker for your qsort implementation.")
        .addPositional("dynamic_library", "Your qsort implementation dynamic library. (e.g. qsort.so)",
                       ArgumentParser::stringParser(arguments.dynamicLibrary))
        .addOption("data-size", "n",
                   "number",
                   "The size of array being sorted.",
                   ArgumentParser::integerParser(arguments.dataSize),
                   true, "2000000")
        .addOption("seed", "s",
                   "number",
                   "The random seed for RNG. Zero for a random value.",
                   ArgumentParser::integerParser(arguments.seed),
                   true, "0")
        .addOption("function-name", "f",
                   "name",
                   "The function name exported by your dynamic library.",
                   ArgumentParser::stringParser(arguments.functionName),
                   true, "turing_qsort")
        .parse();
    return arguments;
}
