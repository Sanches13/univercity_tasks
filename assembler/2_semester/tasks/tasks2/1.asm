section .bss
 number resb 16
 ind resb 4
 char resb 4
 msg resb 4
 sum resb 4

section .text:
 global _start
_start:
 
 pop ebx
 pop ebx
 pop ebx
 mov [msg], ebx
 pop ebx
 mov [char], ebx

 mov eax, 0
 mov [sum], eax
 mov [ind], eax

point_1:
 mov esi, [msg]
 mov edi, [char]
 cmp edi, esi
 je point_2

 lodsb
 mov [msg], esi
 mov bl, al
 
 mov esi, [char]
 lodsb
 cmp al, bl
 je equal

 jmp point_1

equal:
 mov eax, [sum]
 inc eax
 mov [sum], eax
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
 cmp eax, 0
 jnge exit
 
 add esi, eax
 mov eax, [esi]
 mov [char], eax
 
 mov eax, 4
 mov ebx, 1
 mov ecx, char
 mov edx, 1
 int 0x80

 mov eax, [ind]
 dec eax
 mov [ind], eax
 
 jmp point_3

exit:
 mov eax, 1
 mov ebx, 0
 int 0x80
 
