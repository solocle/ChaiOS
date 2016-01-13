BITS 32
section .text

;Parameters on the stack. Order does not matter
;Returns in EDX:EAX
global __allmul
__allmul:
push ebp
mov ebp, esp

mov eax, [ebp+12]		;High DWORD
mov ecx, [ebp+20]		;High DWORD

;If both high DWORDs are zero, we multiply the low 32 bits only
or eax, ecx
cmp eax, 0
mov eax, [ebp+8]		;Low DWORD
mov ecx, [ebp+16]		;Low DWORD
je .lowmultiply

;Full 64 bit multiply. We first do the low DWORDs, then the High DWORDs. with the Low DWORDs (note high*high >2^64)
;iAPX 186/188 manual says that CF and OF are set if the high DWORD (EDX) is set on MUL
;Therefore, we just return after truncating the high 32 bits.
mul ecx
;Non-volatile registers
push esi
push edi

;Store the result of low multiply for later
mov esi, eax	;Low
mov edi, edx	;High

mov eax, [ebp+12]	;High multiply
mov ecx, [ebp+16]	;Low DWORD
mul ecx
add edi, eax		;"Low" of result is actually high

mov eax, [ebp+8]	;Low bits
mov ecx, [ebp+20]	;high DWORD
mul ecx

add edi, eax

;The final result
mov eax, esi
mov edx, edi

pop edi
pop esi

jmp .end

.lowmultiply:
;x86 stores result in EDX:EAX, as required.
mul ecx

.end:
leave
ret
