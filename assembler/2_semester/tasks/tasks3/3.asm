section .bss
 msg resb 100
 char resb 1
 len resb 4
 fd resb 4
 ind resb 4

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

 mov eax, 3
 mov ebx, 0
 mov ecx, msg
 mov edx, 100
 int 0x80
 sub eax, 1
 mov [len], eax

 mov eax, 0 
 mov [ind], eax

 mov eax, 19 
 mov ebx, [fd]
 mov ecx, 0
 mov edx, 2
 int 0x80

point:
 mov esi, msg
 mov eax, [len]
 add esi, eax
 lodsb
 mov [char], al
 
 mov eax, 4 
 mov ebx, [fd]
 mov ecx, char
 mov edx, 1
 int 0x80

 mov eax, [len]
 dec eax
 cmp eax, 0
 jnge exit
 mov [len], eax
 jmp point

exit:
 mov eax, 6
 mov ebx, [fd]
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
