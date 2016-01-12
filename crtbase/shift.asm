BITS 32
section .text

;Parameters: 
;EDX:EAX -> Long value to shift
;CL -> number of bits to shift by
;RETURN -> EDX:EAX
global _allshl
_allshl:
push ebp
mov ebp, esp

cmp cl, 64
jge .zero

cmp cl, 32		;Intel maunal says undefined when greater than data size
jge .highbits

.shift_carry:
shld edx, eax, cl
shl eax, cl

jmp .end

.highbits:
sub cl, 32
shl eax, cl
mov edx, eax
xor eax, eax
jmp .end

.zero:
xor eax, eax
xor edx, edx
.end:
leave
ret

;SIGNED
global _allshr
_allshr:
push ebp
mov ebp, esp

cmp cl, 64
jge .zero

cmp cl, 32
jge .highbits

shrd eax, edx, cl
sar edx, cl

jmp .end
.highbits:
mov eax, edx
sar edx, 31		;Sign extend
sub cl, 32
sar eax, cl
jmp .end
.zero:
xor eax, eax
xor edx, edx
.end:
leave
ret

global _aullshr
_aullshr:
push ebp
mov ebp, esp

cmp cl, 64
jge .zero

cmp cl, 32
jge .highbits

shrd eax, edx, cl
shr edx, cl
jmp .end

.highbits:
sub cl, 32
shr edx, cl
mov eax, edx
xor edx, edx
jmp .end
.zero:
xor eax, eax
xor edx, edx
.end:
leave
ret
