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

%define LOADED_BASE 0
%define ACTUAL_BASE 0x1000

extern _SMP_CPU_entry

extern _thread_exit

extern _thread_eoi_helper

global _SMP_StartupRoutine
_SMP_StartupRoutine:
cli
mov ax, cs
mov ds, ax
mov es, ax
mov ss, ax
;Forcing size overrides
;Base of this code module
db 0x66		;Size override
mov ebx, _SMP_StartupRoutine

db 0x66
mov ecx, .flat
db 0x66
sub ecx, ebx
db 0x66
add ecx, ACTUAL_BASE

db 0x66
mov edx, .jump_flat
db 0x66
sub edx, ebx
db 0x66
add edx, LOADED_BASE+1

db 0x67
mov [edx], ecx

;Jump from segmented to flat
.jump_flat:
db 0xea
dw 0x0, 0x0
.flat:
mov ax, cs
mov ds, ax
mov es, ax
mov ss, ax
;Load GDT address
db 0x66		;Size override
mov eax, temp_gdt_ent
;Fixup GDT address
db 0x66		;Size override
sub eax, ebx
db 0x66		;Size override
add eax, ACTUAL_BASE

;Fixup address of GDT
db 0x66
mov ecx, temp_gdt
db 0x66
sub ecx, ebx
db 0x66
add ecx, ACTUAL_BASE
db 0x66, 0x67
mov [eax+2], ecx

;Load the GDT
.lgdt:
db 0x67
lgdt [eax]

;Setup address of next
db 0x66
mov ecx, .next
db 0x66
sub ecx, ebx
db 0x66
add ecx, ACTUAL_BASE

;Setup address of jump to PMODE
db 0x66
mov edx, .jump
db 0x66
sub edx, ebx
db 0x66
add edx, ACTUAL_BASE+1

;Make jump to pmode
db 0x67
mov [edx], ecx

;Setup CR3
db 0x66
mov edx, _cr3_val
db 0x66
sub edx, ebx
db 0x66
add edx, ACTUAL_BASE
db 0x66, 0x67
mov ecx, [edx]
mov cr3, ecx

;Setup CR0
mov eax, cr0
db 0x66
or eax, 0x80000001

mov cr0, eax

.jump:
db 0xea
dw 0x0, 0x8
.next:
BITS 32
mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
lgdt [_gdt_addr]		;Now we're 32 bit, we can go into the kernel address space
mov eax, .in_kernel_addr
jmp eax
.in_kernel_addr:
;Setup IDT
lidt [_idt_addr]
;Setup CR4
mov eax, [_cr4_val]
mov cr4, eax
;Find the local APIC ID
mov eax, [_mapped_lapic]
mov ecx, [eax+0x20]	;LAPIC ID Register
shr ecx, 24			;This puts the ID (which is in high bits), to the value

;Now we synchronise with the kernel
.syncloop:
mov eax, [_kernel_sync]
cmp eax, 0
je .syncloop
test eax, 1
jz .syncloop
.cpu_test:
mov ebx, eax
shr ebx, 8
and ebx, 0xff
cmp ebx, ecx
jnz .syncloop
;OK, this is us. We have an instruction
mov ebx, eax
shr ebx, 16
;and ebx, 0xFFFF	;Not neccessary
;EBX now contains the instruction
cmp ebx, 1
jne .syncloop
mov eax, 2
mov [_kernel_sync], eax
;Get some initialisation data
.dataloop:
mov eax, [_kernel_sync_ptr]
cmp eax, 0
je .dataloop
mov ebx, [eax]
mov esp, ebx
mov DWORD[_kernel_sync_ptr], 0

;More CPU setup
mov eax, cr0
or eax, 0x22
and eax, 0xFFFFFFF3
mov cr0, eax

;Enable our APIC (just to be safe)
mov ecx, 0x1B
rdmsr
or eax, 0x800
wrmsr
;APIC Surious interrupt register
mov eax, [_mapped_lapic]
mov ecx, 0xF0 | 0x100
mov [eax+0xF0], ecx

;APIC timer
mov ecx, 0x28 | 0x20000
mov [eax+0x320], ecx
mov ecx, 0b1010		;Divide by 128
mov [eax+0x3E0], ecx
mov ecx, 10
mov [eax+0x380], ecx

;Enable interrupts
sti

call _SMP_CPU_entry

cli
hlt

align 4
global _gdt_addr
_gdt_addr:
.len: dw 0
.base: dd 0

align 4
global _idt_addr
_idt_addr:
.len: dw 0
.base: dd 0

align 4
global _cr3_val
_cr3_val: dd 0

global _cr4_val
_cr4_val: dd 0

global _mapped_lapic
_mapped_lapic: dd 0

global _kernel_sync
_kernel_sync:
.flags: db 0
.cpu: db 0
.instruc: dw 0

global _kernel_sync_ptr
_kernel_sync_ptr:
dd 0

align 4
temp_gdt_ent:
.len: dw 0x17
.base: dd temp_gdt

align 4
temp_gdt:
dq 0
dd 0x0000FFFF, 0x00CF9A00
dd 0x0000FFFF, 0x00CF9200

global _SMP_StartupRoutine_End
_SMP_StartupRoutine_End:
cli
hlt
ret

global _scheduler_start_thread
_scheduler_start_thread:
push ebp		;Simulate stack frame with return address
push ebp
mov ebp, esp
mov eax, [ebp+24]
mov ebx, [ebp+20]
mov ecx, [ebp+16]
and ecx, 0xFFFF
mov edx, [ebp+12]	;Stack
mov gs, ax		;TLS
mov es, bx
mov ds, bx
mov eax, [ebp+8]

;Build a return address. 
mov ebx, .end
push ebx

;Build a iret frame to call the stack
cmp cx, 0x8		;Kernel?
je .no_stack_iret
push ds		;SS
push edx	;ESP
.no_stack_iret:
pushfd
pop ebx
or ebx, 1<<9	;IF
push ebx
push ecx	;CS
push eax	;EIP

call _thread_eoi_helper

iret

.end:
call _thread_exit
jmp $
pop ebp
ret