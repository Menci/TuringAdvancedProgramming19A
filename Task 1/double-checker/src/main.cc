#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <bitset>
#include <cstdint>

#include <math.h>
#include <dlfcn.h>

#include "TerminalColor/TerminalColor.h"

#include "Arguments.h"
#include "Utility.h"

const double RELATIVE_ERROR_LIMIT = 1e-8;

using double_calc_t = uint64_t (*)(uint64_t, uint64_t, char);

double calcAnswer(double a, double b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            return 0;
    }
}

double randDouble(std::mt19937_64 &rng) {
    // The first method can generate more edge case while the second
    // method can prevent one number is less/greater than too many times
    // of another.
    if (rng() & 1) {
        return i2f(rng());
    } else {
        double a = (double)rng() * (double)(1ull << 63) + rng(),
               b = (double)rng() * (double)(1ull << 63) + rng();
        int sign = (rng() & 1) ? 1 : -1; 
        return a / b * sign;
    }   
}

constexpr size_t FP_LENGTH_SIGN = 1;
constexpr size_t FP_LENGTH_EXPONENT = 11;
constexpr size_t FP_LENGTH_FRACTION = 52;
constexpr size_t FP_BIAS = (int64_t)((1ULL << (FP_LENGTH_EXPONENT - 1)) - 1);

struct DoubleDetail {
    double value;
    std::string type, hexadecimal, decimalExponentNotation;
    bool sign;
    std::bitset<FP_LENGTH_EXPONENT> exponent;
    std::bitset<FP_LENGTH_FRACTION> fraction;
};

DoubleDetail inspectDouble(double x) {
    if (f2i(x) == 0x43dffc0000000000) throw 1;
    DoubleDetail result;
    result.value = x;

    switch (fpclassify(x)) {
        case FP_NAN:
            result.type = "not a number";
            break;
        case FP_INFINITE:
            result.type = "infinity";
            break;
        case FP_ZERO:
            result.type = "zero";
            break;
        case FP_SUBNORMAL:
            result.type = "denormalized";
            break;
        case FP_NORMAL:
            result.type = "normalized";
            break;
        default:
            throw;
    }

    constexpr size_t DOUBLE_PRINTF_BUFFER_LENGTH = 1024;
    char buffer[DOUBLE_PRINTF_BUFFER_LENGTH];
    sprintf(buffer, "0x%016llx", (unsigned long long)f2i(x));
    result.hexadecimal = buffer;
    sprintf(buffer, "%.8le", x);
    result.decimalExponentNotation = buffer;
    
    union {
        double x;
        struct {
            // On GNU/Linux x86_64 it's little endian, so we declare them
            // in reverse order.
            uint64_t fraction    : FP_LENGTH_FRACTION;
            uint64_t exponent    : FP_LENGTH_EXPONENT;
            uint64_t sign        : FP_LENGTH_SIGN;
        } structure;
    } inspector;

    inspector.x = x;
    result.sign = inspector.structure.sign;
    result.exponent = inspector.structure.exponent;
    result.fraction = inspector.structure.fraction;

    return result;
}

std::string formatPercentage(double x) {
    char buffer[sizeof("100.00%")];
    sprintf(buffer, "%05.2f%%", x * 100);
    std::string str = buffer;
    return str == "100.00%" ? "100.0%" : str;
}

void printDoubleDetail(std::string name,
                       DoubleDetail detail,
                       bool highlightSign = false,
                       size_t highlightExponentFrom = FP_LENGTH_EXPONENT,
                       size_t highlightFractionFrom = FP_LENGTH_FRACTION) {
    constexpr size_t NAME_LENGTH = 14 + 4;
    name = std::string(NAME_LENGTH - name.length(), ' ') + name;
    std::string fraction = detail.fraction.to_string(),
                fractionL = fraction.substr(0, highlightFractionFrom),
                fractionR = fraction.substr(highlightFractionFrom, fraction.length()),
                exponent = detail.exponent.to_string(),
                exponentL = exponent.substr(0, highlightExponentFrom),
                exponentR = exponent.substr(highlightExponentFrom, exponent.length());
    std::cout << name
              << " = "
              << TerminalColor::ForegroundMagenta
              << detail.decimalExponentNotation
              << TerminalColor::Reset
              << " ("
              << TerminalColor::ForegroundCyan
              << detail.type
              << TerminalColor::Reset
              << ")"
              << ", in 64-bit unsigned integer "
              << TerminalColor::ForegroundYellow
              << detail.hexadecimal
              << TerminalColor::Reset
              << std::endl
              << "            sign = "
              << (highlightSign ? TerminalColor::ForegroundRed : TerminalColor::Reset)
              << TerminalColor::Bold
              << (detail.sign ? '-' : '+')
              << TerminalColor::Reset << ", "
              << "fraction = "
              << TerminalColor::Bold
              << fractionL
              << TerminalColor::ForegroundRed
              << fractionR
              << TerminalColor::Reset
              << ", "
              << "exponent = "
              << TerminalColor::Bold
              << exponentL
              << TerminalColor::ForegroundRed
              << exponentR
              << TerminalColor::Reset
              << " = "
              << TerminalColor::Bold << TerminalColor::ForegroundBlue
              << ((long long)detail.exponent.to_ullong() - (long long)FP_BIAS)
              << TerminalColor::Reset
              << " + " << FP_BIAS
              << " = " << detail.exponent.to_ullong()
              << std::endl;
}

void printDiagnosticMessage(double a, double b, double answer, double userOutput) {
    DoubleDetail aDetail = inspectDouble(a),
                 bDetail = inspectDouble(b),
                 answerDetail = inspectDouble(answer),
                 userOutputDetail = inspectDouble(userOutput);
    printDoubleDetail("Operand a", aDetail);
    printDoubleDetail("Operand b", bDetail);

    bool highlightSign = answerDetail.sign != userOutputDetail.sign;
    size_t highlightExponentFrom = 0, highlightFractionFrom = 0;
    while (highlightExponentFrom < FP_LENGTH_EXPONENT
        && answerDetail.exponent[FP_LENGTH_EXPONENT - highlightExponentFrom - 1]
        == userOutputDetail.exponent[FP_LENGTH_EXPONENT - highlightExponentFrom - 1]) highlightExponentFrom++;
    while (highlightFractionFrom < FP_LENGTH_FRACTION
        && answerDetail.fraction[FP_LENGTH_FRACTION - highlightFractionFrom - 1]
        == userOutputDetail.fraction[FP_LENGTH_FRACTION - highlightFractionFrom - 1]) highlightFractionFrom++;

    printDoubleDetail("Correct answer", answerDetail, highlightSign, highlightExponentFrom, highlightFractionFrom);
    printDoubleDetail("Your result", userOutputDetail, highlightSign, highlightExponentFrom, highlightFractionFrom);
}

void printStatistics(size_t checkRounds,
                     double timeElapsedInSeconds,
                     size_t correctCount,
                     size_t partiallyCorrectCount,
                     size_t wrongCount) {
    const size_t CPS_GREEN = 3000000,
                 CPS_YELLOW = 500000;

    double calcPerSecond = checkRounds / timeElapsedInSeconds;
    TerminalColor cpsColor;
    if (calcPerSecond >= CPS_GREEN) cpsColor = TerminalColor::ForegroundGreen;
    else if (calcPerSecond >= CPS_YELLOW) cpsColor = TerminalColor::ForegroundYellow;
    else cpsColor = TerminalColor::ForegroundRed;

    std::cout << "Finished in "
              << std::setprecision(3)
              << std::fixed
              << cpsColor << TerminalColor::Bold
              << timeElapsedInSeconds
              << TerminalColor::Reset
              << " seconds, "
              << cpsColor << TerminalColor::Bold
              << calcPerSecond
              << TerminalColor::Reset
              << " calcs per second."
              << std::endl;
    if (correctCount == checkRounds + 1) {
        std::cout << TerminalColor::ForegroundGreen << TerminalColor::Bold
                  << "All"
                  << TerminalColor::Reset
                  << " correct!" << std::endl;
    } else {
        std::string strCorrectCount = std::to_string(correctCount),
                    strPartiallyCorrectCount = std::to_string(partiallyCorrectCount),
                    strWrongCount = std::to_string(wrongCount);
        size_t length = std::max(std::max(strCorrectCount.length(), strPartiallyCorrectCount.length()), strWrongCount.length()) + 4;
        strCorrectCount = std::string(length - strCorrectCount.length(), ' ') + strCorrectCount;
        strPartiallyCorrectCount = std::string(length - strPartiallyCorrectCount.length(), ' ') + strPartiallyCorrectCount;
        strWrongCount = std::string(length - strWrongCount.length(), ' ') + strWrongCount;

        std::cout << TerminalColor::ForegroundGreen << TerminalColor::Bold
                  << strCorrectCount << " "
                  << TerminalColor::Reset << TerminalColor::ForegroundGreen
                  << "(" + formatPercentage((double)correctCount / checkRounds) + ")"
                  << TerminalColor::Reset << TerminalColor::ForegroundGreen
                  << " correct"
                  << std::endl
                  << TerminalColor::ForegroundYellow << TerminalColor::Bold
                  << strPartiallyCorrectCount << " "
                  << TerminalColor::Reset << TerminalColor::ForegroundYellow
                  << "(" + formatPercentage((double)partiallyCorrectCount / checkRounds) + ")"
                  << TerminalColor::Reset << TerminalColor::ForegroundYellow
                  << " partially correct (0 < relative error < 1e-8)"
                  << std::endl
                  << TerminalColor::ForegroundRed << TerminalColor::Bold
                  << strWrongCount << " "
                  << TerminalColor::Reset << TerminalColor::ForegroundRed
                  << "(" + formatPercentage((double)wrongCount / checkRounds) + ")"
                  << TerminalColor::Reset << TerminalColor::ForegroundRed
                  << " wrong"
                  << std::endl;
    }
}

void check(char op, double_calc_t calcFunction, std::mt19937_64 &rng, size_t checkRounds) {
    std::uniform_int_distribution<uint64_t> gen;
    auto calc = [&](double a, double b, char op) { return i2f(calcFunction(f2i(a), f2i(b), op)); };

    std::cout << TerminalColor::Reset
              << "Checking operator "
              << TerminalColor::Bold
              << op
              << TerminalColor::Reset
              << " for "
              << TerminalColor::Bold
              << checkRounds
              << TerminalColor::Reset
              << " rounds:" << std::endl;

    struct Question {
        double a, b, answer, userOutput;
    };

    // Generate random questions and calculate answer
    std::vector<Question> questions(checkRounds);
    for (Question &question : questions) {
        question.a = randDouble(rng), question.b = randDouble(rng);
        question.answer = calcAnswer(question.a, question.b, op);
    }

    // Get user's output for each question and measure the time
    auto startTime = std::chrono::high_resolution_clock::now();
    for (Question &question : questions) {
        question.userOutput = calc(question.a, question.b, op);
    }
    auto endTime = std::chrono::high_resolution_clock::now();

    // Check
    bool nonCorrectOccured = false;
    size_t correctCount = 0, partiallyCorrectCount = 0, wrongCount = 0;
    for (size_t i = 0; i < checkRounds; i++) {
        double a = questions[i].a, b = questions[i].b;
        double userOutput = questions[i].userOutput, answer = questions[i].answer;

        bool correct = false, partiallyCorrect = false;
        if (isnan(answer)) {
            if (isnan(userOutput)) {
                correctCount++;
                correct = true;
            } else wrongCount++;
        } else if (isinf(answer)) {
            if (isinf(userOutput)) {
                correctCount++;
                correct = true;
            } else wrongCount++;
        } else if (userOutput == answer) {
            correctCount++;
            correct = true;
        } else {
            double relativeError = fabs(userOutput - answer) / fabs(answer);
            if (relativeError < RELATIVE_ERROR_LIMIT) partiallyCorrectCount++, partiallyCorrect = true;
            else wrongCount++;
        }

        if (!correct && !nonCorrectOccured) {
            // First non-correct
            nonCorrectOccured = true;
            std::cout << "First non-correct calculation ";
            if (partiallyCorrect) {
                std::cout << TerminalColor::ForegroundYellow
                          << "(partially correct)"
                          << TerminalColor::Reset;
            } else {
                std::cout << TerminalColor::ForegroundRed
                          << "(wrong)"
                          << TerminalColor::Reset;
            }
            std::cout << " occurs on round "
                      << TerminalColor::Bold
                      << (i + 1)
                      << TerminalColor::Reset
                      << " (" + formatPercentage((double)i / checkRounds) + "):"
                      << std::endl;
            printDiagnosticMessage(a, b, answer, userOutput);
        }
    }

    double timeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count(),
           timeElapsedInSeconds = timeElapsed / 1000000000;
    printStatistics(checkRounds, timeElapsedInSeconds, correctCount, partiallyCorrectCount, wrongCount);
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
    double_calc_t calcFunction = (double_calc_t)dlsym(handle, arguments.functionName.c_str());
    if (!calcFunction) {
        std::cerr << "Can't get function, error: " << dlerror() << std::endl;
        return 1;
    }

    // Initialize dynamic library
    calcFunction(0, 0, '+');

    // Initialize RNG
    uint64_t seed = arguments.seed;
    if (seed == 0) {
        seed = std::random_device()();
        std::cout << "Running double checker with random seed " << seed << "." << std::endl;
    } else {
        std::cout << "Running double checker with specfied seed " << seed << "." << std::endl;
    }
    std::mt19937_64 rng(seed);

    for (char op : arguments.operators) {
        std::cout << std::endl;
        check(op, calcFunction, rng, arguments.checkRounds);
    }
}
