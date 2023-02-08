section .bss
 fd resb 4
 msg resb 100
 char resb 1
 ind_gamma resb 4
 ind_msg resb 4

section .data
 message db 'Enter your string', 0xA
 len equ $ - message
 gamma db '1234'

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

 mov eax, 0  
 mov [ind_gamma], eax
 mov [ind_msg], eax

 mov eax, 4
 mov ebx, 1
 mov ecx, message
 mov edx, len
 int 0x80

 mov eax, 3
 mov ebx, 0 
 mov ecx, msg
 mov edx, 100
 int 0x80

point:
 mov esi, gamma
 mov edi, msg
 
 mov eax, [ind_gamma]
 add esi, eax
 mov bl, [esi]

 mov eax, [ind_msg]
 add edi, eax
 mov al, [edi]
 cmp al, 0x0A
 je exit

 xor al, bl  
 mov [char], al

 mov eax, 4
 mov ebx, [fd]
 mov ecx, char
 mov edx, 1
 int 0x80

 mov eax, [ind_msg]
 inc eax
 cmp eax, 100
 je exit
 mov [ind_msg], eax

 mov eax, [ind_gamma]
 inc eax
 cmp eax, 4
 je point2
 mov [ind_gamma], eax
 jmp point

point2:
 mov eax, 0
 mov [ind_gamma], eax
 jmp point
 
exit:
 mov eax, 6
 mov ebx, [fd]
 int 0x80

 mov eax, 1
 mov ecx, 0
 int 0x80
