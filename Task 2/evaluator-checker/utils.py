def float_to_string(x):
    l = list("%.1000f" % x)
    while l[-1] == "0":
        l.pop()
    if l[-1] == ".":
        l.pop()
    return ''.join(l)
