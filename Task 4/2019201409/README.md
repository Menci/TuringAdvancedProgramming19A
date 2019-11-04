# 伪编译器

上一个大作业魔改了一下，只用了6个Caller-Saved的寄存器暂存各种变量，并没有用Callee-Saved的几个寄存器。

%rax、%rdx专用于除法，%rcx用于处理idivq不能加imm的问题。