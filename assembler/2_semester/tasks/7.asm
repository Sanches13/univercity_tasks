;Уничтожение знаков препинания и пробелов, перевод прописных букв в строчные
 section .bss
 fd_1 resb 4
 fd_2 resb 4
 fname_1 resb 4
 char resb 1

section .text
 global _start
_start:

 pop ebx
 pop ebx
 pop ebx
 mov [fname_1], ebx
 pop ebx

 mov eax, 8
 mov ecx, 420
 int 0x80
 mov [fd_2], eax

 mov eax, 5
 mov ebx, [fname_1]
 mov ecx, 0
 mov edx, 0
 int 0x80
 mov [fd_1], eax

point:
 mov eax, 3
 mov ebx, [fd_1]
 mov ecx, char
 mov edx, 1
 int 0x80

 cmp eax, 0
 je exit

 mov al, [char]
 cmp al, 0x41
 jl num_point

 cmp al, 0x5B
 jl up_point 

 cmp al, 0x61
 jl point
 
 cmp al, 0x7A
 jg point

 mov eax, 4
 mov ebx, [fd_2]
 mov ecx, char
 mov edx, 1
 int 0x80

 jmp point
  
exit:
 mov eax, 6
 mov ebx, [fd_1]
 int 0x80

 mov eax, 6
 mov ebx, [fd_2]
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80

num_point:
 cmp al, 0x30
 jl point

 cmp al, 0x39
 jg point

 mov eax, 4
 mov ebx, [fd_2]
 mov ecx, char
 mov edx, 1
 int 0x80

 jmp point

up_point:
 add al, 0x20
 mov [char], al
 
 mov eax, 4
 mov ebx, [fd_2]
 mov ecx, char
 mov edx, 1
 int 0x80
 
 jmp point
