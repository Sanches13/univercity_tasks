;ДЗ5 номер 4
section .bss
 msg resb 100
 x resb 1
 ind_msg resb 4
 ind_gamma resb 4

section .data
 gamma db '1234'

section .text:
 global _start
_start:
 
 mov eax, 3
 mov ebx, 0
 mov ecx, msg
 mov edx, 101
 int 0x80

 mov eax, 0
 mov [ind_msg], eax
 mov [ind_gamma], eax

point:
 mov esi, msg
 mov edi, gamma

 mov eax, [ind_gamma]   
 add edi, eax
 mov bl, [edi]

 mov eax, [ind_msg]
 cmp eax, 100
 je exit
 add esi, eax
 mov al, [esi]
 cmp al, 0x0A
 je exit
 
 xor al, bl
 mov [x], al
 
 mov eax, 4
 mov ebx, 1
 mov ecx, x
 mov edx, 1
 int 0x80

 mov eax, [ind_msg]
 inc eax
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
 mov eax, 1
 mov ebx, 0
 int 0x80

