	.file	"align.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"Taille d'un element de t = %zu\n"
	.align 8
.LC1:
	.string	"Offset t[0].unInt    = %2zu (sizeof = %1zu)\n"
	.align 8
.LC2:
	.string	"Offset t[0].unShort  = %2zu (sizeof = %1zu)\n"
	.align 8
.LC3:
	.string	"Offset t[0].unShort2 = %2zu (sizeof = %1zu)\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
	.string	""
	.text
	.p2align 4,,15
	.globl	printTableInfo
	.type	printTableInfo, @function
printTableInfo:
.LFB24:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$8, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movl	$4, %edx
	xorl	%esi, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	$2, %edx
	movl	$4, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	printf
	movl	$.LC3, %edi
	movl	$2, %edx
	movl	$6, %esi
	xorl	%eax, %eax
	call	printf
	movl	$.LC4, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	puts
	.cfi_endproc
.LFE24:
	.size	printTableInfo, .-printTableInfo
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB25:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	call	printTableInfo
	xorl	%eax, %eax
	call	getrusageStart
	movl	$t, %eax
	movl	$t+400000000, %edx
	.p2align 4,,10
	.p2align 3
.L4:
	movl	$1234567890, (%rax)
	addq	$8, %rax
	cmpq	%rdx, %rax
	jne	.L4
	xorl	%eax, %eax
	call	getrusageStop
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE25:
	.size	main, .-main
	.comm	t,400000000,32
	.ident	"GCC: (GNU) 6.3.1 20170306"
	.section	.note.GNU-stack,"",@progbits
