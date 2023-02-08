section .bss
 x resb 4    ;выделяем 4 байта памяти для первого числа
 y resb 4    ;выделяем 4 байта памяти для второго числа

section .text:
 global _start
_start:
 mov eax, 3    ;получаем символ первого числа, как в программе num1.asm
 mov ebx, 0
 mov ecx, x
 mov edx, 4
 int 0x80

 mov eax, 3    ;получаем символ второго числа
 mov ebx, 0
 mov ecx, y
 mov edx, 4
 int 0x80

 mov ah, [x]    ;заносим в регистр ah значение переменной х
 sub ah, 0x30   ;преобразуем символ в число
 mov al, [y]    ;заносим в регистр al значение переменной у
 sub al, 0x30   ;преобразуем символ в число
 add ah, al     ;складываем значения регистров ah и al, сумма помещается в ah 
 add ah, 0x30   ;преобразуем число обратно в строку
 mov [x], ah    ;помещаем в переменную х полученное значение регистра ah
 
 mov eax, 4     ;выводим на экран значение х, как в программе num1.asm
 mov ebx, 1
 mov ecx, x
 mov edx, 4
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
