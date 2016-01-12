;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: ChaiOS
;File: chaimodule16.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\chaimodule16.asm
;Created by Nathaniel on 29/6/2015 at 12:09
;
;Description: ChaiOS 16 Bit Module - Common Macros. Include at top of modules (refer to documentation)
;**********************************************************/

;CHAIOS16 header - arg1 - base address, arg2 - Module name ("......", max 7 chars)
;Use this at the file beginning - then entry point is modentry
%macro header 2
BITS 16
section .text

%strlen modnamelen %2

%if modnamelen > 7
%error Module Name %2 is too long (%[modnamelen] chars, the max is 7)
%endif

org %1
start:
jmp modentry
align 4
db "CHAIOS16"
db %2
TIMES 8-modnamelen db 0
dd %1
dd ChaiOSentry32
dq ChaiOSentry64

%endmacro

;****************************************************************************
;Put this at the end of the file. This handles 32/64 bit stuff. No parameters
;If necessary, define prologue and epilogue functions - func_prologue_32, func_prologue_64,
;func_epilogue_32 and func_epilogue_64 - as macros

%macro footer 0

;Default macros, if macros not defined
%ifnmacro func_prologue_32
%macro func_prologue_32 0
nop
%endmacro
%endif

%ifnmacro func_prologue_64
%macro func_prologue_64 0
nop
%endmacro
%endif

%ifnmacro func_epilogue_32
%macro func_epilogue_32 0
nop
%endmacro
%endif

%ifnmacro func_epilogue_64
%macro func_epilogue_64 0
nop
%endmacro
%endif

BITS 32
ChaiOSentry32:
pushad
;Prologue function
func_prologue_32
cli
sgdt [gdtold]
sgdt [gdtnew]
sidt [idtold]
mov DWORD[os_bits], 32
;Now use existing GDT, but load it for non-paging
mov esi, [gdtold.base]
xor ecx, ecx
mov cx, [gdtold.limit]
add cx, 1
lea edi, [GDT]
rep movsb
lea eax, [GDT]
mov [gdtnew.base], eax
lgdt [gdtnew]
mov ax, 0x30	;Data 16 segment
mov WORD[savess], 0x10
call 0x28:Entry16
lgdt [gdtold]
lidt [idtold]
;Epilogue function
func_epilogue_32
popad
ret

BITS 64
bits32addr:
dq ChaiOSentry64.bits32
dw 0x28
ChaiOSentry64:
push rbx
push rsi
push rcx
push rdi
push rax
;Prologue function
func_prologue_64
cli
sgdt [gdtold]
sgdt [gdtnew]
sidt [idtold]
;Now use existing GDT, but load it for non-paging
mov rsi, [gdtold.base]
xor rcx, rcx
mov cx, [gdtold.limit]
add cx, 1
lea rdi, [GDT]
rep movsb
lea rax, [GDT]
mov [gdtnew.base], rax
mov [saveesp], rsp
lgdt [gdtnew]
jmp far [bits32addr]
BITS 32
.bits32:
mov ax, 0x30
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

mov DWORD[os_bits], 64
;We are in compatibliity mode. Now we need to move to legacy mode
mov eax, cr0
and eax, 0x7FFFFFFF
mov cr0, eax
jmp 0x28:.pmode
.pmode:
mov ax, 0x50	;Data 16 segment
mov WORD[savess], 0x30
call 0x48:Entry16_64
mov eax, cr0
or eax, 0x80000000
mov cr0, eax
jmp 0x8:.end
BITS 64
.end:
mov rsp, [saveesp]
mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
lgdt [gdtold]
lidt [idtold]
;Epilogue function
func_epilogue_64
pop rbx

pop rax
pop rdi
pop rcx
pop rsi
pop rbx
ret

BITS 16 
idt_real:
	dw 0x3ff		; 256 entries, 4b each = 1K
	dq 0			; Real Mode IVT @ 0x0000
 
savcr0:
	dq 0			; Storage location for pmode CR0.
saveesp:
	dq 0
savess:
	dw 0
savecs:
	dw 0
 
Entry16:
        ; We are already in 16-bit mode here!
 
	cli			; Disable interrupts.
	mov [saveesp], esp
	; Need 16-bit Protected Mode GDT entries!
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
 
	; Disable paging (we need everything to be 1:1 mapped).
	mov eax, cr0
	mov [savcr0], eax	; save pmode CR0
	and eax, 0x7FFFFFFe	; Disable paging bit & enable 16-bit pmode.
	mov cr0, eax
 
	jmp 0:GoRMode		; Perform Far jump to set CS.
 
GoRMode:
	mov sp, 0x9000		; pick a stack pointer.
	mov ax, 0		; Reset segment registers to 0.
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	lidt [idt_real]
	sti			; Restore interrupts -- be careful, unhandled int's will kill it.
	call modentry
	cli
	lidt [idtold]
	mov bx, [savecs]
	mov eax, [savcr0]
	mov cr0, eax
	jmp 0x8:.restored
	BITS 32
	.restored:
	mov ax, [savess]
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, [saveesp]
	retf

	BITS 16
Entry16_64:
        ; We are already in 16-bit mode here!
 
	cli			; Disable interrupts.
 
	; Need 16-bit Protected Mode GDT entries!
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov [saveesp], esp
 
	; Disable paging (we need everything to be 1:1 mapped).
	mov eax, cr0
	mov [savcr0], eax	; save pmode CR0
	and eax, 0x7FFFFFFe	; Disable paging bit & enable 16-bit pmode.
	mov cr0, eax
 
	jmp 0:GoRMode_64		; Perform Far jump to set CS.
 
GoRMode_64:
	mov sp, 0x9000		; pick a stack pointer.
	mov ax, 0		; Reset segment registers to 0.
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	lidt [idt_real]
	sti			; Restore interrupts -- be careful, unhandled int's will kill it.
	call modentry
	cli
	lidt [idtold]
	mov bx, [savecs]
	mov esp, [saveesp]
	mov eax, [savcr0]
	mov cr0, eax
	jmp 0x28:.restored
	BITS 32
	.restored:
	mov ax, [savess]
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	retf
	

section .data

gdtold:
.limit: dw 0
.base: dq 0
idtold: dq 0,0

gdtnew:
.limit: dw 0
.base: dq 0

GDT:
TIMES 16 dq 0

os_bits dd 0

%endmacro

;/******************************************
;Runtime functions used for OS interface.
;Please use these functions sparsely,
;since the overhead of mode changing is huge.
;******************************************/

;callsystem: performs a system call.
;Arg1: intN. Arg2: call number. 
%macro callsystem 2-5 0

;Call the pmode OS.
mov eax, [os_bits]
cmp eax, 64
jg %%unsupported
je %%bits64
cmp eax, 32
je %%bits32
;Something's gone wrong
jmp %%unsupported

%%bits32:
cli
mov BYTE[interrupt+1], %0
lidt [idtold]
mov bx, [savecs]
mov eax, [savcr0]
mov cr0, eax
jmp 0x8:.restored
BITS 32
.restored:
mov ax, [savess]
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov esp, [saveesp]
lgdt [gdtold]
mov eax, %1
mov rbx, %2
mov rcx, %3
mov rdx, %4
.interrupt:		;Calls the OS
int 0

;Now return back to 16 bit land

cli
hlt

jmp %%end

%%bits64:


%%unsupported:
;An unsupported (>64 bit or invalid) OS. Don't know how we got here.
cli
hlt


%%end

%endmacro