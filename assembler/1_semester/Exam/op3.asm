;Вывод содержимого файла на экран
section .bss
 fd resb 4
 char resb 1

section .text:
 global _start
_start:

 pop ebx
 pop ebx
 pop ebx

 mov eax, 5
 mov ecx, 0
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

 mov eax, 4
 mov ebx, 1
 mov ecx, char
 mov edx, 1
 int 0x80

 jmp point

exit:
 mov eax, 6
 mov ebx, [fd]
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
