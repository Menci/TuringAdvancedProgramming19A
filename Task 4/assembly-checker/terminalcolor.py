from enum import Enum

class TerminalColor(Enum):
    RESET             = 0
    BOLD              = 1

    FOREGROUNDBLACK   = 30
    FOREGROUNDRED     = 31
    FOREGROUNDGREEN   = 32
    FOREGROUNDYELLOW  = 33
    FOREGROUNDBLUE    = 34
    FOREGROUNDMAGENTA = 35
    FOREGROUNDCYAN    = 36
    FOREGROUNDWHITE   = 37

    BACKGROUNDBLACK   = 40
    BACKGROUNDRED     = 41
    BACKGROUNDGREEN   = 42
    BACKGROUNDYELLOW  = 43
    BACKGROUNDBLUE    = 44
    BACKGROUNDMAGENTA = 45
    BACKGROUNDCYAN    = 46
    BACKGROUNDWHITE   = 47

def colorize(text, foreground=None, background=None, bold=False):
    def ansi_sgr(codename):
        return "\033[%dm" % TerminalColor[codename.upper()].value

    seq = ""
    if bold:
        seq += ansi_sgr("BOLD")
    if foreground:
        seq += ansi_sgr("FOREGROUND" + foreground)
    if background:
        seq += ansi_sgr("BACKGROUND" + background)

    return ansi_sgr("RESET") + seq + str(text) + ansi_sgr("RESET")
