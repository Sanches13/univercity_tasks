section .data:
 str1 db '12345', 0
 str2 db '12345', 0
 msg1 db 'ravno', 0
 len1 equ $ - msg1
 msg2 db 'ne ravno', 0
 len2 equ $ - msg2

section .text:
 global _start
_start:

 mov esi, str1
 mov edi, str2
 mov ecx, 5
 cld
 repe cmpsb
 jecxz equal

 mov eax, 4
 mov ebx, 1
 mov ecx, msg2
 mov edx, len2
 int 0x80
 jmp exit

equal:
 mov eax, 4
 mov ebx, 1
 mov ecx, msg1
 mov edx, len1
 int 0x80

exit:
 mov eax, 1
 mov ebx, 0
 int 0x80
