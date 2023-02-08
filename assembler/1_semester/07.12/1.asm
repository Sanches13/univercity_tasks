section .bss
 res resb 10
 str1 resb 5
 str2 resb 5
section .text:
 global _start
_start:
 mov eax, 3
 mov ebx, 0
 mov ecx, str1
 mov edx, 6
 int 0x80

 mov eax, 3
 mov ebx, 0 
 mov ecx, str2
 mov edx, 5
 int 0x80
 
 cld
 lea esi, [str1]
 lea edi, [res]

 mov ecx, 5
point1:
 movsb
 loop point1

 cld 
 lea esi, [str2]
 lea edi, [res+5]

 mov ecx, 5
point2:
 movsb
 loop point2

 mov eax, 4
 mov ebx, 1
 mov ecx, res
 mov edx, 10
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
