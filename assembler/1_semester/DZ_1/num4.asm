section .bss
 x resb 2    ;выделяем 2 байта памяти для числа

section .text: 
 global _start
_start:
 
 mov eax, 3    ;получаем символ первого числа
 mov ebx, 0
 mov ecx, x
 mov edx, 2
 int 0x80

 mov ax, [x]    ;заносим в регистр aх значение переменной х
 sub ax, 0x30   ;преобразуем символ в число
 mov bx, 3      ;заносим в регистр bх число 3
 div bx         ;делим значение регистра ax на значение регистра bx, результат помещается в регистр al
 add al, 0x30   ;преобразуем число обратно в строку
 mov [x], al    ;помещаем в переменную х полученное значение регистра al

 mov eax, 4    ;выводим на экран значение х
 mov ebx, 1
 mov ecx, x
 mov edx, 1
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
