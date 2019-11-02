subq $64, %rsp
movq $0, %r11
movq %rsi, %r10
addq %r10, %rsi
xorq %rsi, %r11
movq %rbx, %r9
subq %rsi, %rbx
xorq %rbx, %r11
movq %rbx, %r8
imulq %r9, %rbx
xorq %rbx, %r11
movq %r10, 56(%rsp)
movq %rsi, %r10
addq %r9, %rsi
xorq %rsi, %r11
movq %rdi, 48(%rsp)
movq 56(%rsp), %rdi
movq %rdi, %rax
cqto
movq $27926, %rcx
idivq %rcx
movq %rax, %rdi
xorq %rdi, %r11
movq %r9, 40(%rsp)
movq %r10, %r9
subq %rsi, %r10
xorq %r10, %r11
movq %rdi, 32(%rsp)
movq %rbx, %rdi
addq %rsi, %rbx
xorq %rbx, %r11
movq %r8, 24(%rsp)
movq %rbx, %r8
subq 48(%rsp), %rbx
xorq %rbx, %r11
movq %r10, 16(%rsp)
movq %rsi, %r10
subq 48(%rsp), %rsi
xorq %rsi, %r11
movq 40(%rsp), %rsi
subq %r9, %rsi
xorq %rsi, %r11
movq %rsi, 8(%rsp)
movq %rdi, %rsi
movq %rdi, %rax
cqto
movq $13090, %rcx
idivq %rcx
movq %rax, %rdi
xorq %rdi, %r11
movq %r10, %rdi
imulq $22524, %r10
xorq %r10, %r11
movq %rsi, (%rsp)
movq %r10, %rsi
addq 56(%rsp), %r10
xorq %r10, %r11
imulq %r8, %r9
xorq %r9, %r11
movq %rbx, %rax
cqto
idivq 48(%rsp)
movq %rax, %rbx
xorq %rbx, %r11
movq 56(%rsp), %r8
imulq $25112, %r8
xorq %r8, %r11
subq %rdi, %r10
xorq %r10, %r11
movq 40(%rsp), %rdi
addq $29804, %rdi
xorq %rdi, %r11
movq 48(%rsp), %r10
movq %r10, %rax
cqto
idivq 8(%rsp)
movq %rax, %r10
xorq %r10, %r11
movq %r9, -8(%rsp)
movq 16(%rsp), %r9
addq 48(%rsp), %r9
xorq %r9, %r11
movq 56(%rsp), %r9
addq 40(%rsp), %r9
xorq %r9, %r11
movq %r9, -16(%rsp)
movq 56(%rsp), %r9
subq $-26461, %r9
xorq %r9, %r11
movq %rsi, -24(%rsp)
movq 48(%rsp), %rsi
imulq (%rsp), %rsi
xorq %rsi, %r11
movq %rsi, -32(%rsp)
movq 48(%rsp), %rsi
movq %rsi, %rax
cqto
idivq 40(%rsp)
movq %rax, %rsi
xorq %rsi, %r11
movq 48(%rsp), %rsi
movq %rsi, %rax
cqto
idivq (%rsp)
movq %rax, %rsi
xorq %rsi, %r11
movq %rdi, -40(%rsp)
movq 56(%rsp), %rdi
imulq $-22869, %rdi
xorq %rdi, %r11
movq %rdi, -48(%rsp)
movq 56(%rsp), %rdi
imulq 56(%rsp), %rdi
xorq %rdi, %r11
movq 40(%rsp), %rdi
movq %rdi, %rax
cqto
idivq %r8
movq %rax, %rdi
xorq %rdi, %r11
movq 16(%rsp), %rdi
imulq 48(%rsp), %rdi
xorq %rdi, %r11
movq %rdi, -56(%rsp)
movq 56(%rsp), %rdi
movq %rdi, %rax
cqto
idivq %r8
movq %rax, %rdi
xorq %rdi, %r11
movq %rsi, %r8
imulq $-28581, %rsi
xorq %rsi, %r11
movq %r9, %rax
cqto
idivq %r10
movq %rax, %r9
xorq %r9, %r11
imulq -16(%rsp), %rbx
xorq %rbx, %r11
movq 48(%rsp), %rsi
imulq 32(%rsp), %rsi
xorq %rsi, %r11
movq 48(%rsp), %rsi
imulq 40(%rsp), %rsi
xorq %rsi, %r11
movq 48(%rsp), %rsi
imulq 56(%rsp), %rsi
xorq %rsi, %r11
movq -40(%rsp), %rsi
addq 56(%rsp), %rsi
xorq %rsi, %r11
movq -48(%rsp), %rsi
addq 40(%rsp), %rsi
xorq %rsi, %r11
movq 40(%rsp), %rsi
subq 56(%rsp), %rsi
xorq %rsi, %r11
movq 56(%rsp), %r8
imulq 56(%rsp), %r8
xorq %r8, %r11
subq -24(%rsp), %rsi
xorq %rsi, %r11
movq 40(%rsp), %rsi
imulq 40(%rsp), %rsi
xorq %rsi, %r11
movq 40(%rsp), %rsi
movq %rsi, %rax
cqto
movq $-14676, %rcx
idivq %rcx
movq %rax, %rsi
xorq %rsi, %r11
movq 40(%rsp), %r8
addq 40(%rsp), %r8
xorq %r8, %r11
movq 40(%rsp), %r8
subq $11762, %r8
xorq %r8, %r11
movq 40(%rsp), %r8
subq $-29928, %r8
xorq %r8, %r11
movq 32(%rsp), %r8
addq 24(%rsp), %r8
xorq %r8, %r11
movq -32(%rsp), %r8
movq %r8, %rax
cqto
idivq %rdi
movq %rax, %r8
xorq %r8, %r11
imulq 56(%rsp), %rsi
xorq %rsi, %r11
movq 56(%rsp), %rdi
addq $-24108, %rdi
xorq %rdi, %r11
movq 56(%rsp), %rdi
subq 48(%rsp), %rdi
xorq %rdi, %r11
movq 56(%rsp), %rdi
addq $16662, %rdi
xorq %rdi, %r11
movq -24(%rsp), %rdi
imulq $7643, %rdi
xorq %rdi, %r11
movq 56(%rsp), %rdi
addq $22557, %rdi
xorq %rdi, %r11
movq -8(%rsp), %rdi
movq %rdi, %rax
cqto
idivq 48(%rsp)
movq %rax, %rdi
xorq %rdi, %r11
addq -56(%rsp), %rbx
xorq %rbx, %r11
movq -16(%rsp), %rdi
imulq 40(%rsp), %rdi
xorq %rdi, %r11
movq 56(%rsp), %rdi
movq %rdi, %rax
cqto
idivq 48(%rsp)
movq %rax, %rdi
xorq %rdi, %r11
movq 48(%rsp), %rdi
imulq 40(%rsp), %rdi
xorq %rdi, %r11
movq 56(%rsp), %rdi
addq $-8411, %rdi
xorq %rdi, %r11
addq $64, %rsp
movq %r11, %rax
ret
