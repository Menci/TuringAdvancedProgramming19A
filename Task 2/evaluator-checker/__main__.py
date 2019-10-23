from argparse import ArgumentParser
from random import randint
from math import isnan, isinf
from tempfile import NamedTemporaryFile
import subprocess

from generator import ExpressionGenerator
from terminalcolor import colorize
from utils import float_to_string

def check_relative_error(value, output_value):
    MAX_RELATIVE_ERROR = 1e-8
    if isnan(value):
        return isnan(output_value)
    elif isinf(value):
        return value == output_value
    elif value == 0:
        return abs(value) <= MAX_RELATIVE_ERROR
    else:
        return abs(value - output_value) <= MAX_RELATIVE_ERROR or abs((value - output_value) / value) <= MAX_RELATIVE_ERROR

def print_error_and_exit(expression_string, value, output):
    MAX_PRINT_LENGTH = 250
    expression_output = None
    if len(expression_string) > MAX_PRINT_LENGTH:
        temp = NamedTemporaryFile("w+", encoding="utf-8", delete=False)
        temp.write(expression_string + "\n")
        expression_output = "Too large, written to %s" % colorize(temp.name, foreground="yellow", bold=True)
    else:
        expression_output = expression_string
    print("\t Expression: %s\n\t      Value: %s\n\tYour Output: %s\n"
          % (expression_output,
             colorize(float_to_string(value), bold=True),
             colorize(output, bold=True)))
    exit(1)

def main():
    parser = ArgumentParser(prog="evaluator-checker", description="Checker for your expression evaluator implementation")
    parser.add_argument("-r", "--check-rounds",
                        help="The number of rounds to check",
                        type=int, default=1000)
    parser.add_argument("-l", "--expression-length",
                        help="The length of generated expressions (the number of numbers in it)",
                        type=int, default=100)
    parser.add_argument("-s", "--seed",
                        help="The random seed for RNG (zero for a random value)",
                        type=int, default=0)
    parser.add_argument("executable",
                        help="Your expression evaluator executable file (e.g. a.out)",
                        type=str)
    args = parser.parse_args()

    seed = args.seed
    if seed == 0:
        seed = randint(1, 1000000)
        print("Running evaluator checker with random seed %d.\n" % seed)
    else:
        print("Running evaluator checker with specfied seed %d.\n" % seed)

    generator = ExpressionGenerator(seed, args.expression_length)

    process = None
    try:
        process = subprocess.Popen(args.executable, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    except BaseException as e:
        print("Error spawning your execuable: %s" % colorize(e, bold=True))
        exit(1)

    print("Checking expression evaluating for %s rounds." % colorize(args.check_rounds, bold=True))
    for i in range(args.check_rounds):
        print("Running on %s..." % (colorize(i + 1, bold=True)), end="\r")
        expression = generator.generate()
        expression_string = expression.to_string()
        value = expression.evaluate()
        try:
            process.stdin.write(bytes(expression_string + "\n", encoding="utf-8"))
            process.stdin.flush()
        except BaseException as e:
            if e is KeyboardInterrupt:
                exit(1)
            print("\n\nError writing expression to your input on round %s (%05.2f%%): %s" % (colorize(i + 1, bold=True), (i + 1) / args.check_rounds, colorize(e, bold=True)))
            print_error_and_exit(expression_string, value, None)

        output_line = None
        try:
            output_line = str(process.stdout.readline(), encoding="utf-8").strip()
        except BaseException as e:
            if e is KeyboardInterrupt:
                exit(1)
            print("\n\nError reading your output on round %s (%05.2f%%): %s" % (colorize(i + 1, bold=True), (i + 1) / args.check_rounds, colorize(e, bold=True)))
            print_error_and_exit(expression_string, value, None)

        error_message = None

        output_value = None
        try:
            output_value = float(output_line)
        except ValueError as e:
            print("\n\nError parsing output on round %s (%05.2f%%): %s" % (colorize(i + 1, bold=True), (i + 1) / args.check_rounds, colorize(e, bold=True)))
            print_error_and_exit(expression_string, value, output_line)

        if error_message or not check_relative_error(value, output_value):
            print("\n\nWrong output on round %s (%05.2f%%):" % (colorize(i + 1, bold=True), (i + 1) / args.check_rounds))
            print_error_and_exit(expression_string, value, output_line)

    print(colorize("\n\nAll right, congratulations!", foreground="green", bold=True))

try:
    main()
except KeyboardInterrupt as e:
    exit(1)
