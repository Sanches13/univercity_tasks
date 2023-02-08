section .bss
 char resb 1

section .data
 msg db 'Number', 0
 len equ $ - msg

section .text:
 global _start
_start:

point:
 mov eax, 3
 mov ebx, 0
 mov ecx, char
 mov edx, 1
 int 0x80

 cmp byte [char], 0x30
 jl point

 cmp byte [char], 0x39
 jg point

 mov eax, 4
 mov ebx, 1
 mov ecx, msg
 mov edx, len 
 int 0x80
  
 jmp point

 mov eax, 1
 mov ebx, 0 
 int 0x80
