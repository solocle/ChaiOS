;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: ChaiOS
;File: x64bootstrap.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\x64\bootstrap\x64bootstrap.asm
;Created by Nathaniel on 1/11/2014 at 17:41
;
;Description: Loads the kernel. It is first MultiBoot module
;**********************************************************/
BITS 32
section .code
org 0x10000
%define BOOTSTRAP_BASE 0x10000
%define BOOTSTRAP_LOAD 0x800000
%define KERNEL_LOAD 0x100000
%define KERNEL_BASE 0xFFFFC00000000000
start:
mov esi, BOOTSTRAP_LOAD
mov edi, BOOTSTRAP_BASE
mov ecx, file_end-BOOTSTRAP_BASE
rep movsb
lgdt [toc]
jmp 0x8:.relocate
.relocate:
mov [mutiboot_info_ptr], ebx
mov eax, [ebx]
test eax, 1<<1
jz .erro
mov eax, [ebx+20]
cmp eax, 0
je .erro
sub eax, 1
mov [ebx+20],eax
;Get kernel
mov eax, [ebx+24]
mov esi, [eax]
mov ecx, [eax+4]
;Copy kernel to load address
sub ecx, esi
;ecx is now byte length of kernel
push eax
push ebx
mov edx, 0
mov eax, ecx
mov ebx, 4
div ebx
mov ecx, eax
pop ebx
pop eax
;now ecx is DWORDs. edx contains remainder
mov edi, KERNEL_LOAD
rep movsd
;Move the last few bytes
mov ecx, edx
rep movsb
;Now remove kernel from modules list
add eax, 16
mov [ebx+24], eax
;Now set up long mode
pushfd               ; Store the FLAGS-register.
    pop eax              ; Restore the A-register.
    mov ecx, eax         ; Set the C-register to the A-register.
    xor eax, 1 << 21     ; Flip the ID-bit, which is bit 21.
    push eax             ; Store the A-register.
    popfd                ; Restore the FLAGS-register.
    pushfd               ; Store the FLAGS-register.
    pop eax              ; Restore the A-register.
    push ecx             ; Store the C-register.
    popfd                ; Restore the FLAGS-register.
    xor eax, ecx         ; Do a XOR-operation on the A-register and the C-register.
    jz .erro          ; The zero flag is set, no CPUID.
	mov eax, 0x80000000    ; Set the A-register to 0x80000000.
    cpuid                  ; CPU identification.
    cmp eax, 0x80000001    ; Compare the A-register with 0x80000001.
    jb .erro         ; It is less, there is no long mode.
	mov eax, 0x80000001    ; Set the A-register to 0x80000001.
    cpuid                  ; CPU identification.
    test edx, 1 << 29      ; Test if the LM-bit, which is bit 29, is set in the D-register.
    jz .erro         ; They aren't, there is no long mode.
	call SetupPaging64
	mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
    rdmsr                        ; Read from the model-specific register.
    or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
    wrmsr                        ; Write to the model-specific register.
	mov eax, cr0                 ; Set the A-register to control register 0.
    or eax, 1 << 31              ; Set the PG-bit, which is the 32nd bit (bit 31).
    mov cr0, eax                 ; Set control register 0 to the A-register.
	lgdt[GDT64.Pointer]
	jmp 0x8:.realm64
	BITS 64
	.realm64:
	mov rax, KERNEL_BASE
	xor rbx, rbx
	mov ebx, [mutiboot_info_ptr]	;Pass mbinfo in rbx and rcx
	xor rcx, rcx
	mov ecx, [rax+0x3C]
	xor rdx, rdx
	mov edx, [rax+rcx+0x28]
	add rax, rdx
	mov rcx, rbx					;the rcx bit
	call rax
	jmp $
	BITS 32
.erro:
lea esi, [msgError]
mov edi, 0xB8000
.loop:
cmp BYTE[esi], 0
je .end
mov al, [esi]
mov ah, 0x4F
mov [edi], ax
inc esi
add edi, 2
jmp .loop
.end:
cli
hlt

msgError: db "FATAL error encountered: No Long Mode!", 0

;*******************************************************
;
;	Paging.inc
;		Basic paging for bootloader
;
;	OS Development Series
;*******************************************************

; Added since Tutorial 17

;With added LM paging for TeraOS - Nathaniel Cleland

%ifndef __PAGING_INC_67343546FDCC56AAB872_INCLUDED__
%define __PAGING_INC_67343546FDCC56AAB872_INCLUDED__

bits	32


;---Paging32 declarations---
; page directory table
%define		PAGE_DIR			0x9C000

; 0th page table. Address must be 4KB aligned
%define		PAGE_TABLE_0		0x9D000

; 768th page table. Address must be 4KB aligned
%define		PAGE_TABLE_768		0x9E000

%define		PAGE_TABLE_769		0x9F000

; each page table has 1024 entries
%define		PAGE_TABLE_ENTRIES	1024

; attributes (page is present;page is writable; supervisor mode)
%define		PRIV				3
;%define			PRIV				7

;****************************************
;	Enable Paging
;****************************************

EnablePaging:
	pusha										; save stack frame

	;------------------------------------------
	;	idenitity map 1st page table (4MB)
	;------------------------------------------

	mov		eax, PAGE_TABLE_0					; first page table
	mov		ebx, 0x0 | PRIV						; starting physical address of page
	mov		ecx, PAGE_TABLE_ENTRIES				; for every page in table...
.loop:
	mov		dword [eax], ebx					; write the entry
	add		eax, 4								; go to next page entry in table (Each entry is 4 bytes)
	add		ebx, 4096							; go to next page address (Each page is 4Kb)
	loop	.loop								; go to next entry
	;------------------------------------------
	;		Do the same for the second 4MB
	;------------------------------------------
	mov		eax, PAGE_TABLE_769					; first page table
	mov		ebx, 1024*1024*4 | PRIV				; starting physical address of page
	mov		ecx, PAGE_TABLE_ENTRIES				; for every page in table...
.loophigher4:
	mov		dword [eax], ebx					; write the entry
	add		eax, 4								; go to next page entry in table (Each entry is 4 bytes)
	add		ebx, 4096							; go to next page address (Each page is 4Kb)
	loop	.loophigher4

	;------------------------------------------
	;	set up the entries in the directory table
	;------------------------------------------

	mov		eax, PAGE_TABLE_0 | PRIV			; 1st table is directory entry 0
	mov		dword [PAGE_DIR], eax

	mov		eax, PAGE_TABLE_768 | PRIV			; 768th entry in directory table
	mov		dword [PAGE_DIR+(768*4)], eax
	mov		eax, PAGE_TABLE_769 | PRIV
	mov		dword [PAGE_DIR+(769*4)], eax

	;------------------------------------------
	;	install directory table
	;------------------------------------------

	mov		eax, PAGE_DIR
	mov		cr3, eax

	;------------------------------------------
	;	enable paging
	;------------------------------------------

	mov		eax, cr0
	or		eax, 0x80000000
	mov		cr0, eax

	;------------------------------------------
	;	map the 768th table to physical addr 1MB
	;	the 768th table starts the 3gb virtual address
	;------------------------------------------
 
	mov		eax, PAGE_TABLE_768				; first page table
	mov		ebx, 0x100000 | PRIV			; starting physical address of page
	mov		ecx, PAGE_TABLE_ENTRIES			; for every page in table...
.loop2:
	mov		dword [eax], ebx				; write the entry
	add		eax, 4							; go to next page entry in table (Each entry is 4 bytes)
	add		ebx, 4096						; go to next page address (Each page is 4Kb)
	loop	.loop2							; go to next entry

	popa
	ret

;============Paging64=============
%define PAGE_TABLE_770 0x97000
%define PAGE_TABLE_771 0x98000
%define PML4T 0x96000
%define PAGE_DIR_POINTER 0x9A000
%define PAGE_DIR_2 0x9B000
%define PAGE_DIR_POINTER_2 0x99000
;INTERNAL: Loop to fill page table
;EBX = physical address, EDI = page table
SetEntry:
or ebx, PRIV
mov ecx, 512                 ; Set the C-register to 512.
.setentryloop:
    mov DWORD [edi], ebx         ; Set the double word at the destination index to the B-register.
	mov DWORD [edi+4], 0
    add ebx, 0x1000              ; Add 0x1000 to the B-register.
    add edi, 8                   ; Add eight to the destination index.
    loop .setentryloop               ; Set the next entry.
ret
;32 bit function, 64 bit paging
SetupPaging64:
;Clear PML4T
mov edi, PML4T
xor eax, eax
mov ecx, 1024*6
rep stosd
mov edi, PML4T
;Set up pointers
mov DWORD[edi], PAGE_DIR_POINTER | PRIV
mov DWORD[edi+8*384], PAGE_DIR_POINTER_2 | PRIV
mov DWORD[edi+8*511], PML4T | PRIV		;Fractal
mov edi, PAGE_DIR_POINTER
xor eax, eax
mov ecx, 1024
rep stosd
mov edi, PAGE_DIR_POINTER
mov DWORD[edi], PAGE_DIR | PRIV
mov DWORD[edi+8*3], PAGE_DIR_2 | PRIV	;for compatibility (we use the 32 bit addr in bootloader)
mov edi, PAGE_DIR_POINTER_2				;64 bit address
xor eax, eax
mov ecx, 1024
rep stosd
mov edi, PAGE_DIR_POINTER_2
mov DWORD[edi], PAGE_DIR_2 | PRIV
mov edi, PAGE_DIR_2
xor eax, eax
mov ecx, 1024
rep stosd
mov edi, PAGE_DIR_2
mov DWORD[edi], PAGE_TABLE_768 | PRIV
mov DWORD[edi+8], PAGE_TABLE_769 | PRIV
mov DWORD[edi+16], PAGE_TABLE_770 | PRIV
mov DWORD[edi+24], PAGE_TABLE_771 | PRIV
mov edi, PAGE_DIR
xor eax, eax
mov ecx, 1024
rep stosd
mov edi, PAGE_DIR
mov DWORD[edi], PAGE_TABLE_0 | PRIV
mov edi, PAGE_TABLE_0
mov ebx, 0x00000000          ; Set the B-register to 0x00000003.
call SetEntry
mov edi, PAGE_TABLE_768
mov ebx, 0x00100000          ;KERNEL (1MB)
call SetEntry
mov edi, PAGE_TABLE_769
mov ebx, 0x00300000
call SetEntry
mov edi, PAGE_TABLE_770
mov ebx, 0x00500000
call SetEntry
mov edi, PAGE_TABLE_771
mov ebx, 0x00700000
call SetEntry
;Now enable PAE
mov edi, PML4T
mov cr3, edi
mov eax, cr4
or eax, 1<<5
mov cr4, eax
ret
%endif

section .data
align 4
MULTIBOOT_HEADER:
dd 0x1BADB002
dd 0x00010007
dd -(0x1BADB002 + 0x00010007)
dd MULTIBOOT_HEADER-0x10000+0x800000
dd 0x800000
dd 0
dd 0
dd start-0x10000+0x800000
dd 0
dd 1024
dd 768
dd 24

mutiboot_info_ptr: dq 0

gdt_data: 
	dd 0                ; null descriptor
	dd 0 

; gdt code:	            ; code descriptor
	dw 0FFFFh           ; limit low
	dw 0                ; base low
	db 0                ; base middle
	db 10011010b        ; access
	db 11001111b        ; granularity
	db 0                ; base high

; gdt data:	            ; data descriptor
.data:
	dw 0FFFFh           ; limit low (Same as code)10:56 AM 7/8/2007
	dw 0                ; base low
	db 0                ; base middle
	db 10010010b        ; access
	db 11001111b        ; granularity
	db 0                ; base high
	
end_of_gdt:
toc: 
	dw end_of_gdt - gdt_data - 1 	; limit (Size of GDT)
	dd gdt_data 			; base of GDT

GDT64:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT64         ; The null descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 0                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - GDT64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access.
    db 00100000b                 ; Granularity.
    db 0                         ; Base (high).
    .Data: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access.
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .Pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64                     ; Base.
	

	
file_end:
