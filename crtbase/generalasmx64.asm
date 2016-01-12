BITS 64
segment .text

global fpu_setup
fpu_setup:
fninit
ret

global load_taskreg
load_taskreg:
mov ax, cx
ltr ax
ret

global load_cs
load_cs:
pop rax	;Return address
mov rdx, rsp
mov r8w, ds
push r8w
push rdx	;This is the stack as we left it
pushfq
push cx		;Segment
push rax	;Address
db 0x48
iretq

global load_ds
load_ds:
mov ds, cx
mov es, cx
mov ss, cx
ret

global interrupt
interrupt:
mov al, cl
cmp al, 3
jne .normalint
int 3
.normalint:
mov rbx, .actInt
add rbx, 1
mov [rbx], al
.actInt
int 0xFF
ret

global setjmp
setjmp:
mov [rcx], rbx
mov [rcx+8], rsi
mov [rcx+16], rdi
ret

global longjmp
longjmp:
ret