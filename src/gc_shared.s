	.file	"gc_shared.c"
	.comm	stack_top,8,8
	.comm	stack_bottom,8,8
	.comm	BBSstart,8,8
	.comm	BBSend,8,8
	.text
	.globl	block_get_size
	.type	block_get_size, @function
block_get_size:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_has_forward
	testl	%eax, %eax
	je	.L2
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_forward
	movq	%rax, %rdi
	call	block_get_size
	jmp	.L3
.L2:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_info
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_is_array
	testl	%eax, %eax
	je	.L4
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_array_size
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	imulq	%rdx, %rax
	movq	%rax, %rdi
	call	align_size
	addq	$16, %rax
	jmp	.L3
.L4:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	align_size
	addq	$8, %rax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	block_get_size, .-block_get_size
	.globl	block_get_forward
	.type	block_get_forward, @function
block_get_forward:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_type
	cmpq	$1, %rax
	jne	.L6
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	jmp	.L7
.L6:
	movl	$0, %eax
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	block_get_forward, .-block_get_forward
	.globl	block_get_type
	.type	block_get_type, @function
block_get_type:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movabsq	$9223372036854775807, %rax
	andq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	block_get_type, .-block_get_type
	.globl	block_get_array_size
	.type	block_get_array_size, @function
block_get_array_size:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_has_forward
	testl	%eax, %eax
	je	.L11
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_forward
	movq	%rax, %rdi
	call	block_get_array_size
	jmp	.L12
.L11:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	block_get_array_size, .-block_get_array_size
	.globl	block_get_info
	.type	block_get_info, @function
block_get_info:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_has_forward
	testl	%eax, %eax
	je	.L14
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_forward
	movq	%rax, %rdi
	call	block_get_info
	jmp	.L15
.L14:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_type
	movq	%rax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	$type_table, %rax
.L15:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	block_get_info, .-block_get_info
	.globl	block_is_array
	.type	block_is_array, @function
block_is_array:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_has_forward
	testl	%eax, %eax
	je	.L17
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_forward
	movq	%rax, %rdi
	call	block_is_array
	jmp	.L18
.L17:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	shrq	$63, %rax
	movzbl	%al, %eax
.L18:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	block_is_array, .-block_is_array
	.section	.rodata
	.align 8
.LC0:
	.string	"error: Call block_set_size(NULL, %u)"
	.align 8
.LC1:
	.string	"Error: Obsolete function block_set_size called"
	.text
	.globl	block_set_size
	.type	block_set_size, @function
block_set_size:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L20
	movq	-16(%rbp), %rax
	movl	%eax, %edx
	movl	$.LC0, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
	jmp	.L21
.L20:
	movl	$.LC1, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
.L21:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	block_set_size, .-block_set_size
	.section	.rodata
	.align 8
.LC2:
	.string	"error: Call block_set_forward(NULL, %p)"
	.text
	.globl	block_set_forward
	.type	block_set_forward, @function
block_set_forward:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L23
	movq	-16(%rbp), %rax
	movq	%rax, %rdx
	movl	$.LC2, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
	jmp	.L24
.L23:
	movq	-8(%rbp), %rax
	movq	$1, (%rax)
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 8(%rax)
	movl	$0, %eax
.L24:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	block_set_forward, .-block_set_forward
	.section	.rodata
	.align 8
.LC3:
	.string	"error: Call block_set_type(NULL, %x)"
	.text
	.globl	block_set_type
	.type	block_set_type, @function
block_set_type:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L26
	movq	-16(%rbp), %rax
	movl	%eax, %edx
	movl	$.LC3, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
	jmp	.L27
.L26:
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movabsq	$-9223372036854775808, %rax
	andq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movabsq	$9223372036854775807, %rax
	andq	-16(%rbp), %rax
	orq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movl	$0, %eax
.L27:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	block_set_type, .-block_set_type
	.section	.rodata
	.align 8
.LC4:
	.string	"error: Call block_set_array_size(NULL, %u)"
	.text
	.globl	block_set_array_size
	.type	block_set_array_size, @function
block_set_array_size:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L29
	movq	-16(%rbp), %rax
	movl	%eax, %edx
	movl	$.LC4, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
	jmp	.L30
.L29:
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movl	$0, %eax
.L30:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	block_set_array_size, .-block_set_array_size
	.section	.rodata
	.align 8
.LC5:
	.string	"error: Call block_set_info(NULL, %p)"
	.align 8
.LC6:
	.string	"Error: Obsolete function block_set_info used"
	.text
	.globl	block_set_info
	.type	block_set_info, @function
block_set_info:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L32
	movq	-16(%rbp), %rax
	movq	%rax, %rdx
	movl	$.LC5, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
	jmp	.L33
.L32:
	movl	$.LC6, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
.L33:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	block_set_info, .-block_set_info
	.section	.rodata
	.align 8
.LC7:
	.string	"error: Call block_set_is_array(NULL, %d)"
	.text
	.globl	block_set_is_array
	.type	block_set_is_array, @function
block_set_is_array:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L35
	movl	-12(%rbp), %eax
	movl	%eax, %edx
	movl	$.LC7, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
	jmp	.L36
.L35:
	cmpl	$0, -12(%rbp)
	je	.L37
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movabsq	$-9223372036854775808, %rax
	orq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	jmp	.L38
.L37:
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movabsq	$9223372036854775807, %rax
	andq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
.L38:
	movl	$0, %eax
.L36:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	block_set_is_array, .-block_set_is_array
	.globl	block_has_forward
	.type	block_has_forward, @function
block_has_forward:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movabsq	$9223372036854775807, %rax
	andq	%rdx, %rax
	cmpq	$1, %rax
	sete	%al
	movzbl	%al, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	block_has_forward, .-block_has_forward
	.globl	copy_block_metadata
	.type	copy_block_metadata, @function
copy_block_metadata:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_type
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	block_set_type
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_is_array
	testl	%eax, %eax
	je	.L42
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_array_size
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	block_set_array_size
	movq	-16(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	block_set_is_array
	jmp	.L43
.L42:
	movq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	block_set_is_array
.L43:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	copy_block_metadata, .-copy_block_metadata
	.globl	block_is_struct_block
	.type	block_is_struct_block, @function
block_is_struct_block:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_type
	movl	%eax, -4(%rbp)
	cmpl	$0, -4(%rbp)
	je	.L45
	cmpl	$2, -4(%rbp)
	je	.L45
	cmpl	$4, -4(%rbp)
	je	.L45
	cmpl	$5, -4(%rbp)
	je	.L45
	movl	$1, %eax
	jmp	.L46
.L45:
	movl	$0, %eax
.L46:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	block_is_struct_block, .-block_is_struct_block
	.globl	next_block
	.type	next_block, @function
next_block:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_get_size
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	next_block, .-next_block
	.globl	next_nth_block
	.type	next_nth_block, @function
next_nth_block:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$1, -16(%rbp)
	je	.L51
	cmpq	$0, -16(%rbp)
	jne	.L52
.L51:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	next_block
	jmp	.L53
.L52:
	movq	-16(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	next_nth_block
	movq	%rax, %rdi
	call	next_block
.L53:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	next_nth_block, .-next_nth_block
	.globl	get_memory_primitive
	.type	get_memory_primitive, @function
get_memory_primitive:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	get_memory_primitive, .-get_memory_primitive
	.globl	release_memory_primitive
	.type	release_memory_primitive, @function
release_memory_primitive:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	release_memory_primitive, .-release_memory_primitive
	.globl	get_data_start
	.type	get_data_start, @function
get_data_start:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	block_is_array
	testl	%eax, %eax
	je	.L59
	movq	-8(%rbp), %rax
	addq	$16, %rax
	jmp	.L60
.L59:
	movq	-8(%rbp), %rax
	addq	$8, %rax
.L60:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	get_data_start, .-get_data_start
	.globl	get_data_end
	.type	get_data_end, @function
get_data_end:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	next_block
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	get_data_end, .-get_data_end
	.globl	align_size
	.type	align_size, @function
align_size:
.LFB23:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	andl	$7, %eax
	testq	%rax, %rax
	jne	.L64
	movq	-24(%rbp), %rax
	jmp	.L65
.L64:
	movq	-24(%rbp), %rax
	shrq	$3, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	addq	$1, %rax
	salq	$3, %rax
.L65:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	align_size, .-align_size
	.globl	init_block_from_chunk
	.type	init_block_from_chunk, @function
init_block_from_chunk:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	block_set_is_array
	movq	-8(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	block_set_type
	movq	-32(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	block_set_array_size
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	init_block_from_chunk, .-init_block_from_chunk
	.section	.rodata
	.align 8
.LC8:
	.string	"error: split_block called with non-raw block: split_block(%p, %u)"
	.text
	.globl	split_block
	.type	split_block, @function
split_block:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -56(%rbp)
	movq	%rsi, -64(%rbp)
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	block_get_type
	testq	%rax, %rax
	je	.L69
	movq	-64(%rbp), %rax
	movl	%eax, %edx
	movq	-56(%rbp), %rax
	movl	%edx, %ecx
	movq	%rax, %rdx
	movl	$.LC8, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	gc_log
	movl	$0, %eax
	jmp	.L70
.L69:
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	align_size
	movq	%rax, -48(%rbp)
	cmpq	$0, -56(%rbp)
	je	.L71
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L72
.L71:
	movl	$0, %eax
	jmp	.L70
.L72:
	movq	-48(%rbp), %rax
	leaq	16(%rax), %rbx
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	block_get_size
	subq	$16, %rax
	cmpq	%rax, %rbx
	jb	.L73
	movl	$0, %eax
	jmp	.L70
.L73:
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	block_get_size
	subq	-48(%rbp), %rax
	subq	$32, %rax
	movq	%rax, -40(%rbp)
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	addq	$16, %rax
	movq	%rax, -32(%rbp)
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -24(%rbp)
	movq	-56(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-32(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	block_set_is_array
	movq	-32(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	block_set_type
	movq	-40(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	block_set_array_size
	movq	-24(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	block_set_is_array
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	block_set_type
	movq	-48(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	block_set_array_size
	movq	-24(%rbp), %rax
.L70:
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	split_block, .-split_block
	.globl	is_pointer_to
	.type	is_pointer_to, @function
is_pointer_to:
.LFB26:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	get_data_start
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	get_data_end
	movq	%rax, -8(%rbp)
	movq	-32(%rbp), %rax
	cmpq	-16(%rbp), %rax
	jb	.L75
	movq	-32(%rbp), %rax
	cmpq	-8(%rbp), %rax
	jnb	.L75
	movl	$1, %eax
	jmp	.L76
.L75:
	movl	$0, %eax
.L76:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE26:
	.size	is_pointer_to, .-is_pointer_to
	.globl	get_ptr_to_slot
	.type	get_ptr_to_slot, @function
get_ptr_to_slot:
.LFB27:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE27:
	.size	get_ptr_to_slot, .-get_ptr_to_slot
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
