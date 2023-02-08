section .bss
 x resb 4    ;выделяем 4 байта памяти для переменной х
 
section .data
 mess db 'number', 0xA    ;строка, которая будет выведена в том случае, если пользователь ввёл число
 len equ $ - mess    ;длина строки

section .text:
 global _start:
_start:
 
 mov eax, 3    ;получаем значение переменной х
 mov ebx, 0
 mov ecx, x
 mov edx, 4
 int 0x80

 mov ah, [x]    ;заносим в регистр ah значение переменной х
 sub ah, 0x30    ;преобразуем символ в число
 mov al, 0      ;заносим в регистр al число 0
 cmp ah, al     ;сравниваем значения двух регистров
 jl point       ;если значение регистра ah меньше нуля, то переходим к метке point

 mov al, 9      ;заносим в регистр al число 9
 cmp ah, al    ;если значение регистра ah больше 9, то переходим к метке point
 jg point

 mov eax, 4     ;выводим сообщение о том, что введённый символ является числом
 mov ebx, 1
 mov ecx, mess
 mov edx, len
 int 0x80

 mov eax, 1    ;завершаем программу
 mov ebx, 0
 int 0x80

point:
 mov eax, 1    ;завершаем программу
 mov ebx, 0
 int 0x80
