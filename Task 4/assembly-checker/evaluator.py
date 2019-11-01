from cint import CInt

def evaluate(expressions, arguments, variable_count):
    for i in range(variable_count):
        varname = "v%d" % (i + 1)

    expressions = "\n".join(["v%d = CInt()" % (i + 1) for i in range(variable_count)]) + "\n" + expressions
    expressions = "\n".join(["a%d = CInt(%d)" % (i + 1, arguments[i]) for i in range(len(arguments))]) + "\n" + expressions

    try:
        exec(expressions)
        return eval("(%s).value" % " ^ ".join(["v%d" % (i + 1) for i in range(variable_count)]))
    except ZeroDivisionError:
        return None
