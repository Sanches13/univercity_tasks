;замена символов файла по подстановке
section .bss
 fd resb 4
 sym resb 1

section .data
 subst db '9', '6', '0', '2', '8', '4', '5', '1', '3', '7'
section .text:
 global _start
_start:
 
 pop ebx
 pop ebx
 pop ebx
 
 mov eax, 5
 mov ecx, 2
 mov edx, 0
 int 0x80 
 mov [fd], eax

point:
 mov eax, 3
 mov ebx, [fd]
 mov ecx, sym
 mov edx, 1
 int 0x80

 cmp eax, 0
 je exit

 mov ebx, subst
 mov al, [sym]

 cmp al, 0x0A
 je exit

 sub al, 0x30
 xlatb

 ;add al, 0x30
 mov [sym], al

 mov eax, 19
 mov ebx, [fd]
 mov ecx, -1
 mov edx, 1
 int 0x80

 mov eax, 4
 mov ebx, [fd]
 mov ecx, sym
 mov edx, 1
 int 0x80

 jmp point

exit:
 mov eax, 6
 mov ebx, [fd]
 int 0x80
 
 mov eax, 1
 mov ebx, 0
 int 0x80
