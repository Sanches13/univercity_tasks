;Вывод двоичного представления числа
section .bss
 char resb 4
section .data
 msg_1 db '1'
 len_1 equ $ - msg_1
 msg_2 db '0'
 len_2 equ $ - msg_2

section .text:
 global _start
_start:

 mov eax, 0x0F0F0F0F

 mov ecx, 32
point:
 push ecx
 rol eax, 1
 jc point_2

 mov [char], eax
 mov eax, 4
 mov ebx, 0
 mov ecx, msg_2
 mov edx, len_2
 int 0x80

 jmp point_3

point_2:
 mov [char], eax
 mov eax, 4
 mov ebx, 1
 mov ecx, msg_1
 mov edx, len_1
 int 0x80

point_3:
 mov eax, [char]
 pop ecx
 loop point

 mov eax, 1
 mov ebx, 0
 int 0x80

