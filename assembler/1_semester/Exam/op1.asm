;Получение и вывод на экран размера файла
section .bss
 fd resb 4
 char resb 4
 size resb 4
 number resb 16
 ind resb 4

section .data
 msg_1 db 'File size: ', 0
 len_1 equ $ - msg_1
 msg_2 db ' byte', 0xA
 len_2 equ $ - msg_2

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
 mov [size], eax
 mov [ind], eax
 
 mov eax, 4
 mov ebx, 1
 mov ecx, msg_1
 mov edx, len_1
 int 0x80

point_1:
 mov eax, 3
 mov ebx, [fd]
 mov ecx, char
 mov edx, 1
 int 0x80

 cmp eax, 0
 je point_2

 mov eax, [size]
 inc eax
 mov [size], eax
 jmp point_1

point_2:
 mov eax, [size]
 cmp eax, 0
 je point_3

 xor edx, edx
 mov ebx, 10
 div ebx

 mov [size], eax
 add edx, 0x30
 mov esi, number
 mov eax, [ind]
 add esi, eax
 mov [esi], edx
 
 inc eax
 mov [ind], eax
 jmp point_2

point_3:
 mov esi, number
 mov eax, [ind]
 add esi, eax
 lodsb
 mov [char], al

 mov eax, 4
 mov ebx, 1
 mov ecx, char
 mov edx, 1
 int 0x80

 mov eax, [ind]
 dec eax 
 cmp eax, 0
 jnge exit
 mov [ind], eax
 jmp point_3

exit:
 mov eax, 4
 mov ebx, 1
 mov ecx, msg_2
 mov edx, len_2
 int 0x80
 
 mov eax, 6
 mov ebx, [fd]
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80 
