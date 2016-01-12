;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: Hal
;File: SMPasm32.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\SMPasm32.asm
;Created by Nathaniel on 23/12/2015 at 21:42
;
;Description: SMP Startup routine for x86 Multiprocessing
;**********************************************************/
BITS 32
section .text

%define LOADED_BASE 0x1000

global _SMP_StartupRoutine
_SMP_StartupRoutine:
;Forcing size overrides
;Load GDT
db 0x66		;Size override
mov eax, _gdt_addr
db 0x66		;Size override
mov ebx, _SMP_StartupRoutine
db 0x66		;Size override
sub eax, ebx
db 0x66		;Size override
add eax, LOADED_BASE


db 0x66
mov ecx, .next
db 0x66
sub ecx, ebx
db 0x66
add ecx, LOADED_BASE

db 0x66
mov edx, .jump
db 0x66
sub edx, ebx
db 0x66
add edx, LOADED_BASE+1

db 0x67
mov [edx], ecx


db 0x67
lgdt [0]

mov eax, cr0
db 0x66
or eax, 1

mov cr0, eax

.jump:
db 0xea
dw 0x0, 0x8
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