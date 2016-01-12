;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: Hal
;File: SMPAsm64.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\SMPAsm64.asm
;Created by Nathaniel on 23/12/2015 at 22:06
;
;Description: SMP Startup routine for x86-64 Multiprocessing
;**********************************************************/
BITS 64
section .text

global SMP_StartupRoutine
SMP_StartupRoutine:
jmp $
cli
hlt

align 4
global gdt_addr
gdt_addr:
.len: dw 0
.base: dq 0

align 4
global idt_addr
idt_addr:
.len: dw 0
.base: dq 0

align 4
global cr3_val
cr3_val: dq 0

global cr4_val
cr4_val: dq 0

global mapped_lapic
mapped_lapic: dq 0

global kernel_sync
kernel_sync:
.flags: db 0
.cpu: db 0
.instruc: dw 0
.resv: dd 0

global kernel_sync_ptr
kernel_sync_ptr:
dq 0

align 4
temp_gdt_ent:
.len: dw 0x17
.base: dq temp_gdt

align 4
temp_gdt:
dq 0
dd 0x0000FFFF, 0x00CF9A00
dd 0x0000FFFF, 0x00CF9200

global SMP_StartupRoutine_End
SMP_StartupRoutine_End:
cli
hlt
ret