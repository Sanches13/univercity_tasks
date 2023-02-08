%macro print 1
 mov eax, 4
 mov ebx, 1 
 mov ecx, %1
 mov edx, 1
 int 0x80
%endmacro
