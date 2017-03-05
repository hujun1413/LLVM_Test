	.file	"update.bc"
	.text
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rbp
.Ltmp4:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp5:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp6:
	.cfi_def_cfa_offset 32
.Ltmp7:
	.cfi_offset %rbx, -32
.Ltmp8:
	.cfi_offset %r14, -24
.Ltmp9:
	.cfi_offset %rbp, -16
	cmpl	$0, n(%rip)
                                        # implicit-def: R14D
	jle	.LBB0_3
# BB#1:
	xorl	%ebx, %ebx
                                        # implicit-def: R14D
	.align	16, 0x90
.LBB0_2:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	callq	rand
	movl	%eax, %ebp
	movl	%ebp, %edi
	callq	PrintWrite
	addl	%ebp, %r14d
	incl	%ebx
	cmpl	n(%rip), %ebx
	jl	.LBB0_2
.LBB0_3:                                # %for.end
	movl	$.L.str, %edi
	movl	%r14d, %esi
	xorb	%al, %al
	callq	printf
	movl	$.L_25__25_sum_20_outputted_20_by_20_Hujun_3A__0A_, %edi
	xorb	%al, %al
	callq	printf
	movl	%r14d, %edi
	callq	PrintWrite
	xorl	%eax, %eax
	popq	%rbx
	popq	%r14
	popq	%rbp
	ret
.Ltmp10:
	.size	main, .Ltmp10-main
	.cfi_endproc

	.type	n,@object               # @n
	.data
	.globl	n
	.align	4
n:
	.long	4                       # 0x4
	.size	n, 4

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	 "%d\n"
	.size	.L.str, 4

	.type	.L_25__25_sum_20_outputted_20_by_20_Hujun_3A__0A_,@object # @"%%sum outputted by Hujun:\0A"
	.section	.rodata,"a",@progbits
.L_25__25_sum_20_outputted_20_by_20_Hujun_3A__0A_:
	.asciz	 "sum outputted by Hujun:\n"
	.size	.L_25__25_sum_20_outputted_20_by_20_Hujun_3A__0A_, 25


	.section	".note.GNU-stack","",@progbits
