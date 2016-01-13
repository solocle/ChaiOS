BITS 32
section .text

global __dtoui3
__dtoui3:
;TODO: support 64 bit integers
cvttsd2si eax, xmm0
ret

global __ultod3
__ultod3:
;TODO: support 64 bit integers
cvtsi2sd xmm0, ecx
ret