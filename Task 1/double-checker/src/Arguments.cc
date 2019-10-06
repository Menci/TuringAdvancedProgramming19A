#include "Arguments.h"

#include "ArgumentParser/ArgumentParser.h"

Arguments parseArguments(int argc, char *argv[]) {
    Arguments arguments;
    ArgumentParser(argc, argv)
        .setProgramDescription("Checker for your double calculating implementation.")
        .addPositional("dynamic_library", "Your double implementation dynamic library. (e.g. double.so)",
                       ArgumentParser::stringParser(arguments.dynamicLibrary))
        .addOption("operators", "o",
                   "operators",
                   "The operators to check, in a string without any delimiter.",
                   [&](const std::string &str) -> std::optional<std::string> {
                       std::vector<char> operators;
                       for (char ch : str) {
                           switch (ch) {
                               case '+':
                               case '-':
                               case '*':
                               case '/':
                                   if (std::find(operators.begin(), operators.end(), ch) == operators.end())
                                       operators.push_back(ch);
                                   break;
                               default:
                                   return std::string("Invalid operator ") + ch;
                           }
                       }
                       arguments.operators = operators;
                       return std::nullopt;
                   },
                   true, "+-*/")
        .addOption("check-rounds", "r",
                   "number",
                   "The number of rounds to check, per operator.",
                   ArgumentParser::integerParser(arguments.checkRounds),
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
                   true, "double_checker_calc")
        .parse();
    return arguments;
}
