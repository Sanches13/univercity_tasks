section .bss
 msg resb 32

section .text:
 global _start
_start:

 lea edi, [msg]
 mov ecx, 32
 mov al, '@'
 repne stosb

 mov eax, 4
 mov ebx, 1
 mov ecx, msg
 mov edx, 32
 int 0x80

 mov eax, 1
 mov ebx, 0 
 int 0x80
