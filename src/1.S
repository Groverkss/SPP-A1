# Loop: k, i, j 
# mat[i][j] += matA[i][k] * matB[k][j]

assemblyMultiply:
.Start:
	pushq	%r12
	testl	%ecx, %ecx
	pushq	%rbp
	pushq	%rbx
	movl	32(%rsp), %ebx
	jle	.Loop1
	leal	-1(%rcx), %eax
	movq	%rdi, %rbp
	movslq	%r9d, %rdi
	salq	$3, %rdi
	movq	%rsi, %r8
	movq	%rdx, %r10
	leaq	8(%rsi,%rax,8), %r12
.Loop2:
	testl	%ebx, %ebx
	jle	.Loop2End
	xorl	%esi, %esi
	xorl	%r11d, %r11d
.Loop3:
	testl	%r9d, %r9d
	jle	.Loop1End
	leaq	0(%rbp,%rsi), %rcx
	xorl	%eax, %eax
.Multiply:
	# (%r8, %rsi) --> matA[i][k]
	# (%)
	movq	(%r8,%rsi), %rdx			# Move whatever at %r8 + %rsi to %rdx 
	imulq	(%r10,%rax,8), %rdx			# Multiply contents at %c10 + %rax*8 with %rdx and store at %rds 
	addq	%rdx, (%rcx,%rax,8)			# Add %rdx at %rcx + 8*%rax
	addq	$1, %rax					# Add 1 to %rax
	cmpl	%eax, %r9d					# Check if %eax == %r9d
	jg	.Multiply
.Loop1End:
	addl	$1, %r11d
	addq	%rdi, %rsi
	cmpl	%ebx, %r11d
	jne	.Loop3
.Loop2End:
	addq	$8, %r8
	addq	%rdi, %r10
	cmpq	%r12, %r8
	jne	.Loop2
.Loop1:
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
