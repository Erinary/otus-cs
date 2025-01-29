%include 'library.asm'

section .data
	arr dd 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1
	n dd 12

section .text
	global _start

_start:
	xor eax, eax		; max
	xor edx, edx		; count
	mov ecx, [n]		; i
	mov ebx, arr		; &array[i]

loop_start:
	cmp dword [ebx], 0
	je case0
	
	inc edx

	cmp eax, edx
	jge next
	
	mov eax, edx
	jmp next

case0:
	xor edx, edx
	jmp next
	
next:
	add ebx, 4
	loop loop_start
	
	call print_number
	call exit