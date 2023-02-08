%include "mac.inc"

section .bss
 char resb 1

section .text:
 global _start
_start:
 
 mov byte [char], 'N'
 print char
 mov eax, 1
 mov ebx, 0
 int 0x80
