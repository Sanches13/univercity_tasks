section .bss
 fd resb 4
 char resb 1

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
 mov ecx, char
 mov edx, 1
 int 0x80

 cmp eax, 0 
 je exit

 rol byte [char], 4
 
 mov eax, 19
 mov ebx, [fd]
 mov ecx, -1
 mov edx, 1
 int 0x80

 mov eax, 4
 mov ebx, [fd]
 mov ecx, char
 mov edx, 1
 int 0x80

 jmp point 

exit:
 mov eax, 1
 mov ebx, 0 
 int 0x80
