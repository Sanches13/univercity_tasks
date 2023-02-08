;Подсчёт количества повторения символа  в файле
section .bss
 fd resb 4
 ind resb 4
 number resb 16
 char resb 4
 x resb 1
 sum resb 4

section .text:
 global _start
_start:
 pop ebx
 pop ebx
 pop ebx
 mov [char], ebx
 pop ebx

 mov eax, 5
 mov ecx, 0
 mov edx, 0
 int 0x80
 mov [fd], eax

 mov eax, 0
 mov [ind], eax
 mov [sum], eax

point_1:
 mov eax, 3
 mov ebx, [fd] 
 mov ecx, x
 mov edx, 1 
 int 0x80

 cmp eax, 0
 je point_2

 mov esi, [char]
 lodsb
 mov bl, [x]
 cmp al, bl
 je equal

 jmp point_1

point_2:
 mov eax, [sum]
 cmp eax, 0
 je point_3

 xor edx, edx
 mov ebx, 10
 div ebx

 mov [sum], eax
 add edx, 0x30
 mov edi, number
 mov eax, [ind]
 add edi, eax
 mov [edi], edx
 inc eax
 mov [ind], eax

 jmp point_2

point_3:
 mov eax, [ind]
 cmp eax, 0
 jnge exit
 
 mov edi, number
 add edi, eax
 
 mov al, [edi]
 mov [x], al

 mov eax, 4
 mov ebx, 1
 mov ecx, x
 mov edx, 1
 int 0x80

 mov eax, [ind]
 dec eax
 mov [ind], eax
 jmp point_3

exit:
 mov eax, 6
 mov ebx, [fd]
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80

equal:
 mov eax, [sum]
 inc eax
 mov [sum], eax
 jmp point_1 
