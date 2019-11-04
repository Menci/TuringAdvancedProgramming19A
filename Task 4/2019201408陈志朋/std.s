	.file	"std.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	movq	b(%rip), %rax
	cqto
	idivq	c(%rip)
	movq	%rax, a(%rip)
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.globl	c
	.data
	.align 8
	.type	c, @object
	.size	c, 8
c:
	.quad	3
	.globl	b
	.align 8
	.type	b, @object
	.size	b, 8
b:
	.quad	2
	.globl	a
	.align 8
	.type	a, @object
	.size	a, 8
a:
	.quad	1
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
