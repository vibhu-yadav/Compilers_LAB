/*
Name : Vibhu
Roll No : 20CS10072
Compilers Lab => Assignment - 1
*/

/*
COMMONLY USED INSTRUCTIONS AND THEIR MEANING

	mov : move
	add : add
	sub : subtract
	mul : multiply
	div : divide
	cmp : compare
	lea : load effective address
	j : jump

SUFFIXES determine the data sizes involved

	b : byte (1 byte)
	w : word (2 bytes)
	l : long (4 bytes)
	q : quad (8 bytes)
*/

.file	"asgn1.c" # Source file that has been used to generate this file.
	
	.text	# This marks the beginning of text segment of code
	
	.section	.rodata # Marks the beginning of the read-only data segment
	.align 8 # Sets the alignment of the next section to (modulo) 8 bytes.

.LC0:	# address of string used in 1st printf, contains a read-only string
	.string	"Enter the string (all lower case): " 

.LC1: 	# address of string used in 1st scanf, contains a read-only string
	.string	"%s"

.LC2:	# address of string used in 2nd printf, contains a read-only string
	.string	"Length of the string: %d\n"
	.align 8

.LC3:	# address of string used in 3rd printf, contains a read-only string
	.string	"The string in descending order: %s\n"
	.text	# This marks the beginning of the text segment of code
	.globl	main	# declares main as a global name to the compiler
	.type	main, @function # specifies that "main" is a function

main:	# beginning of the main segment

.LFB0:	
# .L is used for "local" labels and LFB is function_begin_label for the beginning of the 1st local function (main)
	.cfi_startproc
	endbr64
	pushq	%rbp # Save the base pointer on the stack frame of the function
	.cfi_def_cfa_offset 16 
	.cfi_offset 6, -16
	movq	%rsp, %rbp	# rbp <-- rsp , set new stack base pointer
	.cfi_def_cfa_register 6
	subq	$80, %rsp	# allocate space of 80 bytes from stack for local variables of main 
	movq	%fs:40, %rax # Stack Canary Stuff
	movq	%rax, -8(%rbp)	# (rbp -8) <-- rax
	xorl	%eax, %eax	# set eax to 0 (a^a = 0 for all a), xor is used instead of mov as it is faster
						# setting eax to 0 signifies that we can safely execute further instructions

#  1st printf statement => printf("Enter the string (all lower case): [line 14 of source code]
	leaq	.LC0(%rip), %rax # load address of string in .LC0 into rax, %rip indicates use of relative address.
	movq	%rax, %rdi	# rdi <-- rax, rdi is the first argument to printf
	movl	$0, %eax	# set eax to 0
	call	printf@PLT	# call printf with rdi as first argument to print the LC0 string

# 1st scanf statement => scanf("%s", &str); [line 15 of source code]
	leaq	-64(%rbp), %rax # rax <-- (rbp - 64), this refers to the char array (str) which is to be used to store input string
	movq	%rax, %rsi # rsi <-- rax, rsi is the second argument to scanf, thus we pass the address of str to scanf
	leaq	.LC1(%rip), %rax # load address of string in .LC1 into rax, this is the "%s" string used to indicate the input being a string / char array.
	movq	%rax, %rdi # rdi <-- rax, rdi is the first argument to scanf "%s"
	movl	$0, %eax # eax <-- 0
	call	__isoc99_scanf@PLT # call scanf with rdi and rsi as arguments to scan the input string
							   # Thus, we do scanf("%s",str);

# Calling the length function => len = length(str); [line 17 of source code]
	leaq	-64(%rbp), %rax # rax <-- (rbp - 64), this refers to the char array (str) which is to be passed to length function
	movq	%rax, %rdi # rdi <-- rax, rdi is the first argument to length function
	call	length # call the length function with rdi as argument to get the length of the string
	movl	%eax, -68(%rbp) # (rbp - 68) <-- eax, this is the length of the string
							# eax is used to store return value, thus (rbp-68) is the location of the variable len in source code.							

#  2nd printf statement => printf("Length of the string: %d\n", len); [line 18 of source code]
	movl	-68(%rbp), %eax # eax <-- (rbp - 68), this refers to the len variable
	movl	%eax, %esi # esi <-- eax, esi is the second argument to printf
	leaq	.LC2(%rip), %rax # load address of string in .LC2 into rax, this is the first argument to printf 
	movq	%rax, %rdi # rdi <-- rax, rdi is the first argument to printf
	movl	$0, %eax # eax <-- 0
	call	printf@PLT	# call printf with rdi and esi as arguments to print the LC2 string
						  # Thus, we do printf("Length of the string: %d\n", len);

# Calling the sort function => sort(str, len, dest); [line 20 of source code]
	leaq	-32(%rbp), %rdx # rdx <-- (rbp - 32), this refers to the char array (dest) which is to be passed as the 3rd argument to sort function
	movl	-68(%rbp), %ecx # ecx <-- (rbp - 68), this refers to the len variable 
	leaq	-64(%rbp), %rax # rax <-- (rbp - 64), this refers to the char array (str) 
	movl	%ecx, %esi # esi <-- ecx, esi is the second argument to sort function (len)
	movq	%rax, %rdi # rdi <-- rax, rdi is the first argument to sort function (str)
	call	sort # Call the sort function with rdi, esi and rdx as arguments to sort the string
				# Thus, we do sort(str, len, dest);

# 3rd printf statement => printf("The string in descending order: %s\n", dest); [line 21 of source code]
	leaq	-32(%rbp), %rax # rax <-- (rbp - 32), this refers to the char array (dest)
	movq	%rax, %rsi # rsi <-- rax, rsi is the second argument to printf
	leaq	.LC3(%rip), %rax # load address of string in .LC3 into rax
	movq	%rax, %rdi # rdi <-- rax, rdi is the first argument to printf
	movl	$0, %eax # eax <-- 0
	call	printf@PLT # call printf with rdi and rsi as arguments to print the LC3 string
						# Thus, we do printf("The string in descending order: %s\n", dest);

	movl	$0, %eax # eax <-- 0
	movq	-8(%rbp), %rdx # Stack Canary Stuff
	subq	%fs:40, %rdx	
	je	.L3 # If value of previous instruction is 0, jump to .L3
	call	__stack_chk_fail@PLT	# Else call the stack_chk_fail function to signify some error

.L3:
	leave # leave the function, pop the return address of the caller from the callee stack and jump to it
	.cfi_def_cfa 7, 8
	ret # return from the main function
	.cfi_endproc

# LFE = function_end_label, thus this segment marks the end of the main function
.LFE0: 
	.size	main, .-main # .size directive tells the compiler the size of the main function.
						# size of main = .(current address of pointer) - main (address of starting of main function)
	.globl	length # declare the name length in the global scope
	.type	length, @function # specifiy that the name length refers to a function

# int length(char str[20]);
length:
.LFB1: # function_begin_label, thus this segment marks the beginning of the length function
	.cfi_startproc
	endbr64
	pushq	%rbp # push the base pointer onto the stack
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp # set the base pointer to the stack pointer
	.cfi_def_cfa_register 6 
	movq	%rdi, -24(%rbp) # (rbp - 24) <-- rdi, this is the first argument to length function (str)
	movl	$0, -4(%rbp) # (rbp - 4) <-- 0, this refers to the variable i inside length function
	jmp	.L5 # jump to .L5

.L6:
	addl	$1, -4(%rbp) # increment value of i by 1

.L5:
	movl	-4(%rbp), %eax # eax <-- (rbp - 4), this refers to the variable i inside length function
	movslq	%eax, %rdx # rdx <- eax while transforming from 32 bit to 64 bit and preserving the value & sing. [long to quad]
	movq	-24(%rbp), %rax # rax <-- (rbp - 24), this refers to the beginning of (str) array
	addq	%rdx, %rax # rax <-- rax + rdx, as rdx = i, rax goes from str[0] to str[i]
	movzbl	(%rax), %eax # eax <-- (rax), move value at rax[0] (which is 1 byte) to eax, extending byte to long 
	testb	%al, %al # al is used to check LSB of eax
					# testb is Bitwise AND of al with itself, so if al is 0 (i.e null '\0'), then testb = 0,
					# testb = 1 otherwise
	jne	.L6 # if testb = 1 (or str[i] != '\0'), jump to .L6
	
	movl	-4(%rbp), %eax # If str[i] == '\0', then eax <-- (rbp - 4), this refers to the variable i inside length function
							# This value of i will be the return value of the length function
	popq	%rbp  # restore the base pointer from the stack
	.cfi_def_cfa 7, 8
	ret # return from the length function
	.cfi_endproc

# LFE = function_end_label, thus this segment marks the end of the length function
.LFE1:
	.size	length, .-length # .size directive tells the compiler the size of the length function
							# size of length = .(current address of pointer) - length (address of starting of length function)
	.globl	sort # declare the name sort in the global scope
	.type	sort, @function # specifiy that the name sort refers to a function


# void sort(char str[20], int len, char dest[20]);
sort:
# LFB = function_begin_label, thus this segment marks the beginning of the sort function
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp # push the base pointer onto the stack
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp # set the base pointer to the stack pointer
	.cfi_def_cfa_register 6
	subq	$48, %rsp # allocate space of 48 bytes in the stack for local variables of sort function
	movq	%rdi, -24(%rbp) # (rbp - 24) <-- rdi, this is the first argument to sort function (str)
	movl	%esi, -28(%rbp) # (rbp - 28) <-- esi, this is the second argument to sort function (len)
	movq	%rdx, -40(%rbp) # (rbp - 40) <-- rdx, this is the third argument to sort function (dest)
	movl	$0, -8(%rbp) # (rbp - 8) <-- 0, this refers to the variable i inside sort function, initialized to 0
	jmp	.L9 # jump to .L9

.L13:
	movl	$0, -4(%rbp) # (rbp - 4) <-- 0, this refers to the variable j inside sort function, initialized to 0
	jmp	.L10 # jump to .L10

.L12:
	movl	-8(%rbp), %eax # eax <-- (rbp - 8), this refers to the variable i inside sort function
	movslq	%eax, %rdx # rdx <- eax while transforming from 32 bit to 64 bit and preserving the value & sing. [long to quad]. %rdx = i 
	movq	-24(%rbp), %rax # rax <-- (rbp - 24), this refers to the beginning of (str) array, str[0]. %rax = str[0]
	addq	%rdx, %rax # rax <-- rax + rdx, as rdx = i, rax goes from str[0] to str[i]. %rax = str[i]
	movzbl	(%rax), %edx # edx <-- (rax), move value at rax[0] (which is 1 byte) to edx, extending byte to long. %edx = str[i]
	movl	-4(%rbp), %eax # eax <-- (rbp - 4), this refers to the variable j inside sort function. %eax = j
	movslq	%eax, %rcx # rcx <- eax while transforming from 32 bit to 64 bit and preserving the value & sign. [long to quad]. %rcx = j
	movq	-24(%rbp), %rax # rax <-- (rbp - 24), this refers to the beginning of (str) array, str[0]
	addq	%rcx, %rax # rax <-- rax + rcx, as rcx = j, rax goes from str[0] to str[j]. %rax = str[j]
	movzbl	(%rax), %eax # eax <-- rax . Now, %eax = str[j]
	cmpb	%al, %dl # compare value of str[i](%al) and str[j] (%dl)
	jge	.L11 # If str[i] >= str[j], jump to .L11

	# If str[i] < str[j], then swap str[i] and str[j] as follows
	movl	-8(%rbp), %eax # eax <-- (rbp - 8), this refers to the variable i inside sort function 
	movslq	%eax, %rdx # rdx <-- eax while transforming from 32 bit to 64 bit and preserving the value & sign. [long to quad]. %rdx = i
	movq	-24(%rbp), %rax # rax <-- (rbp - 24), this refers to the beginning of (str) array, str[0]
	addq	%rdx, %rax # rax <-- rax + rdx, as rdx = i, rax goes from str[0] to str[i]
	movzbl	(%rax), %eax # eax <-- (rax). %eax = str[i]
	movb	%al, -9(%rbp) # (rbp - 9) <-- %al, this refers to the char variable temp inside sort function, initialized to str[i]

	movl	-4(%rbp), %eax # eax <-- (rbp - 4), this refers to the variable j inside sort function
	movslq	%eax, %rdx # rdx <-- eax while transforming from 32 bit to 64 bit and preserving the value & sign. [long to quad]. %rdx = j
	movq	-24(%rbp), %rax # rax <-- (rbp - 24), this refers to the beginning of (str) array, str[0]
	addq	%rdx, %rax # rax <-- rax + rdx, as rdx = j, rax goes from str[0] to str[j]
	
	movl	-8(%rbp), %edx # edx <-- (rbp - 8), this refers to the variable i inside sort function
	movslq	%edx, %rcx # rcx <-- edx while transforming from 32 bit to 64 bit and preserving the value & sign. [long to quad]. %rcx = i
	movq	-24(%rbp), %rdx # rdx <-- (rbp - 24), this refers to the beginning of (str) array, str[0]
	addq	%rcx, %rdx # rdx <-- rdx + rcx, as rcx = i, rdx goes from str[0] to str[i]

	movzbl	(%rax), %eax # eax <-- (rax). %eax = str[i]
	movb	%al, (%rdx) # (rdx) <-- %al, this refers to the char variable temp inside sort function, initialized to str[j]

	movl	-4(%rbp), %eax # eax <-- (rbp - 4), this refers to the variable j inside sort function
	movslq	%eax, %rdx # rdx <-- eax while transforming from 32 bit to 64 bit and preserving the value & sign. [long to quad]. %rdx = j
	movq	-24(%rbp), %rax # rax <-- (rbp - 24), this refers to the beginning of (str) array, str[0]
	addq	%rax, %rdx # rdx <-- rdx + rax, as rax = str[0], rdx goes from str[0] to str[j]
	movzbl	-9(%rbp), %eax # eax <-- (rbp - 9), this refers to the char variable temp inside sort function, initialized to str[i]
	movb	%al, (%rdx) # (rdx) <-- %al, this refers to the char variable temp inside sort function, initialized to str[j]

.L11:
	addl	$1, -4(%rbp) # increment j, j <-- j+1

.L10:
	movl	-4(%rbp), %eax # eax <-- (rbp - 4), this refers to the variable j inside sort function
	cmpl	-28(%rbp), %eax # compare (rbp - 28) and (rbp - 4), i.e, check if (j < len)
	jl	.L12 # If j < len, jump to .L12
	addl	$1, -8(%rbp) # Else, end the inner loop and increment i (rbp -8) by 1, then continue with instructions in L9

.L9:
	movl	-8(%rbp), %eax # eax <-- (rbp - 8), this refers to the variable i inside sort function
	cmpl	-28(%rbp), %eax # compare (rbp - 28) and (rbp - 8), i.e, check if (i < len)
	jl	.L13 # If i < len, jump to .L13

	# Else, continue with instructions below
	movq	-40(%rbp), %rdx # rdx <-- (rbp - 40), this is address of (dest) in sort
	movl	-28(%rbp), %ecx # ecx <-- (rbp - 28), this is (len) variable in sort
	movq	-24(%rbp), %rax # rax <-- (rbp - 24), this is address of (str) in sort
	
	movl	%ecx, %esi # esi <-- ecx, this is the length of the array
	movq	%rax, %rdi # rdi <-- rax, this is the address of the (str) array 
	call	reverse # call reverse function with 3 arguments (rdi => str, rsi => len, rdx => dest)

	nop	# no operation

	leave  # leave the function, pop the return address of the caller from the callee stack and jump to it
	.cfi_def_cfa 7, 8

	ret # return from the function
	.cfi_endproc

# LFE = function_end_label, this is the end of the function sort
.LFE2:
	.size	sort, .-sort # .size sort, the size of the function sort
						# size of length = .(current address of pointer) - length (address of starting of length function)
	.globl	reverse	# declare the name reverse in the global scope
	.type	reverse, @function # specifiy that reverse is a function

# void reverse(char str[20], int len, char dest[20])
reverse:
# LFB = function_begin_label, thus this segment marks the beginning of the reverse function
.LFB3:
	.cfi_startproc
	endbr64
	
	pushq	%rbp # Save the base pointer on the stack frame of the function
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp # rbp <-- rsp , set new stack base pointer
	.cfi_def_cfa_register 6

	movq	%rdi, -24(%rbp) # (rbp - 24) <-- %rdi, this refers to the beginning of (str) array, str[0]
	movl	%esi, -28(%rbp)	# (rbp - 28) <-- %esi, this refers to the length of the array (len)
	movq	%rdx, -40(%rbp) # (rbp - 40) <-- %rdx, this refers to the beginning of (dest) array, dest[0]
	movl	$0, -8(%rbp) # (rbp - 8) <-- $0, this refers to the variable i inside reverse function, initialized to 0
	jmp	.L15	# jump to .L15

.L20:
	movl	-28(%rbp), %eax # eax <-- (rbp - 28), this is len variable
	subl	-8(%rbp), %eax # eax <-- eax - (rbp - 8). Thus, %eax = len - i
	subl	$1, %eax # eax <-- eax - 1, decrement i by 1. $eax = len -i -1
	movl	%eax, -4(%rbp) # (rbp -4) is variable j, thus j = (len -i - 1).

	nop # no operation

	movl	-28(%rbp), %eax # eax <-- (rbp - 28), this is len variable
	movl	%eax, %edx # edx <-- eax, this is len variable
	shrl	$31, %edx # edx <-- edx >> 31. 31 right shifts remove all the bits from number except for MSB (sign bit)
	addl	%edx, %eax # eax <-- eax + edx
	sarl	%eax # Right shift eax once, i.e, eax <-- eax/2
	cmpl	%eax, -4(%rbp) # compare (rbp - 4) and eax, i.e, check if (j < len/2)
	jl	.L18	# If yes, jump to .L18

	# Else, continue with instructions below
	movl	-8(%rbp), %eax # eax <-- (rbp - 8), this is variable i inside reverse function
	cmpl	-4(%rbp), %eax # compare (rbp - 4) and eax, i.e, check if (i == j)
	je	.L23 # If yes, jump to .L23

	# Else if (i != j), swap str[i] and str[j] as follows:

	# temp = str[i], [line 66 of source code]
	movl	-8(%rbp), %eax # eax <-- (rbp - 8). Thus, %eax = i
	movslq	%eax, %rdx # rdx <-- eax. %rdx = i
	movq	-24(%rbp), %rax  # rax <-- (rbp - 24). Thus, %rax = str[0]
	addq	%rdx, %rax # rax <-- rax + rdx. rax = str[i]
	movzbl	(%rax), %eax # eax <-- (str[i]), this is the character at index i
	movb	%al, -9(%rbp) # (rbp -9) <-- al, temp = str[i]

	# str[i] = str[j], [line 67 of source code]
	movl	-4(%rbp), %eax # eax <-- (rbp - 4). Thus, %eax = j
	movslq	%eax, %rdx # rdx <-- eax. %rdx = j
	movq	-24(%rbp), %rax # rax <-- (rbp - 24). Thus, %rax = str[0]
	addq	%rdx, %rax # rax <-- rax + rdx. rax = str[j]
	movl	-8(%rbp), %edx # edx <-- (rbp - 8). Thus, %edx = i
	movslq	%edx, %rcx # rcx <-- edx. %rcx = i
	movq	-24(%rbp), %rdx # rdx <-- (rbp - 24). Thus, %rdx = str[0]
	addq	%rcx, %rdx # rdx <-- rdx + rcx. rdx = str[i]
	movzbl	(%rax), %eax # eax <-- (str[j]), this is the character at index j
	movb	%al, (%rdx)	# rdx <-- eax (%al is single char), str[i] = str[j]

	# str[j] = temp, [line 68 of source code]
	movl	-4(%rbp), %eax # eax <-- (rbp - 4). Thus, %eax = j
	movslq	%eax, %rdx # rdx <-- eax. %rdx = j
	movq	-24(%rbp), %rax # rax <-- (rbp - 24). Thus, %rax = str[0]
	addq	%rax, %rdx # rdx <-- rax + rdx. rdx = str[j]
	movzbl	-9(%rbp), %eax # eax <-- (rbp - 9). Thus, %eax = temp
	movb	%al, (%rdx) # rdx <-- eax, str[j] = temp


	jmp	.L18 # jump to .L18

.L23:
	nop # no operation

.L18:
	addl	$1, -8(%rbp) # (rbp-8) <-- (rbp-8)+1. increment i by 1

.L15:
	movl	-28(%rbp), %eax # eax <-- (rbp - 28), this is len variable
	movl	%eax, %edx # edx <-- eax . %edx = len
	shrl	$31, %edx # edx <-- edx >> 31. 31 right shifts remove all the bits from number except for MSB (sign bit)
	addl	%edx, %eax	# eax <-- eax + edx
	sarl	%eax # Right shift eax once, i.e, eax <-- eax/2 (len /2)
	cmpl	%eax, -8(%rbp) # compare (rbp - 8) and eax, i.e, check if (i < len/2)

	jl	.L20 # If (i < len/2), jump to .L20
	movl	$0, -8(%rbp) # Else, (rbp - 8) <-- 0 . i = 0

	jmp	.L21 # jump to .L21

# Inside the for loop for copying str onto dest [line 75 of source code] 
# dest[i]=str[i];
.L22:

	movl	-8(%rbp), %eax # eax <-- (rbp - 8). Thus, %eax = i
	movslq	%eax, %rdx # rdx <-- eax. %rdx = i
	movq	-24(%rbp), %rax # rax <-- (rbp - 24). Thus, %rax = str[0]
	addq	%rdx, %rax # rax <-- rax + rdx. rax = str[i]

	movl	-8(%rbp), %edx # edx <-- (rbp - 8). Thus, %edx = i
	movslq	%edx, %rcx # rcx <-- edx. %rcx = i
	movq	-40(%rbp), %rdx # rdx <-- (rbp - 40). Thus, %rdx = dist[0]
	addq	%rcx, %rdx # rdx <-- rdx + rcx. rdx = dist[i]
	movzbl	(%rax), %eax # eax <-- (str[i]), this is the character at index i
	movb	%al, (%rdx) # rdx <-- eax (%al is single character from eax), dist[i] = str[i]
	addl	$1, -8(%rbp) # (rbp-8) <-- (rbp-8)+1. increment i by 1

# Condition checking and the for loop [line 74 of source code] 
# for(i=0;i<len;i++)
.L21:
	
	movl	-8(%rbp), %eax # eax <-- (rbp - 8). Thus, %eax = i
	cmpl	-28(%rbp), %eax # compare (rbp - 28) and (rbp - 8), i.e, (i < len)
	
	jl	.L22 # If (i < len), jump to .L22
	
	nop # no operation
	nop # no operation

	popq	%rbp # Save the base pointer on the stack frame of the function
	.cfi_def_cfa 7, 8

	ret # return from the function reverse
	.cfi_endproc

# LFE = function_end_label, this is the end of the function reverse
.LFE3:
	.size	reverse, .-reverse # .size directive tells the compiler the size of the reverse function.
						# size of reverse = .(current address of pointer) - (address of starting of reverse function)
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
