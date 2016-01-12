;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: Hal
;File: Interrupts64.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\Interrupts64.asm
;Created by Nathaniel on 14/7/2015 at 03:22
;
;Description: Interrupt Handler Entry Points
;**********************************************************/
BITS 64
section .code

extern interruptHandler

%macro SAVE_STATE_IRQ 1
push rbp
mov rbp, rsp
push rax
push rcx
push rdx
push r8
push r9
push r10
push r11
push r12
;Segments
push ds
push es
push fs
mov ax, 0x10
mov ds, ax
mov es, ax
;Kernel TLS could be loaded
;Exception struct
%if %1
mov r11, tempErrorCode
mov r12, [r11]
push r12
push rbp
mov r8, rsp		;Helps retrieve the stack
%else
mov r12, 0
push r12
push rbp
mov r8, rsp		;Helps retrieve the stack
%endif
%endmacro

%macro RESTORE_STATE_IRQ 1
%if %1
add rsp, 16
%endif
pop fs
pop es
pop ds
pop r12
pop r11
pop r10
pop r9
pop r8
pop rdx
pop rcx
pop rax
pop rbp
%endmacro

%macro SORT_ERROR_CODE 0
push rax
push rbx
mov rax, [rsp+16]
mov rbx, tempErrorCode
mov [rbx], rax
pop rbx
pop rax
add rsp, 8
%endmacro

global intrptHndlr0
intrptHndlr0:
SAVE_STATE_IRQ 1
mov rcx, 0
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr1
intrptHndlr1:
SAVE_STATE_IRQ 1
mov rcx, 1
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr2
intrptHndlr2:
SAVE_STATE_IRQ 1
mov rcx, 2
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr3
intrptHndlr3:
SAVE_STATE_IRQ 1
mov rcx, 3
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr4
intrptHndlr4:
SAVE_STATE_IRQ 1
mov rcx, 4
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr5
intrptHndlr5:
SAVE_STATE_IRQ 1
mov rcx, 5
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr6
intrptHndlr6:
SAVE_STATE_IRQ 1
mov rcx, 6
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr7
intrptHndlr7:
SAVE_STATE_IRQ 1
mov rcx, 7
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr8
intrptHndlr8:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 8
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr9
intrptHndlr9:
SAVE_STATE_IRQ 1
mov rcx, 9
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr10
intrptHndlr10:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 10
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr11
intrptHndlr11:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 11
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr12
intrptHndlr12:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 12
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr13
intrptHndlr13:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 13
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr14
intrptHndlr14:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 14
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr15
intrptHndlr15:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 15
mov rdx, r8
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr16
intrptHndlr16:
SAVE_STATE_IRQ 1
mov rcx, 16
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr17
intrptHndlr17:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 17
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr18
intrptHndlr18:
SAVE_STATE_IRQ 1
mov rcx, 18
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr19
intrptHndlr19:
SAVE_STATE_IRQ 1
mov rcx, 19
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr20
intrptHndlr20:
SAVE_STATE_IRQ 1
mov rcx, 20
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr21
intrptHndlr21:
SAVE_STATE_IRQ 1
mov rcx, 21
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr22
intrptHndlr22:
SAVE_STATE_IRQ 1
mov rcx, 22
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr23
intrptHndlr23:
SAVE_STATE_IRQ 1
mov rcx, 23
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr24
intrptHndlr24:
SAVE_STATE_IRQ 1
mov rcx, 24
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr25
intrptHndlr25:
SAVE_STATE_IRQ 1
mov rcx, 25
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr26
intrptHndlr26:
SAVE_STATE_IRQ 1
mov rcx, 26
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr27
intrptHndlr27:
SAVE_STATE_IRQ 1
mov rcx, 27
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr28
intrptHndlr28:
SAVE_STATE_IRQ 1
mov rcx, 28
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr29
intrptHndlr29:
SAVE_STATE_IRQ 1
mov rcx, 29
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr30
intrptHndlr30:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov rcx, 30
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr31
intrptHndlr31:
SAVE_STATE_IRQ 1
mov rcx, 31
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 1
iretq

global intrptHndlr32
intrptHndlr32:
SAVE_STATE_IRQ 0
mov rcx, 32
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr33
intrptHndlr33:
SAVE_STATE_IRQ 0
mov rcx, 33
push rdx
push rcx
push rbx
push rax
lea rdx, [rsp]
sub rsp, 32
call interruptHandler
add rsp, 32
add rsp, 0x20
mov [rsp+56], rax		;Preserve return value
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr34
intrptHndlr34:
SAVE_STATE_IRQ 0
mov rcx, 34
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr35
intrptHndlr35:
SAVE_STATE_IRQ 0
mov rcx, 35
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr36
intrptHndlr36:
SAVE_STATE_IRQ 0
mov rcx, 36
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr37
intrptHndlr37:
SAVE_STATE_IRQ 0
mov rcx, 37
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr38
intrptHndlr38:
SAVE_STATE_IRQ 0
mov rcx, 38
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr39
intrptHndlr39:
SAVE_STATE_IRQ 0
mov rcx, 39
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr40
intrptHndlr40:
SAVE_STATE_IRQ 0
mov rcx, 40
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr41
intrptHndlr41:
SAVE_STATE_IRQ 0
mov rcx, 41
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr42
intrptHndlr42:
SAVE_STATE_IRQ 0
mov rcx, 42
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr43
intrptHndlr43:
SAVE_STATE_IRQ 0
mov rcx, 43
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr44
intrptHndlr44:
SAVE_STATE_IRQ 0
mov rcx, 44
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr45
intrptHndlr45:
SAVE_STATE_IRQ 0
mov rcx, 45
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr46
intrptHndlr46:
SAVE_STATE_IRQ 0
mov rcx, 46
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr47
intrptHndlr47:
SAVE_STATE_IRQ 0
mov rcx, 47
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr48
intrptHndlr48:
SAVE_STATE_IRQ 0
mov rcx, 48
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr49
intrptHndlr49:
SAVE_STATE_IRQ 0
mov rcx, 49
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr50
intrptHndlr50:
SAVE_STATE_IRQ 0
mov rcx, 50
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr51
intrptHndlr51:
SAVE_STATE_IRQ 0
mov rcx, 51
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr52
intrptHndlr52:
SAVE_STATE_IRQ 0
mov rcx, 52
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr53
intrptHndlr53:
SAVE_STATE_IRQ 0
mov rcx, 53
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr54
intrptHndlr54:
SAVE_STATE_IRQ 0
mov rcx, 54
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr55
intrptHndlr55:
SAVE_STATE_IRQ 0
mov rcx, 55
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr56
intrptHndlr56:
SAVE_STATE_IRQ 0
mov rcx, 56
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr57
intrptHndlr57:
SAVE_STATE_IRQ 0
mov rcx, 57
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr58
intrptHndlr58:
SAVE_STATE_IRQ 0
mov rcx, 58
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr59
intrptHndlr59:
SAVE_STATE_IRQ 0
mov rcx, 59
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr60
intrptHndlr60:
SAVE_STATE_IRQ 0
mov rcx, 60
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr61
intrptHndlr61:
SAVE_STATE_IRQ 0
mov rcx, 61
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr62
intrptHndlr62:
SAVE_STATE_IRQ 0
mov rcx, 62
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr63
intrptHndlr63:
SAVE_STATE_IRQ 0
mov rcx, 63
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr64
intrptHndlr64:
SAVE_STATE_IRQ 0
mov rcx, 64
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr65
intrptHndlr65:
SAVE_STATE_IRQ 0
mov rcx, 65
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr66
intrptHndlr66:
SAVE_STATE_IRQ 0
mov rcx, 66
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr67
intrptHndlr67:
SAVE_STATE_IRQ 0
mov rcx, 67
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr68
intrptHndlr68:
SAVE_STATE_IRQ 0
mov rcx, 68
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr69
intrptHndlr69:
SAVE_STATE_IRQ 0
mov rcx, 69
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr70
intrptHndlr70:
SAVE_STATE_IRQ 0
mov rcx, 70
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr71
intrptHndlr71:
SAVE_STATE_IRQ 0
mov rcx, 71
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr72
intrptHndlr72:
SAVE_STATE_IRQ 0
mov rcx, 72
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr73
intrptHndlr73:
SAVE_STATE_IRQ 0
mov rcx, 73
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr74
intrptHndlr74:
SAVE_STATE_IRQ 0
mov rcx, 74
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr75
intrptHndlr75:
SAVE_STATE_IRQ 0
mov rcx, 75
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr76
intrptHndlr76:
SAVE_STATE_IRQ 0
mov rcx, 76
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr77
intrptHndlr77:
SAVE_STATE_IRQ 0
mov rcx, 77
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr78
intrptHndlr78:
SAVE_STATE_IRQ 0
mov rcx, 78
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr79
intrptHndlr79:
SAVE_STATE_IRQ 0
mov rcx, 79
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr80
intrptHndlr80:
SAVE_STATE_IRQ 0
mov rcx, 80
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr81
intrptHndlr81:
SAVE_STATE_IRQ 0
mov rcx, 81
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr82
intrptHndlr82:
SAVE_STATE_IRQ 0
mov rcx, 82
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr83
intrptHndlr83:
SAVE_STATE_IRQ 0
mov rcx, 83
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr84
intrptHndlr84:
SAVE_STATE_IRQ 0
mov rcx, 84
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr85
intrptHndlr85:
SAVE_STATE_IRQ 0
mov rcx, 85
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr86
intrptHndlr86:
SAVE_STATE_IRQ 0
mov rcx, 86
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr87
intrptHndlr87:
SAVE_STATE_IRQ 0
mov rcx, 87
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr88
intrptHndlr88:
SAVE_STATE_IRQ 0
mov rcx, 88
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr89
intrptHndlr89:
SAVE_STATE_IRQ 0
mov rcx, 89
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr90
intrptHndlr90:
SAVE_STATE_IRQ 0
mov rcx, 90
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr91
intrptHndlr91:
SAVE_STATE_IRQ 0
mov rcx, 91
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr92
intrptHndlr92:
SAVE_STATE_IRQ 0
mov rcx, 92
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr93
intrptHndlr93:
SAVE_STATE_IRQ 0
mov rcx, 93
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr94
intrptHndlr94:
SAVE_STATE_IRQ 0
mov rcx, 94
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr95
intrptHndlr95:
SAVE_STATE_IRQ 0
mov rcx, 95
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr96
intrptHndlr96:
SAVE_STATE_IRQ 0
mov rcx, 96
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr97
intrptHndlr97:
SAVE_STATE_IRQ 0
mov rcx, 97
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr98
intrptHndlr98:
SAVE_STATE_IRQ 0
mov rcx, 98
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr99
intrptHndlr99:
SAVE_STATE_IRQ 0
mov rcx, 99
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr100
intrptHndlr100:
SAVE_STATE_IRQ 0
mov rcx, 100
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr101
intrptHndlr101:
SAVE_STATE_IRQ 0
mov rcx, 101
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr102
intrptHndlr102:
SAVE_STATE_IRQ 0
mov rcx, 102
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr103
intrptHndlr103:
SAVE_STATE_IRQ 0
mov rcx, 103
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr104
intrptHndlr104:
SAVE_STATE_IRQ 0
mov rcx, 104
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr105
intrptHndlr105:
SAVE_STATE_IRQ 0
mov rcx, 105
push rdx
push rcx
push rbx
push rax
lea rdx, [rsp]
sub rsp, 32
call interruptHandler
add rsp, 32
add rsp, 0x20
mov [rsp+56], rax		;Preserve return value
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr106
intrptHndlr106:
SAVE_STATE_IRQ 0
mov rcx, 106
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr107
intrptHndlr107:
SAVE_STATE_IRQ 0
mov rcx, 107
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr108
intrptHndlr108:
SAVE_STATE_IRQ 0
mov rcx, 108
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr109
intrptHndlr109:
SAVE_STATE_IRQ 0
mov rcx, 109
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr110
intrptHndlr110:
SAVE_STATE_IRQ 0
mov rcx, 110
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr111
intrptHndlr111:
SAVE_STATE_IRQ 0
mov rcx, 111
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr112
intrptHndlr112:
SAVE_STATE_IRQ 0
mov rcx, 112
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr113
intrptHndlr113:
SAVE_STATE_IRQ 0
mov rcx, 113
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr114
intrptHndlr114:
SAVE_STATE_IRQ 0
mov rcx, 114
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr115
intrptHndlr115:
SAVE_STATE_IRQ 0
mov rcx, 115
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr116
intrptHndlr116:
SAVE_STATE_IRQ 0
mov rcx, 116
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr117
intrptHndlr117:
SAVE_STATE_IRQ 0
mov rcx, 117
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr118
intrptHndlr118:
SAVE_STATE_IRQ 0
mov rcx, 118
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr119
intrptHndlr119:
SAVE_STATE_IRQ 0
mov rcx, 119
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr120
intrptHndlr120:
SAVE_STATE_IRQ 0
mov rcx, 120
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr121
intrptHndlr121:
SAVE_STATE_IRQ 0
mov rcx, 121
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr122
intrptHndlr122:
SAVE_STATE_IRQ 0
mov rcx, 122
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr123
intrptHndlr123:
SAVE_STATE_IRQ 0
mov rcx, 123
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr124
intrptHndlr124:
SAVE_STATE_IRQ 0
mov rcx, 124
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr125
intrptHndlr125:
SAVE_STATE_IRQ 0
mov rcx, 125
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr126
intrptHndlr126:
SAVE_STATE_IRQ 0
mov rcx, 126
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr127
intrptHndlr127:
SAVE_STATE_IRQ 0
mov rcx, 127
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr128
intrptHndlr128:
SAVE_STATE_IRQ 0
mov rcx, 128
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr129
intrptHndlr129:
SAVE_STATE_IRQ 0
mov rcx, 129
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr130
intrptHndlr130:
SAVE_STATE_IRQ 0
mov rcx, 130
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr131
intrptHndlr131:
SAVE_STATE_IRQ 0
mov rcx, 131
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr132
intrptHndlr132:
SAVE_STATE_IRQ 0
mov rcx, 132
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr133
intrptHndlr133:
SAVE_STATE_IRQ 0
mov rcx, 133
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr134
intrptHndlr134:
SAVE_STATE_IRQ 0
mov rcx, 134
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr135
intrptHndlr135:
SAVE_STATE_IRQ 0
mov rcx, 135
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr136
intrptHndlr136:
SAVE_STATE_IRQ 0
mov rcx, 136
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr137
intrptHndlr137:
SAVE_STATE_IRQ 0
mov rcx, 137
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr138
intrptHndlr138:
SAVE_STATE_IRQ 0
mov rcx, 138
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr139
intrptHndlr139:
SAVE_STATE_IRQ 0
mov rcx, 139
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr140
intrptHndlr140:
SAVE_STATE_IRQ 0
mov rcx, 140
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr141
intrptHndlr141:
SAVE_STATE_IRQ 0
mov rcx, 141
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr142
intrptHndlr142:
SAVE_STATE_IRQ 0
mov rcx, 142
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr143
intrptHndlr143:
SAVE_STATE_IRQ 0
mov rcx, 143
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr144
intrptHndlr144:
SAVE_STATE_IRQ 0
mov rcx, 144
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr145
intrptHndlr145:
SAVE_STATE_IRQ 0
mov rcx, 145
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr146
intrptHndlr146:
SAVE_STATE_IRQ 0
mov rcx, 146
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr147
intrptHndlr147:
SAVE_STATE_IRQ 0
mov rcx, 147
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr148
intrptHndlr148:
SAVE_STATE_IRQ 0
mov rcx, 148
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr149
intrptHndlr149:
SAVE_STATE_IRQ 0
mov rcx, 149
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr150
intrptHndlr150:
SAVE_STATE_IRQ 0
mov rcx, 150
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr151
intrptHndlr151:
SAVE_STATE_IRQ 0
mov rcx, 151
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr152
intrptHndlr152:
SAVE_STATE_IRQ 0
mov rcx, 152
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr153
intrptHndlr153:
SAVE_STATE_IRQ 0
mov rcx, 153
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr154
intrptHndlr154:
SAVE_STATE_IRQ 0
mov rcx, 154
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr155
intrptHndlr155:
SAVE_STATE_IRQ 0
mov rcx, 155
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr156
intrptHndlr156:
SAVE_STATE_IRQ 0
mov rcx, 156
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr157
intrptHndlr157:
SAVE_STATE_IRQ 0
mov rcx, 157
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr158
intrptHndlr158:
SAVE_STATE_IRQ 0
mov rcx, 158
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr159
intrptHndlr159:
SAVE_STATE_IRQ 0
mov rcx, 159
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr160
intrptHndlr160:
SAVE_STATE_IRQ 0
mov rcx, 160
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr161
intrptHndlr161:
SAVE_STATE_IRQ 0
mov rcx, 161
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr162
intrptHndlr162:
SAVE_STATE_IRQ 0
mov rcx, 162
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr163
intrptHndlr163:
SAVE_STATE_IRQ 0
mov rcx, 163
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr164
intrptHndlr164:
SAVE_STATE_IRQ 0
mov rcx, 164
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr165
intrptHndlr165:
SAVE_STATE_IRQ 0
mov rcx, 165
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr166
intrptHndlr166:
SAVE_STATE_IRQ 0
mov rcx, 166
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr167
intrptHndlr167:
SAVE_STATE_IRQ 0
mov rcx, 167
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr168
intrptHndlr168:
SAVE_STATE_IRQ 0
mov rcx, 168
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr169
intrptHndlr169:
SAVE_STATE_IRQ 0
mov rcx, 169
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr170
intrptHndlr170:
SAVE_STATE_IRQ 0
mov rcx, 170
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr171
intrptHndlr171:
SAVE_STATE_IRQ 0
mov rcx, 171
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr172
intrptHndlr172:
SAVE_STATE_IRQ 0
mov rcx, 172
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr173
intrptHndlr173:
SAVE_STATE_IRQ 0
mov rcx, 173
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr174
intrptHndlr174:
SAVE_STATE_IRQ 0
mov rcx, 174
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr175
intrptHndlr175:
SAVE_STATE_IRQ 0
mov rcx, 175
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr176
intrptHndlr176:
SAVE_STATE_IRQ 0
mov rcx, 176
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr177
intrptHndlr177:
SAVE_STATE_IRQ 0
mov rcx, 177
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr178
intrptHndlr178:
SAVE_STATE_IRQ 0
mov rcx, 178
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr179
intrptHndlr179:
SAVE_STATE_IRQ 0
mov rcx, 179
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr180
intrptHndlr180:
SAVE_STATE_IRQ 0
mov rcx, 180
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr181
intrptHndlr181:
SAVE_STATE_IRQ 0
mov rcx, 181
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr182
intrptHndlr182:
SAVE_STATE_IRQ 0
mov rcx, 182
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr183
intrptHndlr183:
SAVE_STATE_IRQ 0
mov rcx, 183
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr184
intrptHndlr184:
SAVE_STATE_IRQ 0
mov rcx, 184
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr185
intrptHndlr185:
SAVE_STATE_IRQ 0
mov rcx, 185
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr186
intrptHndlr186:
SAVE_STATE_IRQ 0
mov rcx, 186
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr187
intrptHndlr187:
SAVE_STATE_IRQ 0
mov rcx, 187
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr188
intrptHndlr188:
SAVE_STATE_IRQ 0
mov rcx, 188
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr189
intrptHndlr189:
SAVE_STATE_IRQ 0
mov rcx, 189
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr190
intrptHndlr190:
SAVE_STATE_IRQ 0
mov rcx, 190
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr191
intrptHndlr191:
SAVE_STATE_IRQ 0
mov rcx, 191
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr192
intrptHndlr192:
SAVE_STATE_IRQ 0
mov rcx, 192
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr193
intrptHndlr193:
SAVE_STATE_IRQ 0
mov rcx, 193
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr194
intrptHndlr194:
SAVE_STATE_IRQ 0
mov rcx, 194
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr195
intrptHndlr195:
SAVE_STATE_IRQ 0
mov rcx, 195
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr196
intrptHndlr196:
SAVE_STATE_IRQ 0
mov rcx, 196
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr197
intrptHndlr197:
SAVE_STATE_IRQ 0
mov rcx, 197
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr198
intrptHndlr198:
SAVE_STATE_IRQ 0
mov rcx, 198
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr199
intrptHndlr199:
SAVE_STATE_IRQ 0
mov rcx, 199
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr200
intrptHndlr200:
SAVE_STATE_IRQ 0
mov rcx, 200
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr201
intrptHndlr201:
SAVE_STATE_IRQ 0
mov rcx, 201
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr202
intrptHndlr202:
SAVE_STATE_IRQ 0
mov rcx, 202
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr203
intrptHndlr203:
SAVE_STATE_IRQ 0
mov rcx, 203
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr204
intrptHndlr204:
SAVE_STATE_IRQ 0
mov rcx, 204
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr205
intrptHndlr205:
SAVE_STATE_IRQ 0
mov rcx, 205
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr206
intrptHndlr206:
SAVE_STATE_IRQ 0
mov rcx, 206
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr207
intrptHndlr207:
SAVE_STATE_IRQ 0
mov rcx, 207
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr208
intrptHndlr208:
SAVE_STATE_IRQ 0
mov rcx, 208
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr209
intrptHndlr209:
SAVE_STATE_IRQ 0
mov rcx, 209
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr210
intrptHndlr210:
SAVE_STATE_IRQ 0
mov rcx, 210
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr211
intrptHndlr211:
SAVE_STATE_IRQ 0
mov rcx, 211
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr212
intrptHndlr212:
SAVE_STATE_IRQ 0
mov rcx, 212
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr213
intrptHndlr213:
SAVE_STATE_IRQ 0
mov rcx, 213
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr214
intrptHndlr214:
SAVE_STATE_IRQ 0
mov rcx, 214
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr215
intrptHndlr215:
SAVE_STATE_IRQ 0
mov rcx, 215
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr216
intrptHndlr216:
SAVE_STATE_IRQ 0
mov rcx, 216
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr217
intrptHndlr217:
SAVE_STATE_IRQ 0
mov rcx, 217
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr218
intrptHndlr218:
SAVE_STATE_IRQ 0
mov rcx, 218
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr219
intrptHndlr219:
SAVE_STATE_IRQ 0
mov rcx, 219
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr220
intrptHndlr220:
SAVE_STATE_IRQ 0
mov rcx, 220
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr221
intrptHndlr221:
SAVE_STATE_IRQ 0
mov rcx, 221
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr222
intrptHndlr222:
SAVE_STATE_IRQ 0
mov rcx, 222
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr223
intrptHndlr223:
SAVE_STATE_IRQ 0
mov rcx, 223
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr224
intrptHndlr224:
SAVE_STATE_IRQ 0
mov rcx, 224
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr225
intrptHndlr225:
SAVE_STATE_IRQ 0
mov rcx, 225
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr226
intrptHndlr226:
SAVE_STATE_IRQ 0
mov rcx, 226
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr227
intrptHndlr227:
SAVE_STATE_IRQ 0
mov rcx, 227
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr228
intrptHndlr228:
SAVE_STATE_IRQ 0
mov rcx, 228
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr229
intrptHndlr229:
SAVE_STATE_IRQ 0
mov rcx, 229
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr230
intrptHndlr230:
SAVE_STATE_IRQ 0
mov rcx, 230
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr231
intrptHndlr231:
SAVE_STATE_IRQ 0
mov rcx, 231
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr232
intrptHndlr232:
SAVE_STATE_IRQ 0
mov rcx, 232
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr233
intrptHndlr233:
SAVE_STATE_IRQ 0
mov rcx, 233
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr234
intrptHndlr234:
SAVE_STATE_IRQ 0
mov rcx, 234
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr235
intrptHndlr235:
SAVE_STATE_IRQ 0
mov rcx, 235
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr236
intrptHndlr236:
SAVE_STATE_IRQ 0
mov rcx, 236
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr237
intrptHndlr237:
SAVE_STATE_IRQ 0
mov rcx, 237
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr238
intrptHndlr238:
SAVE_STATE_IRQ 0
mov rcx, 238
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr239
intrptHndlr239:
SAVE_STATE_IRQ 0
mov rcx, 239
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr240
intrptHndlr240:
SAVE_STATE_IRQ 0
mov rcx, 240
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr241
intrptHndlr241:
SAVE_STATE_IRQ 0
mov rcx, 241
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr242
intrptHndlr242:
SAVE_STATE_IRQ 0
mov rcx, 242
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr243
intrptHndlr243:
SAVE_STATE_IRQ 0
mov rcx, 243
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr244
intrptHndlr244:
SAVE_STATE_IRQ 0
mov rcx, 244
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr245
intrptHndlr245:
SAVE_STATE_IRQ 0
mov rcx, 245
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr246
intrptHndlr246:
SAVE_STATE_IRQ 0
mov rcx, 246
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr247
intrptHndlr247:
SAVE_STATE_IRQ 0
mov rcx, 247
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr248
intrptHndlr248:
SAVE_STATE_IRQ 0
mov rcx, 248
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr249
intrptHndlr249:
SAVE_STATE_IRQ 0
mov rcx, 249
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr250
intrptHndlr250:
SAVE_STATE_IRQ 0
mov rcx, 250
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr251
intrptHndlr251:
SAVE_STATE_IRQ 0
mov rcx, 251
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr252
intrptHndlr252:
SAVE_STATE_IRQ 0
mov rcx, 252
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr253
intrptHndlr253:
SAVE_STATE_IRQ 0
mov rcx, 253
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr254
intrptHndlr254:
SAVE_STATE_IRQ 0
mov rcx, 254
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

global intrptHndlr255
intrptHndlr255:
SAVE_STATE_IRQ 0
mov rcx, 255
mov rdx, 0
sub rsp, 32
call interruptHandler
add rsp, 32
RESTORE_STATE_IRQ 0
iretq

section .data
tempErrorCode: dq 0