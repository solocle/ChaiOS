;/********************************************************** 
;ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
;Licensed under the ChaiOS License
;See License for full details
;
;Project: ChaiOS
;File: setvesamode.asm
;Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\setvesamode.asm
;Created by Nathaniel on 26/6/2015 at 18:34
;
;Description: VESA Mode Setter
;**********************************************************/

%include "chaimodule16.asm"

header 0x1000, "VESAVBE"
modentry:
push bx		;width
push cx		;mode
push dx		;height
push si
mov DWORD[maxModeBytes], 0
mov ax, ds
mov es, ax
lea di, [controllerBuffer]
mov ax, 0x4F00
int 10h
cmp ah, 0
jne .fail
cmp al, 0x4F
jne .fail
mov bx, [controllerBuffer+0x0E]
mov cx, [controllerBuffer+0x10]
mov fs, cx
cmp DWORD[iniFileParams.inispecified], 0
jne .altloop

.loop:
mov ax, [fs:bx]
cmp ax, 0xFFFF
je .next
mov cx, ax
lea di, [modeInfo]
mov ax, 0x4F01
int 0x10
mov ax, [es:di]
test ax, 1 << 4
jz .eof_loop
cmp BYTE[es:di+0x1B], 0x06
jne .eof_loop
xor eax, eax
mov ax, [es:di+0x12]
xor edx, edx
mov dx, [es:di+0x14]
cmp eax, 720
je .cmpy
cmp eax, 640
jne .eof_loop
.cmpy:
cmp edx, 480
jne .eof_loop
mul edx
mov [maxModeBytes], eax
mov [maxMode], cx
.eof_loop:
add bx, 2
jmp .loop

.altloop:
mov ax, [fs:bx]
cmp ax, 0xFFFF
je .next
mov cx, ax
lea di, [modeInfo]
mov ax, 0x4F01
int 0x10
mov ax, [es:di]
test ax, 1 << 4
jz .eof_loop2
cmp BYTE[es:di+0x1B], 0x06
jne .eof_loop2
xor eax, eax
mov ax, [es:di+0x12]
xor edx, edx
mov dx, [es:di+0x14]
xor esi, esi
mov si, [es:di+0x19]
cmp eax, [iniFileParams.x]
jne .eof_loop2

cmp edx, [iniFileParams.y]
jne .eof_loop2
.cmpbpp:
cmp esi, [iniFileParams.bpp]
jne .eof_loop2
mov [maxMode], ax
jmp .next
.eof_loop2:
add bx, 2
jmp .altloop

.next:
mov bx, [maxMode]
or bx, 0x4000
mov ax, 0x4F02
int 0x10
;Also store mode info for later use
lea di, [modeInfo]
mov cx, [maxMode]
mov ax, 0x4F01
int 0x10
pop si
pop dx
pop dx
pop dx
ret
.fail:
cli
hlt
ret

section .data
controllerBuffer: TIMES 512 db 0

modeInfo: TIMES 256 db 0

iniFileParams:
.inispecified: dd 0
.x: dd 0
.y: dd 0
.bpp: dd 0

maxMode: dw 0
maxModeBytes: dq 0

section .text

%macro func_prologue_32 0
;Get desired VBE
mov eax, 2
lea ebx, [iniFileParams]
int 0x69
%endmacro

%macro func_prologue_64 0
;Get desired VBE
mov rax, 2
lea rbx, [iniFileParams]
int 0x69
%endmacro

%macro func_epilogue_32 0
;Register VBE info
mov eax, 0
lea ebx, [modeInfo]
int 0x69
%endmacro

%macro func_epilogue_64 0
;Register VBE info
mov rax, 0
lea rbx, [modeInfo]
int 0x69
%endmacro

footer