;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: ChaiOS
;File: stack.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\x64\stack.asm
;Created by Nathaniel on 31/10/2014 at 18:12
;
;Description: Stack management functions
;**********************************************************/
BITS 64
segment .text

global setStack
setStack:
mov rax, [rsp]
mov rsp, rcx
push 0
push 0
mov rbp, rsp
jmp rax

global getStack
getStack:
mov rax, rsp
ret


global getBase
getBase:
mov rax, rbp
ret

;Not strictly stack, but just handy here
extern module_loader_handler
global modload_interrupt_handler
modload_interrupt_handler:
;Create struct
sub rsp, 32
mov [rsp+24], rdx
mov [rsp+16], rcx
mov [rsp+8], rbx
mov [rsp], rax
;Now sort for fastcall
mov rcx, rsp
;And call the handler
call module_loader_handler
add rsp, 32
iretq
