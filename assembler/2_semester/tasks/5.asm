;Содержимое двух файлов переместить в третий
section .bss
 fname_1 resb 4
 fname_2 resb 4
 fd_1 resb 4
 fd_2 resb 4
 fd_3 resb 4
 sym resb 1

section .text:
 global _start
_start:

 pop ebx
 pop ebx
 pop ebx
 mov [fname_1], ebx
 pop ebx
 mov [fname_2], ebx
 pop ebx
  
 mov eax, 8
 mov ecx, 420
 int 0x80
 mov [fd_3], eax

 mov eax, 5
 mov ebx, [fname_1]
 mov ecx, 0
 mov edx, 0
 int 0x80
 mov [fd_1], eax

point_1:
 mov eax, 3
 mov ebx, [fd_1]
 mov ecx, sym
 mov edx, 1
 int 0x80

 cmp eax, 0
 je point_2

 mov eax, 4
 mov ebx, [fd_3]
 mov ecx, sym
 mov edx, 1
 int 0x80

 jmp point_1

point_2:
 mov eax, 6
 mov ebx, [fd_1]
 int 0x80

 mov eax, 5
 mov ebx, [fname_2]
 mov ecx, 0
 mov edx, 0
 int 0x80
 mov [fd_2], eax

point_3:
 mov eax, 3
 mov ebx, [fd_2]
 mov ecx, sym
 mov edx, 1
 int 0x80

 cmp eax, 0
 je exit

 mov eax, 4
 mov ebx, [fd_3]
 mov ecx, sym
 mov edx, 1
 int 0x80

 jmp point_3

exit:
 mov eax, 6 
 mov ebx, [fd_2]
 int 0x80

 mov eax, 6
 mov ebx, [fd_3] 
 int 0x80

 mov eax, 1
 mov ebx, 0 
 int 0x80
