	.file	"clone-copy.c"
	.text
	.section	.rodata
	.align 4
	.type	STACK_SIZE, @object
	.size	STACK_SIZE, 4
STACK_SIZE:
	.long	8192
	.text
	.globl	recursiveHelloWorld
	.type	recursiveHelloWorld, @function
recursiveHelloWorld:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$9, -36(%rbp)
	jg	.L4
	movabsq	$6278066737626506568, %rax
	movq	%rax, -23(%rbp)
	movabsq	$2851464966991703, %rax
	movq	%rax, -16(%rbp)
	movl	-36(%rbp), %eax
	addl	$1, %eax
	movl	%eax, %edi
	call	recursiveHelloWorld
.L4:
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	recursiveHelloWorld, .-recursiveHelloWorld
	.globl	enterPoint
	.type	enterPoint, @function
enterPoint:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	$0, %edi
	call	recursiveHelloWorld
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	enterPoint, .-enterPoint
	.section	.rodata
.LC0:
	.string	"%s: "
	.text
	.globl	errorHandler
	.type	errorHandler, @function
errorHandler:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$288, %rsp
	movq	%rdi, -280(%rbp)
	movq	%rsi, -288(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-288(%rbp), %rdx
	leaq	-272(%rbp), %rax
	leaq	.LC0(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf@PLT
	movq	-280(%rbp), %rdx
	leaq	-272(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcat@PLT
	leaq	-272(%rbp), %rax
	movq	%rax, %rdi
	call	perror@PLT
	call	__errno_location@PLT
	movl	(%rax), %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	errorHandler, .-errorHandler
	.section	.rodata
.LC1:
	.string	"./stack.shared"
.LC2:
	.string	"error opening file"
.LC3:
	.string	"error calling ftruncate"
.LC4:
	.string	"error calling mmap"
.LC5:
	.string	"error calling clone"
.LC6:
	.string	"error waiting for child"
.LC7:
	.string	"error closing file"
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$448, %edx
	movl	$578, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -16(%rbp)
	cmpl	$-1, -16(%rbp)
	jne	.L11
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	errorHandler
	jmp	.L12
.L11:
	movl	$8192, %eax
	movslq	%eax, %rdx
	movl	-16(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	ftruncate@PLT
	testl	%eax, %eax
	je	.L13
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	errorHandler
	jmp	.L12
.L13:
	movl	$8192, %eax
	cltq
	movl	-16(%rbp), %edx
	movl	$0, %r9d
	movl	%edx, %r8d
	movl	$1, %ecx
	movl	$3, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	mmap@PLT
	movq	%rax, -8(%rbp)
	cmpq	$-1, -8(%rbp)
	jne	.L14
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	errorHandler
	jmp	.L12
.L14:
	movl	$8192, %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movl	$0, %ecx
	movl	$256, %edx
	movq	%rax, %rsi
	leaq	enterPoint(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	clone@PLT
	movl	%eax, -12(%rbp)
	cmpl	$-1, -12(%rbp)
	jne	.L15
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	errorHandler
	jmp	.L12
.L15:
	movl	-12(%rbp), %eax
	movl	$-2147483648, %edx
	movl	$0, %esi
	movl	%eax, %edi
	call	waitpid@PLT
	cmpl	$-1, %eax
	jne	.L16
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	call	errorHandler
	jmp	.L12
.L16:
	movl	-16(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
	cmpl	$-1, %eax
	jne	.L17
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	call	errorHandler
	jmp	.L12
.L17:
	movl	$0, %eax
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (GNU) 13.1.1 20230429"
	.section	.note.GNU-stack,"",@progbits
