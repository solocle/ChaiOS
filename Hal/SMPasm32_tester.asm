section .code
BITS 16

%define LOADED_BASE 0x1000

global _SMP_StartupRoutine
_SMP_StartupRoutine:
;Forcing size overrides
;Load GDT
mov eax, _gdt_addr
mov ebx, _SMP_StartupRoutine
sub eax, ebx
add eax, LOADED_BASE

mov ecx, .next
sub ecx, ebx
add ecx, LOADED_BASE

mov edx, .jump
sub edx, ebx
add edx, LOADED_BASE+1
mov [edx], cx

lgdt [eax]

mov eax, cr0
or eax, 1
mov cr0, eax

.jump:
jmp 0x8:0
.next:
BITS 32

jmp $
cli
hlt

global _gdt_addr
_gdt_addr:
.len: dw 0
.base: dd 0

global _cr3_val
_cr3_val: dd 0

global _SMP_StartupRoutine_End
_SMP_StartupRoutine_End:
cli
hlt
ret