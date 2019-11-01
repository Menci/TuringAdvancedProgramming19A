import subprocess
import ctypes
from pathlib import Path
from random import Random, randint
from os import system, listdir, path, chdir
from tempfile import TemporaryDirectory
from argparse import ArgumentParser
from functools import cmp_to_key

from terminalcolor import colorize
from evaluator import evaluate
from utils import format_stderr

TESTDATA_PATH = "testdata"
ARGUMENT_COUNT = 3
VARIABLE_COUNT = 60

chdir(Path(__file__).resolve().parent)

def get_all_testdata():
    def compare_function(a, b):
        if len(a) == len(b):
            if a < b:
                return -1
            elif a == b:
                return 0
            else:
                return 1
        else:
            return len(a) - len(b)

    testdatas = [path.join(TESTDATA_PATH, f) for f in listdir(TESTDATA_PATH) if not f.endswith(".py") and path.isfile(path.join(TESTDATA_PATH, f))]
    testdatas.sort(key=cmp_to_key(compare_function))
    return testdatas

def read_testdata(file):
    try:
        return str(open(file).read())
    except Exception as e:
        print("Error while reading testdata '%s': %s" % (file, str(e)))
        exit(1)

def run_generator(execuatable, input):
    try:
        process = subprocess.Popen(execuatable, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate(bytes(input, "utf-8"))
        if process.returncode != 0:
            if len(stderr):
                print("Your program exited with a non-zero return code %d, with message:\n" % process.returncode + format_stderr(stderr))
            else:
                print("Your program exited with a non-zero return code %d" % process.returncode)
            exit(1)
        elif len(stderr):
            print("Your program exited with message:\n" + format_stderr(stderr))
        return stdout.decode("utf-8")
    except Exception as e:
        print("Error while running your program: %s" % str(e))
        exit(1)

def random_arguments(rng):
    return [rng.randint(-10000, 10000) for i in range(ARGUMENT_COUNT)]

def compile_asm_and_load(code):
    tempdir = TemporaryDirectory()
    asmfile = path.join(tempdir.name, "a.s")
    objfile = path.join(tempdir.name, "a.o")
    dllfile = path.join(tempdir.name, "a.so")

    with open(asmfile, "w") as f:
        f.write(".global f\nf:\n" + code)
    
    process = subprocess.Popen("as %s -o %s" % (asmfile, objfile), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    if process.returncode != 0:
        print("Error compiling your output assembly code:\n" + format_stderr(stderr))
        exit(1)
    
    process = subprocess.Popen("ld -shared %s -o %s" % (objfile, dllfile), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    if process.returncode != 0:
        print("Error linking the compiled object of your output assembly code:\n" + format_stderr(stderr))
        exit(1)
    
    try:
        return ctypes.cdll.LoadLibrary(dllfile)
    except Exception as e:
        print("Error loading C operation library: " + str(e))
        exit(1)

def print_error_and_exit(arguments, answer, asm_output):
    print("\t  Arguments: %s\n\t     Answer: %s\n\tYour Output: %s\n"
          % (repr(arguments),
             colorize(answer, bold=True),
             colorize(asm_output, bold=True)))
    exit(1)

def main():
    parser = ArgumentParser(prog="assembly-checker", description="Checker for your assembly code generator implementation")
    parser.add_argument("-r", "--check-rounds",
                        help="The number of rounds (i.e. groups of random arguments) to check, for each testdata",
                        type=int, default=1000)
    parser.add_argument("-d", "--testdata",
                        help="The testdata file to check (default all files in testdata dir)",
                        type=str, default="")
    parser.add_argument("-s", "--seed",
                        help="The random seed for RNG (zero for a random value)",
                        type=int, default=0)
    parser.add_argument("executable",
                        help="Your assembly code generator executable file (e.g. a.out)",
                        type=str)
    args = parser.parse_args()

    seed = args.seed
    if seed == 0:
        seed = randint(1, 1000000)
        print("Running assembly code generator checker with random seed %d.\n" % seed)
    else:
        print("Running assembly code generator checker with specfied seed %d.\n" % seed)

    rng = Random(seed)
    
    testdatas = [args.testdata] if args.testdata else get_all_testdata()
    for testdata in testdatas:
        print("Running testdata '%s':" % testdata)
        inputcode = read_testdata(testdata)
        asmcode = run_generator(args.executable, inputcode)

        print("Compiling your assembly code.")
        dllasm = compile_asm_and_load(asmcode)
        
        for i in range(args.check_rounds):
            print("Running on %s..." % (colorize(i + 1, bold=True)), end="\r")

            # Generate a group of random arguments without ZeroDivisionError
            arguments = random_arguments(rng)
            answer = evaluate(inputcode, arguments, VARIABLE_COUNT)
            if answer == None:
                continue
            
            asm_result = dllasm.f(*[ctypes.c_int(argument) for argument in arguments])
            
            if answer != asm_result:
                print("\nWrong output on round %s (%05.2f%%):" % (colorize(i + 1, bold=True), (i + 1) / args.check_rounds))
                print_error_and_exit(arguments, answer, asm_result)
        
        print(colorize("\nPassed!\n", bold=True))

try:
    main()
except KeyboardInterrupt as e:
    exit(1)
