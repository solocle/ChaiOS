BITS 32
segment .text

global _fpu_setup
_fpu_setup:
fninit
ret

global _load_taskreg
_load_taskreg:
mov ax, [esp+4]
ltr ax
ret

global _load_cs
_load_cs:
;Handily, CS is below EIP on the stack. retf will work. BUT: caller cleanup. so we need to handle that
pop eax	;Return address
pop ecx
push ecx	;The copy the caller will cleanup
push ecx
push eax
retf

global _load_ds
_load_ds:
mov eax, [esp+4]
mov ds, ax
mov es, ax
mov ss, ax
ret

global _interrupt
_interrupt:
mov al, [esp+4]
cmp al, 3
jne .normalint
int 3
.normalint:
mov [.actInt + 1], al
.actInt:
int 0xFF
ret

global _setjmp
_setjmp:
push ebp
mov ebp, esp
mov ecx, [ebp+8]	;Contains jmp_buf
;Callee-preserved registers
mov [ecx], ebx
mov [ecx+4], esi
mov [ecx+8], edi
mov edx, [ebp]
mov [ecx+12], edx		;EBP
mov edx, [ebp+4]		;Return address
add esp, 8
mov [ecx+16], esp
sub esp, 8
mov [ecx+20], edx
xor eax, eax
pop ebp
ret	;return

global __setjmp3
__setjmp3:
jmp _setjmp

global _longjmp
_longjmp:
mov eax, [esp+8] ;return val
mov ecx, [esp+4] ;Buffer
;Restore our registers
mov ebx, [ecx]
mov esi, [ecx+4]
mov edi, [ecx+8]
;Restore stack
mov ebp, [ecx+12]
mov esp, [ecx+16]
test eax, eax
jnz .next
inc eax
.next:
jmp [ecx+20]	;Return

