def format_stderr(stderr):
    return "\n".join(["    " + s for s in stderr.decode("utf-8").split("\n")])
