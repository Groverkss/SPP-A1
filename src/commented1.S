# Loop: k, i, j 
# mat[i][j] += matA[i][k] * matB[k][j]

# Loop1 --> k loop
# Loop2 --> i loop
# Loop3 --> j loop

# Function Parameters:
# %r9 --> col
# %r8 --> colB
# %rcx --> colA
# %rdx --> void *matBIn
# %rsi  --> void *matAIn
# %rdi   --> void *matIn
# row on stack

assemblyMultiply:
.Start:
	pushq	%r12					   # Save %r12
	testl	%ecx, %ecx				   # Check if %ecx is zero
	pushq	%rbp					   # Save %rbp
	pushq	%rbx					   # Save %rbx
	movl	32(%rsp), %ebx			   # Pull "row" from stack
	jle	.Loop1End					   # If %ecx is zero, end
	leal	-1(%rcx), %eax			   # Move address %rcx - 4 to %rax
	movq	%rdi, %rbp
	movslq	%r9d, %rdi
	salq	$3, %rdi
	movq	%rsi, %r8				   # Use %r8 for matAIn
	movq	%rdx, %r10				   # Use %r10 for matBIn
	leaq	8(%rsi,%rax,8), %r12

.Loop2:
	testl	%ebx, %ebx					# %ebx is end condition for k
	jle	.Loop2End						# If %ebx zero, end Loop2 iteration
	xorl	%esi, %esi					# Clear %esi to 0 (Set k = 0)
	xorl	%r11d, %r11d				# Clear %r11 to 0 (Set i = 0)
.Loop3:
	testl	%r9d, %r9d					# 
	jle	.Loop3End						# If %r9d zero, end Loop3 iteration
	leaq	0(%rbp,%rsi), %rcx			
	xorl	%eax, %eax					# Set %rax to 0 (Set j = 0)
	
# ---------- Main multiplication loop ----------
.Multiply:
	# %r8 --> i * colA
	# %rsi --> k
	# %r10 --> k * colB
	# %rax --> j
	# (%r8, %rsi) --> matA[i][k]
	# (%r10, %rax, 8) --> matB[k][j]
	
	# mat[i][j] += matA[i][k] * matB[k][j]

	movq	(%r8,%rsi), %rdx			# Move whatever at %r8 + %rsi to %rdx 
	imulq	(%r10,%rax,8), %rdx			# Multiply contents at %c10 + %rax*8 with %rdx and store at %rdx 
	addq	%rdx, (%rcx,%rax,8)			# Add %rdx at %rcx + 8*%rax
	addq	$1, %rax					# Add 1 to %rax
	cmpl	%eax, %r9d					# Check if %eax == %r9d
	jg	.Multiply						# If 3rd loop not ended, keep looping in 3rd Loop
# ---------- Main multiplication loop ----------

# j loop End (%rax(j) == %r9d(j condition))
# |
# |
# |
# v

.Loop3End:
	addl	$1, %r11d					# Add 1 to %r11d (Add 1 to i)
	addq	%rdi, %rsi					# Add %rdi to %rsi (Add %rdi to k)
	cmpl	%ebx, %r11d					# Check if i == %rbx
	jne	.Loop3							# If not end, loop again
.Loop2End:
	addq	$8, %r8						# Essentially move to the next address
	addq	%rdi, %r10
	cmpq	%r12, %r8
	jne	.Loop2
.Loop1End:
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
