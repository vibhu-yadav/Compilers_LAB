	.file	"sys.cpp"
# GNU C++17 (Ubuntu 11.2.0-19ubuntu1) version 11.2.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$32, %rsp	#,
# sys.cpp:5: {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp84
	movq	%rax, -8(%rbp)	# tmp84, D.3101
	xorl	%eax, %eax	# tmp84
# sys.cpp:6:     char str[LEN] = "My second program\n";
	movabsq	$7957688353961310541, %rax	#, tmp86
	movabsq	$7021788497383006308, %rdx	#,
	movq	%rax, -32(%rbp)	# tmp86, str
	movq	%rdx, -24(%rbp)	#, str
	movw	$2669, -16(%rbp)	#, str
	movb	$0, -14(%rbp)	#, str
# sys.cpp:7:     write(1,str,LEN);
	leaq	-32(%rbp), %rax	#, tmp83
	movl	$19, %edx	#,
	movq	%rax, %rsi	# tmp83,
	movl	$1, %edi	#,
	call	write@PLT	#
# sys.cpp:8:     _exit(0);
	movl	$0, %edi	#,
	call	_exit@PLT	#
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
