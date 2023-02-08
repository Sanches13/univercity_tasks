section .bss
 x resb 4    ;выделяем 4 байта памяти для переменной х
 y resb 4    ;выделяем 4 байта памяти для переменной y

section .data
 num db '1'    ;строка, содержащая символ 1
 len equ $ - num    ;длина предыдущей строки
 
section .text:
 global _start
_start:

 mov eax, 3    ;получаем значение переменной х
 mov ebx, 0
 mov ecx, x
 mov edx, 4
 int 0x80

 mov eax, 3    ;получаем значение переменной у
 mov ebx, 0
 mov ecx, y
 mov edx, 4
 int 0x80

 mov ah, [x]    ;заносим в регистр ah значение переменной х
 sub ah, 0x30    ;преобразуем символ в число
 mov al, [y]    ;заносим в регистр al значение переменной у
 sub al, 0x30     ;преобразуем символ в число
 add ah, al     ;складываем значения регистров ah и al, сумма помещается в ah
 
 cmp ah, 9      ;сравниваем значение ah с 9
 jl point       ;если значение меньше 9, то перемещаемся к метке point
 
 mov [x], ah    ;помещаем в переменную х значение регистра ah
 mov eax, 4     ;выводим символ 1
 mov ebx, 1
 mov ecx, num
 mov edx, len
 int 0x80

 mov ah, [x]    ;помещаем в регистр ah значение переменной х
 sub ah, 9      ;отнимаем от значения ah число 9
 sub ah, 1      ;отнимаем от значения ah число 1
 add ah, 0x30   ;преобразуем число в строку
 mov [x], ah    ;помещаем значение регистра обратно в переменную х

 mov eax, 4    ;выводим значение переменной х
 mov ebx, 1
 mov ecx, x
 mov edx, 2
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
 
point:
 add ah, 0x30   ;преобразуем число в строку 
 mov [x], ah    ;помещаем значение регистра обратно в переменную х
 mov eax, 4     ;выводим значение переменной х
 mov ebx, 1
 mov ecx, x
 mov edx, 4
 int 0x80
 
 mov eax, 1
 mov ebx, 0
 int 0x80 
 
