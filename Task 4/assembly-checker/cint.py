from ctypes import cdll, c_int
from os import path
from tempfile import TemporaryDirectory
import subprocess

from utils import format_stderr

ccode = '''
int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int div(int a, int b) {
    return a / b;
}

int xor(int a, int b) {
    return a ^ b;
}
'''

clib = None

def init():
    global clib
    tempdir = TemporaryDirectory()
    cfile = path.join(tempdir.name, "a.c")
    dllfile = path.join(tempdir.name, "a.so")

    with open(cfile, "w") as f:
        f.write(ccode)
    
    process = subprocess.Popen("gcc %s -o %s -shared" % (cfile, dllfile), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    if process.returncode != 0:
        print("Error compiling the C operation library:\n" + format_stderr(stderr))
        exit(1)
    
    try:
        clib = cdll.LoadLibrary(dllfile)
    except Exception as e:
        print("Error loading C operation library: " + str(e))
        exit(1)

class CInt:
    def __init__(self, value = 0):
        self.value = value
    
    def __add__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.add(c_int(self.value), c_int(other.value)))
    
    def __sub__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.sub(c_int(self.value), c_int(other.value)))
    
    def __mul__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.mul(c_int(self.value), c_int(other.value)))
    
    def __truediv__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        if other.value == 0:
            raise ZeroDivisionError("C integer division by zero")
        return CInt(clib.div(c_int(self.value), c_int(other.value)))
        
    def __xor__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.xor(c_int(self.value), c_int(other.value)))
    
init()
