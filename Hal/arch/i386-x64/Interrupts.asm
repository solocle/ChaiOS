;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: Hal
;File: Interrupts.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\Interrupts.asm
;Created by Nathaniel on 14/7/2015 at 03:22
;
;Description: Assembly interrupt entry points
;**********************************************************/
BITS 32
segment .code

%macro SAVE_STATE_IRQ 1
push ebp
mov ebp, esp
pushad
;Load segments
push ds
push es
push gs
mov ax, 0x10
mov ds, ax
mov es, ax
;Kernel TLS could be loaded later on. Or interrupts don't use TLS
;Exception struct
%if %1
mov esi, [tempErrorCode]
push esi
push ebp
mov esi, esp
%else
mov esi, 0
push esi
push ebp
mov esi, esp
%endif
%endmacro

%macro RESTORE_STATE_IRQ 0
add esp, 8
pop gs
pop es
pop ds
popad
pop ebp
%endmacro

%macro SORT_ERROR_CODE 0
push eax
mov eax, [esp+8]
mov [tempErrorCode], eax
pop eax
add esp, 4
%endmacro

extern @interruptHandler@8

global @intrptHndlr0@0
@intrptHndlr0@0:
SAVE_STATE_IRQ 1
mov ecx, 0
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr1@0
@intrptHndlr1@0:
SAVE_STATE_IRQ 1
mov ecx, 1
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr2@0
@intrptHndlr2@0:
SAVE_STATE_IRQ 1
mov ecx, 2
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr3@0
@intrptHndlr3@0:
SAVE_STATE_IRQ 1
mov ecx, 3
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr4@0
@intrptHndlr4@0:
SAVE_STATE_IRQ 1
mov ecx, 4
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr5@0
@intrptHndlr5@0:
SAVE_STATE_IRQ 1
mov ecx, 5
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr6@0
@intrptHndlr6@0:
SAVE_STATE_IRQ 1
mov ecx, 6
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr7@0
@intrptHndlr7@0:
SAVE_STATE_IRQ 1
mov ecx, 7
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr8@0
@intrptHndlr8@0:
SAVE_STATE_IRQ 1
mov ecx, 8
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr9@0
@intrptHndlr9@0:
SAVE_STATE_IRQ 1
mov ecx, 9
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr10@0
@intrptHndlr10@0:
SAVE_STATE_IRQ 1
mov ecx, 10
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr11@0
@intrptHndlr11@0:
SAVE_STATE_IRQ 1
mov ecx, 11
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr12@0
@intrptHndlr12@0:
SAVE_STATE_IRQ 1
mov ecx, 12
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr13@0
@intrptHndlr13@0:
SAVE_STATE_IRQ 1
mov ecx, 13
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr14@0
@intrptHndlr14@0:
SAVE_STATE_IRQ 1
mov ecx, 14
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr15@0
@intrptHndlr15@0:
SAVE_STATE_IRQ 1
mov ecx, 15
mov edx, esi
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr16@0
@intrptHndlr16@0:
SAVE_STATE_IRQ 1
mov ecx, 16
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr17@0
@intrptHndlr17@0:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov ecx, 17
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr18@0
@intrptHndlr18@0:
SAVE_STATE_IRQ 1
mov ecx, 18
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr19@0
@intrptHndlr19@0:
SAVE_STATE_IRQ 1
mov ecx, 19
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr20@0
@intrptHndlr20@0:
SAVE_STATE_IRQ 1
mov ecx, 20
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr21@0
@intrptHndlr21@0:
SAVE_STATE_IRQ 1
mov ecx, 21
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr22@0
@intrptHndlr22@0:
SAVE_STATE_IRQ 1
mov ecx, 22
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr23@0
@intrptHndlr23@0:
SAVE_STATE_IRQ 1
mov ecx, 23
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr24@0
@intrptHndlr24@0:
SAVE_STATE_IRQ 1
mov ecx, 24
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr25@0
@intrptHndlr25@0:
SAVE_STATE_IRQ 1
mov ecx, 25
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr26@0
@intrptHndlr26@0:
SAVE_STATE_IRQ 1
mov ecx, 26
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr27@0
@intrptHndlr27@0:
SAVE_STATE_IRQ 1
mov ecx, 27
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr28@0
@intrptHndlr28@0:
SAVE_STATE_IRQ 1
mov ecx, 28
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr29@0
@intrptHndlr29@0:
SAVE_STATE_IRQ 1
mov ecx, 29
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr30@0
@intrptHndlr30@0:
SORT_ERROR_CODE
SAVE_STATE_IRQ 1
mov ecx, 30
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr31@0
@intrptHndlr31@0:
SAVE_STATE_IRQ 1
mov ecx, 31
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr32@0
@intrptHndlr32@0:
SAVE_STATE_IRQ 0
mov ecx, 32
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr33@0
@intrptHndlr33@0:
SAVE_STATE_IRQ 0
push edx
push ecx
push ebx
push eax
mov ecx, 33
lea edx, [esp]
call @interruptHandler@8
add esp, 0x10
RESTORE_STATE_IRQ
iret

global @intrptHndlr34@0
@intrptHndlr34@0:
SAVE_STATE_IRQ 0
mov ecx, 34
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr35@0
@intrptHndlr35@0:
SAVE_STATE_IRQ 0
mov ecx, 35
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr36@0
@intrptHndlr36@0:
SAVE_STATE_IRQ 0
mov ecx, 36
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr37@0
@intrptHndlr37@0:
SAVE_STATE_IRQ 0
mov ecx, 37
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr38@0
@intrptHndlr38@0:
SAVE_STATE_IRQ 0
mov ecx, 38
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr39@0
@intrptHndlr39@0:
SAVE_STATE_IRQ 0
mov ecx, 39
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr40@0
@intrptHndlr40@0:
SAVE_STATE_IRQ 0
mov ecx, 40
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr41@0
@intrptHndlr41@0:
SAVE_STATE_IRQ 0
mov ecx, 41
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr42@0
@intrptHndlr42@0:
SAVE_STATE_IRQ 0
mov ecx, 42
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr43@0
@intrptHndlr43@0:
SAVE_STATE_IRQ 0
mov ecx, 43
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr44@0
@intrptHndlr44@0:
SAVE_STATE_IRQ 0
mov ecx, 44
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr45@0
@intrptHndlr45@0:
SAVE_STATE_IRQ 0
mov ecx, 45
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr46@0
@intrptHndlr46@0:
SAVE_STATE_IRQ 0
mov ecx, 46
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr47@0
@intrptHndlr47@0:
SAVE_STATE_IRQ 0
mov ecx, 47
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr48@0
@intrptHndlr48@0:
SAVE_STATE_IRQ 0
mov ecx, 48
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr49@0
@intrptHndlr49@0:
SAVE_STATE_IRQ 0
mov ecx, 49
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr50@0
@intrptHndlr50@0:
SAVE_STATE_IRQ 0
mov ecx, 50
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr51@0
@intrptHndlr51@0:
SAVE_STATE_IRQ 0
mov ecx, 51
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr52@0
@intrptHndlr52@0:
SAVE_STATE_IRQ 0
mov ecx, 52
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr53@0
@intrptHndlr53@0:
SAVE_STATE_IRQ 0
mov ecx, 53
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr54@0
@intrptHndlr54@0:
SAVE_STATE_IRQ 0
mov ecx, 54
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr55@0
@intrptHndlr55@0:
SAVE_STATE_IRQ 0
mov ecx, 55
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr56@0
@intrptHndlr56@0:
SAVE_STATE_IRQ 0
mov ecx, 56
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr57@0
@intrptHndlr57@0:
SAVE_STATE_IRQ 0
mov ecx, 57
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr58@0
@intrptHndlr58@0:
SAVE_STATE_IRQ 0
mov ecx, 58
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr59@0
@intrptHndlr59@0:
SAVE_STATE_IRQ 0
mov ecx, 59
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr60@0
@intrptHndlr60@0:
SAVE_STATE_IRQ 0
mov ecx, 60
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr61@0
@intrptHndlr61@0:
SAVE_STATE_IRQ 0
mov ecx, 61
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr62@0
@intrptHndlr62@0:
SAVE_STATE_IRQ 0
mov ecx, 62
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr63@0
@intrptHndlr63@0:
SAVE_STATE_IRQ 0
mov ecx, 63
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr64@0
@intrptHndlr64@0:
SAVE_STATE_IRQ 0
mov ecx, 64
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr65@0
@intrptHndlr65@0:
SAVE_STATE_IRQ 0
mov ecx, 65
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr66@0
@intrptHndlr66@0:
SAVE_STATE_IRQ 0
mov ecx, 66
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr67@0
@intrptHndlr67@0:
SAVE_STATE_IRQ 0
mov ecx, 67
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr68@0
@intrptHndlr68@0:
SAVE_STATE_IRQ 0
mov ecx, 68
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr69@0
@intrptHndlr69@0:
SAVE_STATE_IRQ 0
mov ecx, 69
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr70@0
@intrptHndlr70@0:
SAVE_STATE_IRQ 0
mov ecx, 70
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr71@0
@intrptHndlr71@0:
SAVE_STATE_IRQ 0
mov ecx, 71
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr72@0
@intrptHndlr72@0:
SAVE_STATE_IRQ 0
mov ecx, 72
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr73@0
@intrptHndlr73@0:
SAVE_STATE_IRQ 0
mov ecx, 73
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr74@0
@intrptHndlr74@0:
SAVE_STATE_IRQ 0
mov ecx, 74
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr75@0
@intrptHndlr75@0:
SAVE_STATE_IRQ 0
mov ecx, 75
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr76@0
@intrptHndlr76@0:
SAVE_STATE_IRQ 0
mov ecx, 76
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr77@0
@intrptHndlr77@0:
SAVE_STATE_IRQ 0
mov ecx, 77
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr78@0
@intrptHndlr78@0:
SAVE_STATE_IRQ 0
mov ecx, 78
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr79@0
@intrptHndlr79@0:
SAVE_STATE_IRQ 0
mov ecx, 79
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr80@0
@intrptHndlr80@0:
SAVE_STATE_IRQ 0
mov ecx, 80
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr81@0
@intrptHndlr81@0:
SAVE_STATE_IRQ 0
mov ecx, 81
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr82@0
@intrptHndlr82@0:
SAVE_STATE_IRQ 0
mov ecx, 82
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr83@0
@intrptHndlr83@0:
SAVE_STATE_IRQ 0
mov ecx, 83
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr84@0
@intrptHndlr84@0:
SAVE_STATE_IRQ 0
mov ecx, 84
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr85@0
@intrptHndlr85@0:
SAVE_STATE_IRQ 0
mov ecx, 85
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr86@0
@intrptHndlr86@0:
SAVE_STATE_IRQ 0
mov ecx, 86
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr87@0
@intrptHndlr87@0:
SAVE_STATE_IRQ 0
mov ecx, 87
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr88@0
@intrptHndlr88@0:
SAVE_STATE_IRQ 0
mov ecx, 88
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr89@0
@intrptHndlr89@0:
SAVE_STATE_IRQ 0
mov ecx, 89
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr90@0
@intrptHndlr90@0:
SAVE_STATE_IRQ 0
mov ecx, 90
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr91@0
@intrptHndlr91@0:
SAVE_STATE_IRQ 0
mov ecx, 91
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr92@0
@intrptHndlr92@0:
SAVE_STATE_IRQ 0
mov ecx, 92
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr93@0
@intrptHndlr93@0:
SAVE_STATE_IRQ 0
mov ecx, 93
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr94@0
@intrptHndlr94@0:
SAVE_STATE_IRQ 0
mov ecx, 94
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr95@0
@intrptHndlr95@0:
SAVE_STATE_IRQ 0
mov ecx, 95
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr96@0
@intrptHndlr96@0:
SAVE_STATE_IRQ 0
mov ecx, 96
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr97@0
@intrptHndlr97@0:
SAVE_STATE_IRQ 0
mov ecx, 97
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr98@0
@intrptHndlr98@0:
SAVE_STATE_IRQ 0
mov ecx, 98
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr99@0
@intrptHndlr99@0:
SAVE_STATE_IRQ 0
mov ecx, 99
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr100@0
@intrptHndlr100@0:
SAVE_STATE_IRQ 0
mov ecx, 100
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr101@0
@intrptHndlr101@0:
SAVE_STATE_IRQ 0
mov ecx, 101
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr102@0
@intrptHndlr102@0:
SAVE_STATE_IRQ 0
mov ecx, 102
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr103@0
@intrptHndlr103@0:
SAVE_STATE_IRQ 0
mov ecx, 103
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr104@0
@intrptHndlr104@0:
SAVE_STATE_IRQ 0
mov ecx, 104
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr105@0
@intrptHndlr105@0:
SAVE_STATE_IRQ 0
push edx
push ecx
push ebx
push eax
mov ecx, 105
lea edx, [esp]
call @interruptHandler@8
add esp, 0x10
RESTORE_STATE_IRQ
iret

global @intrptHndlr106@0
@intrptHndlr106@0:
SAVE_STATE_IRQ 0
mov ecx, 106
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr107@0
@intrptHndlr107@0:
SAVE_STATE_IRQ 0
mov ecx, 107
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr108@0
@intrptHndlr108@0:
SAVE_STATE_IRQ 0
mov ecx, 108
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr109@0
@intrptHndlr109@0:
SAVE_STATE_IRQ 0
mov ecx, 109
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr110@0
@intrptHndlr110@0:
SAVE_STATE_IRQ 0
mov ecx, 110
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr111@0
@intrptHndlr111@0:
SAVE_STATE_IRQ 0
mov ecx, 111
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr112@0
@intrptHndlr112@0:
SAVE_STATE_IRQ 0
mov ecx, 112
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr113@0
@intrptHndlr113@0:
SAVE_STATE_IRQ 0
mov ecx, 113
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr114@0
@intrptHndlr114@0:
SAVE_STATE_IRQ 0
mov ecx, 114
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr115@0
@intrptHndlr115@0:
SAVE_STATE_IRQ 0
mov ecx, 115
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr116@0
@intrptHndlr116@0:
SAVE_STATE_IRQ 0
mov ecx, 116
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr117@0
@intrptHndlr117@0:
SAVE_STATE_IRQ 0
mov ecx, 117
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr118@0
@intrptHndlr118@0:
SAVE_STATE_IRQ 0
mov ecx, 118
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr119@0
@intrptHndlr119@0:
SAVE_STATE_IRQ 0
mov ecx, 119
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr120@0
@intrptHndlr120@0:
SAVE_STATE_IRQ 0
mov ecx, 120
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr121@0
@intrptHndlr121@0:
SAVE_STATE_IRQ 0
mov ecx, 121
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr122@0
@intrptHndlr122@0:
SAVE_STATE_IRQ 0
mov ecx, 122
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr123@0
@intrptHndlr123@0:
SAVE_STATE_IRQ 0
mov ecx, 123
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr124@0
@intrptHndlr124@0:
SAVE_STATE_IRQ 0
mov ecx, 124
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr125@0
@intrptHndlr125@0:
SAVE_STATE_IRQ 0
mov ecx, 125
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr126@0
@intrptHndlr126@0:
SAVE_STATE_IRQ 0
mov ecx, 126
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr127@0
@intrptHndlr127@0:
SAVE_STATE_IRQ 0
mov ecx, 127
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr128@0
@intrptHndlr128@0:
SAVE_STATE_IRQ 0
mov ecx, 128
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr129@0
@intrptHndlr129@0:
SAVE_STATE_IRQ 0
mov ecx, 129
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr130@0
@intrptHndlr130@0:
SAVE_STATE_IRQ 0
mov ecx, 130
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr131@0
@intrptHndlr131@0:
SAVE_STATE_IRQ 0
mov ecx, 131
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr132@0
@intrptHndlr132@0:
SAVE_STATE_IRQ 0
mov ecx, 132
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr133@0
@intrptHndlr133@0:
SAVE_STATE_IRQ 0
mov ecx, 133
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr134@0
@intrptHndlr134@0:
SAVE_STATE_IRQ 0
mov ecx, 134
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr135@0
@intrptHndlr135@0:
SAVE_STATE_IRQ 0
mov ecx, 135
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr136@0
@intrptHndlr136@0:
SAVE_STATE_IRQ 0
mov ecx, 136
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr137@0
@intrptHndlr137@0:
SAVE_STATE_IRQ 0
mov ecx, 137
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr138@0
@intrptHndlr138@0:
SAVE_STATE_IRQ 0
mov ecx, 138
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr139@0
@intrptHndlr139@0:
SAVE_STATE_IRQ 0
mov ecx, 139
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr140@0
@intrptHndlr140@0:
SAVE_STATE_IRQ 0
mov ecx, 140
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr141@0
@intrptHndlr141@0:
SAVE_STATE_IRQ 0
mov ecx, 141
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr142@0
@intrptHndlr142@0:
SAVE_STATE_IRQ 0
mov ecx, 142
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr143@0
@intrptHndlr143@0:
SAVE_STATE_IRQ 0
mov ecx, 143
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr144@0
@intrptHndlr144@0:
SAVE_STATE_IRQ 0
mov ecx, 144
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr145@0
@intrptHndlr145@0:
SAVE_STATE_IRQ 0
mov ecx, 145
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr146@0
@intrptHndlr146@0:
SAVE_STATE_IRQ 0
mov ecx, 146
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr147@0
@intrptHndlr147@0:
SAVE_STATE_IRQ 0
mov ecx, 147
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr148@0
@intrptHndlr148@0:
SAVE_STATE_IRQ 0
mov ecx, 148
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr149@0
@intrptHndlr149@0:
SAVE_STATE_IRQ 0
mov ecx, 149
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr150@0
@intrptHndlr150@0:
SAVE_STATE_IRQ 0
mov ecx, 150
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr151@0
@intrptHndlr151@0:
SAVE_STATE_IRQ 0
mov ecx, 151
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr152@0
@intrptHndlr152@0:
SAVE_STATE_IRQ 0
mov ecx, 152
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr153@0
@intrptHndlr153@0:
SAVE_STATE_IRQ 0
mov ecx, 153
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr154@0
@intrptHndlr154@0:
SAVE_STATE_IRQ 0
mov ecx, 154
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr155@0
@intrptHndlr155@0:
SAVE_STATE_IRQ 0
mov ecx, 155
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr156@0
@intrptHndlr156@0:
SAVE_STATE_IRQ 0
mov ecx, 156
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr157@0
@intrptHndlr157@0:
SAVE_STATE_IRQ 0
mov ecx, 157
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr158@0
@intrptHndlr158@0:
SAVE_STATE_IRQ 0
mov ecx, 158
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr159@0
@intrptHndlr159@0:
SAVE_STATE_IRQ 0
mov ecx, 159
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr160@0
@intrptHndlr160@0:
SAVE_STATE_IRQ 0
mov ecx, 160
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr161@0
@intrptHndlr161@0:
SAVE_STATE_IRQ 0
mov ecx, 161
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr162@0
@intrptHndlr162@0:
SAVE_STATE_IRQ 0
mov ecx, 162
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr163@0
@intrptHndlr163@0:
SAVE_STATE_IRQ 0
mov ecx, 163
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr164@0
@intrptHndlr164@0:
SAVE_STATE_IRQ 0
mov ecx, 164
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr165@0
@intrptHndlr165@0:
SAVE_STATE_IRQ 0
mov ecx, 165
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr166@0
@intrptHndlr166@0:
SAVE_STATE_IRQ 0
mov ecx, 166
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr167@0
@intrptHndlr167@0:
SAVE_STATE_IRQ 0
mov ecx, 167
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr168@0
@intrptHndlr168@0:
SAVE_STATE_IRQ 0
mov ecx, 168
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr169@0
@intrptHndlr169@0:
SAVE_STATE_IRQ 0
mov ecx, 169
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr170@0
@intrptHndlr170@0:
SAVE_STATE_IRQ 0
mov ecx, 170
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr171@0
@intrptHndlr171@0:
SAVE_STATE_IRQ 0
mov ecx, 171
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr172@0
@intrptHndlr172@0:
SAVE_STATE_IRQ 0
mov ecx, 172
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr173@0
@intrptHndlr173@0:
SAVE_STATE_IRQ 0
mov ecx, 173
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr174@0
@intrptHndlr174@0:
SAVE_STATE_IRQ 0
mov ecx, 174
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr175@0
@intrptHndlr175@0:
SAVE_STATE_IRQ 0
mov ecx, 175
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr176@0
@intrptHndlr176@0:
SAVE_STATE_IRQ 0
mov ecx, 176
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr177@0
@intrptHndlr177@0:
SAVE_STATE_IRQ 0
mov ecx, 177
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr178@0
@intrptHndlr178@0:
SAVE_STATE_IRQ 0
mov ecx, 178
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr179@0
@intrptHndlr179@0:
SAVE_STATE_IRQ 0
mov ecx, 179
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr180@0
@intrptHndlr180@0:
SAVE_STATE_IRQ 0
mov ecx, 180
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr181@0
@intrptHndlr181@0:
SAVE_STATE_IRQ 0
mov ecx, 181
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr182@0
@intrptHndlr182@0:
SAVE_STATE_IRQ 0
mov ecx, 182
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr183@0
@intrptHndlr183@0:
SAVE_STATE_IRQ 0
mov ecx, 183
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr184@0
@intrptHndlr184@0:
SAVE_STATE_IRQ 0
mov ecx, 184
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr185@0
@intrptHndlr185@0:
SAVE_STATE_IRQ 0
mov ecx, 185
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr186@0
@intrptHndlr186@0:
SAVE_STATE_IRQ 0
mov ecx, 186
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr187@0
@intrptHndlr187@0:
SAVE_STATE_IRQ 0
mov ecx, 187
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr188@0
@intrptHndlr188@0:
SAVE_STATE_IRQ 0
mov ecx, 188
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr189@0
@intrptHndlr189@0:
SAVE_STATE_IRQ 0
mov ecx, 189
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr190@0
@intrptHndlr190@0:
SAVE_STATE_IRQ 0
mov ecx, 190
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr191@0
@intrptHndlr191@0:
SAVE_STATE_IRQ 0
mov ecx, 191
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr192@0
@intrptHndlr192@0:
SAVE_STATE_IRQ 0
mov ecx, 192
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr193@0
@intrptHndlr193@0:
SAVE_STATE_IRQ 0
mov ecx, 193
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr194@0
@intrptHndlr194@0:
SAVE_STATE_IRQ 0
mov ecx, 194
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr195@0
@intrptHndlr195@0:
SAVE_STATE_IRQ 0
mov ecx, 195
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr196@0
@intrptHndlr196@0:
SAVE_STATE_IRQ 0
mov ecx, 196
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr197@0
@intrptHndlr197@0:
SAVE_STATE_IRQ 0
mov ecx, 197
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr198@0
@intrptHndlr198@0:
SAVE_STATE_IRQ 0
mov ecx, 198
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr199@0
@intrptHndlr199@0:
SAVE_STATE_IRQ 0
mov ecx, 199
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr200@0
@intrptHndlr200@0:
SAVE_STATE_IRQ 0
mov ecx, 200
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr201@0
@intrptHndlr201@0:
SAVE_STATE_IRQ 0
mov ecx, 201
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr202@0
@intrptHndlr202@0:
SAVE_STATE_IRQ 0
mov ecx, 202
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr203@0
@intrptHndlr203@0:
SAVE_STATE_IRQ 0
mov ecx, 203
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr204@0
@intrptHndlr204@0:
SAVE_STATE_IRQ 0
mov ecx, 204
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr205@0
@intrptHndlr205@0:
SAVE_STATE_IRQ 0
mov ecx, 205
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr206@0
@intrptHndlr206@0:
SAVE_STATE_IRQ 0
mov ecx, 206
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr207@0
@intrptHndlr207@0:
SAVE_STATE_IRQ 0
mov ecx, 207
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr208@0
@intrptHndlr208@0:
SAVE_STATE_IRQ 0
mov ecx, 208
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr209@0
@intrptHndlr209@0:
SAVE_STATE_IRQ 0
mov ecx, 209
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr210@0
@intrptHndlr210@0:
SAVE_STATE_IRQ 0
mov ecx, 210
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr211@0
@intrptHndlr211@0:
SAVE_STATE_IRQ 0
mov ecx, 211
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr212@0
@intrptHndlr212@0:
SAVE_STATE_IRQ 0
mov ecx, 212
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr213@0
@intrptHndlr213@0:
SAVE_STATE_IRQ 0
mov ecx, 213
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr214@0
@intrptHndlr214@0:
SAVE_STATE_IRQ 0
mov ecx, 214
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr215@0
@intrptHndlr215@0:
SAVE_STATE_IRQ 0
mov ecx, 215
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr216@0
@intrptHndlr216@0:
SAVE_STATE_IRQ 0
mov ecx, 216
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr217@0
@intrptHndlr217@0:
SAVE_STATE_IRQ 0
mov ecx, 217
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr218@0
@intrptHndlr218@0:
SAVE_STATE_IRQ 0
mov ecx, 218
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr219@0
@intrptHndlr219@0:
SAVE_STATE_IRQ 0
mov ecx, 219
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr220@0
@intrptHndlr220@0:
SAVE_STATE_IRQ 0
mov ecx, 220
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr221@0
@intrptHndlr221@0:
SAVE_STATE_IRQ 0
mov ecx, 221
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr222@0
@intrptHndlr222@0:
SAVE_STATE_IRQ 0
mov ecx, 222
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr223@0
@intrptHndlr223@0:
SAVE_STATE_IRQ 0
mov ecx, 223
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr224@0
@intrptHndlr224@0:
SAVE_STATE_IRQ 0
mov ecx, 224
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr225@0
@intrptHndlr225@0:
SAVE_STATE_IRQ 0
mov ecx, 225
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr226@0
@intrptHndlr226@0:
SAVE_STATE_IRQ 0
mov ecx, 226
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr227@0
@intrptHndlr227@0:
SAVE_STATE_IRQ 0
mov ecx, 227
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr228@0
@intrptHndlr228@0:
SAVE_STATE_IRQ 0
mov ecx, 228
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr229@0
@intrptHndlr229@0:
SAVE_STATE_IRQ 0
mov ecx, 229
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr230@0
@intrptHndlr230@0:
SAVE_STATE_IRQ 0
mov ecx, 230
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr231@0
@intrptHndlr231@0:
SAVE_STATE_IRQ 0
mov ecx, 231
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr232@0
@intrptHndlr232@0:
SAVE_STATE_IRQ 0
mov ecx, 232
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr233@0
@intrptHndlr233@0:
SAVE_STATE_IRQ 0
mov ecx, 233
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr234@0
@intrptHndlr234@0:
SAVE_STATE_IRQ 0
mov ecx, 234
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr235@0
@intrptHndlr235@0:
SAVE_STATE_IRQ 0
mov ecx, 235
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr236@0
@intrptHndlr236@0:
SAVE_STATE_IRQ 0
mov ecx, 236
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr237@0
@intrptHndlr237@0:
SAVE_STATE_IRQ 0
mov ecx, 237
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr238@0
@intrptHndlr238@0:
SAVE_STATE_IRQ 0
mov ecx, 238
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr239@0
@intrptHndlr239@0:
SAVE_STATE_IRQ 0
mov ecx, 239
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr240@0
@intrptHndlr240@0:
SAVE_STATE_IRQ 0
mov ecx, 240
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr241@0
@intrptHndlr241@0:
SAVE_STATE_IRQ 0
mov ecx, 241
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr242@0
@intrptHndlr242@0:
SAVE_STATE_IRQ 0
mov ecx, 242
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr243@0
@intrptHndlr243@0:
SAVE_STATE_IRQ 0
mov ecx, 243
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr244@0
@intrptHndlr244@0:
SAVE_STATE_IRQ 0
mov ecx, 244
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr245@0
@intrptHndlr245@0:
SAVE_STATE_IRQ 0
mov ecx, 245
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr246@0
@intrptHndlr246@0:
SAVE_STATE_IRQ 0
mov ecx, 246
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr247@0
@intrptHndlr247@0:
SAVE_STATE_IRQ 0
mov ecx, 247
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr248@0
@intrptHndlr248@0:
SAVE_STATE_IRQ 0
mov ecx, 248
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr249@0
@intrptHndlr249@0:
SAVE_STATE_IRQ 0
mov ecx, 249
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr250@0
@intrptHndlr250@0:
SAVE_STATE_IRQ 0
mov ecx, 250
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr251@0
@intrptHndlr251@0:
SAVE_STATE_IRQ 0
mov ecx, 251
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr252@0
@intrptHndlr252@0:
SAVE_STATE_IRQ 0
mov ecx, 252
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr253@0
@intrptHndlr253@0:
SAVE_STATE_IRQ 0
mov ecx, 253
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr254@0
@intrptHndlr254@0:
SAVE_STATE_IRQ 0
mov ecx, 254
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

global @intrptHndlr255@0
@intrptHndlr255@0:
SAVE_STATE_IRQ 0
mov ecx, 255
mov edx, 0
call @interruptHandler@8
RESTORE_STATE_IRQ
iret

section .data
tempErrorCode: dd 0