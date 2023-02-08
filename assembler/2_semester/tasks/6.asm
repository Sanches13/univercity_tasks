;Строка из входных параметров в файл
section .bss
 msg resb 4
 fname resb 4
 fd resb 4
 sym resb 1

section .data
 char db 0x0A, 0
section .text:
 global _start
_start:

 pop ebx
 pop ebx
 pop ebx
 mov [msg], ebx
 pop ebx
 mov [fname], ebx
 
 mov eax, 8
 mov ecx, 420
 int 0x80
 mov [fd], eax

point:
 mov esi, [msg]
 mov edi, [fname]
 cmp edi, esi
 je exit
 lodsb

 mov [sym], al
 mov [msg], esi
 
 mov eax, 4
 mov ebx, [fd]
 mov ecx, sym
 mov edx, 1
 int 0x80

 jmp point

exit:
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

 mov eax, 6
 mov ebx, [fd]
 int 0x80 

 mov eax, 1
 mov ebx, 0 
 int 0x80
