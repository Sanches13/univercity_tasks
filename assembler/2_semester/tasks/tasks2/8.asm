;Замена одного символа на другой
section .bss
 fd resb 4
 char resb 1
 orig resb 4
 new resb 4

section .text:
 global _start
_start:
 pop ebx
 pop ebx
 pop ebx
 mov [orig], ebx
 pop ebx
 mov [new], ebx
 pop ebx

 mov eax, 5
 mov ecx, 2
 mov edx, 0
 int 0x80
 mov [fd], eax

point:
 mov eax, 3
 mov ebx, [fd]
 mov ecx, char
 mov edx, 1
 int 0x80
 
 cmp eax, 0
 je exit

 mov esi, [orig]
 lodsb
 mov bl, [char]
 cmp al, bl
 je equal

 jmp point

exit:
 mov eax, 6
 mov ebx, [fd]
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80

equal:
 mov eax, 19
 mov ebx, [fd]
 mov ecx, -1
 mov edx, 1
 int 0x80

 mov esi, [new]
 lodsb
 mov [char], al
 
 mov eax, 4
 mov ebx, [fd]
 mov ecx, char
 mov edx, 1
 int 0x80

 jmp point
