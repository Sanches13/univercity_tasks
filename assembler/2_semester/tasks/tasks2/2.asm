section .bss
 char resb 1
 ones resb 1
 zeros resb 1

section .data
 msg_1 db 'Ones more than zeros', 0xA
 len_1 equ $ - msg_1
 msg_2 db 'Zeros more than ones', 0xA
 len_2 equ $ - msg_2
 msg_3 db 'Ones and zeros equally', 0xA
 len_3 equ $ - msg_3

section .text:
 global _start
_start:

 mov al, 0
 mov [ones], al
 mov [zeros], al
 
 mov edx, 0b11110000101010101110100001010100

 mov ecx, 32
point:
 push ecx
 bt edx, ecx
 jc point2

 inc byte [zeros]
 pop ecx
 loop point

 mov al, [zeros]
 mov bl, [ones]
 cmp al, bl
 jl point3
 
 cmp al, bl
 je point4

 mov eax, 4
 mov ebx, 1
 mov ecx, msg_2
 mov edx, len_2
 int 0x80

exit:
 mov eax, 1
 mov ebx, 0
 int 0x80

point2:
 inc byte [ones]
 pop ecx
 loop point

point3:
 mov eax, 4
 mov ebx, 1
 mov ecx, msg_1
 mov edx, len_1
 int 0x80
 jmp exit

point4:
 mov eax, 4
 mov ebx, 1
 mov ecx, msg_3
 mov edx, len_3
 int 0x80

