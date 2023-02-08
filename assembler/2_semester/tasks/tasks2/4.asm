section .bss
 msg resb 10

section .text
 global _start
_start:

 mov eax, 3
 mov ebx, 0
 mov ecx, msg
 mov edx, 10
 int 0x80

 mov byte [msg], 'A'
 mov byte [msg+1], 'B'

 mov eax, 4
 mov ebx, 1
 mov ecx, msg
 mov edx, 10 
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
