section .bss
 x resb 4    ;выделяем 4 байта памяти для переменной х

section .text:
 global _start
_start:

 mov eax, 3    ;получаем значение переменной х
 mov ebx, 0
 mov ecx, x
 mov edx, 4
 int 0x80

 mov eax, [x]    ;заносим в регистр eax значение переменной х
 sub eax, 0x30   ;преобразуем символ в число
 mov ebx, 1      ;заносим в регистр ebx число 1
 add eax, ebx    ;складываем значения регистров eax и ebx, сумма помещается в eax
 add eax, 0x30   ;преобразуем число обратно в строку
 mov [x], eax    ;помещаем в переменную х полученное значение регистра eax

 mov eax, 4      ;выводим на экран значение х
 mov ebx, 1
 mov ecx, x
 mov edx, 4
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
