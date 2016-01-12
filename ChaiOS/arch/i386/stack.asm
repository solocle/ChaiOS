;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: ChaiOS
;File: stack.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\i386\stack.asm
;Created by Nathaniel on 31/10/2014 at 15:49
;
;Description: Stack management functions
;**********************************************************/
BITS 32
segment .text

global _setStack
_setStack:
mov edx, [esp]
mov esp, [esp+4]
push 0
mov ebp, esp
jmp edx

global _getStack
_getStack:
mov eax, esp
ret


global _getBase
_getBase:
mov eax, ebp
ret

global _storeMBinfo
_storeMBinfo:
mov ecx, ebx
mov esi, ebx
ret

;Not strictly stack, but just handy here
extern _module_loader_handler
global _modload_interrupt_handler
_modload_interrupt_handler:
;Create struct
push edx
push ecx
push ebx
push eax
;Now push struct for cdecl
push esp
;And call the handler
call _module_loader_handler
add esp, 20
iret
