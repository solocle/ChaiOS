BITS 32
section .text

global __ftol2_sse
__ftol2_sse:
;We can assume SSE2 support, ChaiOS requires this
push ebp
mov ebp, esp
;Create an 8-byte aligned space on the stack
sub esp, 8
and esp, 0xFFFFFFF8
fstp qword[esp]		;Store the operand to the stack
cvttsd2si eax, qword[esp]
leave
ret

global __ftol2
__ftol2:
jmp __ftol2_sse		;use the SSE2 implementation