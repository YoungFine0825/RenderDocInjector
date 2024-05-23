; Made by Sean P


.DATA

PUBLIC JUMPBACK_ADDRESS

JUMPBACK_ADDRESS DQ 1111111111111111h



.CODE

asm_code_example PROC
	pop rax

	;  User code would normally go here
	
	push rax
	mov rax, QWORD PTR [JUMPBACK_ADDRESS]
	jmp rax
asm_code_example ENDP


END
