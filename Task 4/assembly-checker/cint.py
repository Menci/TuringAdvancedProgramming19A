from ctypes import cdll, c_long
from os import path
from tempfile import TemporaryDirectory
import subprocess

from utils import format_stderr

ccode = '''
long add(long a, long b) {
    return a + b;
}

long sub(long a, long b) {
    return a - b;
}

long mul(long a, long b) {
    return a * b;
}

long div(long a, long b) {
    return a / b;
}

long xor(long a, long b) {
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

    clib.add.restype = c_long
    clib.sub.restype = c_long
    clib.mul.restype = c_long
    clib.div.restype = c_long
    clib.xor.restype = c_long

class CInt:
    def __init__(self, value = 0):
        self.value = value
    
    def __add__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.add(c_long(self.value), c_long(other.value)))
    
    def __sub__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.sub(c_long(self.value), c_long(other.value)))
    
    def __mul__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.mul(c_long(self.value), c_long(other.value)))
    
    def __truediv__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        if other.value == 0:
            raise ZeroDivisionError("C integer division by zero")
        return CInt(clib.div(c_long(self.value), c_long(other.value)))
        
    def __xor__(self, other):
        if type(other) != CInt:
            other = CInt(other)
        return CInt(clib.xor(c_long(self.value), c_long(other.value)))
    
init()
