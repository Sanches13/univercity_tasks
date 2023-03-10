section .bss
 x resb 4    ;выделяем 4 байта памяти для переменной х
 y resb 4    ;выделяем 4 байта памяти для переменной у

section .data
 mess1 db 'Less than 50', 0xA    ;сообщение о том, что произведение меньше 50
 len1 equ $ - mess1    ;длина предыдущей строки
 
 mess2 db 'More than 50', 0xA    ;сообщение о том, что произведение больше 50
 len2 equ $ - mess2    ;длина предыдущей строки

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
 mov al, [y]    ;заносим в регистр al значение переменной y
 sub ah, 0x30    ;преобразуем символ в число
 sub al, 0x30    ;преобразуем символ в число
 add ah, al    ;складываем значение двух регистров, сумму помещаем в ah. Если сумма двух чисел меньше 15, то их произведение будет меньше 50, если больше или равно 15, то больше 50
  
 cmp ah, 15    ;сравниваем значение ah с 15, и если значение регистра больше, то переходим к метке point
 jg point      

 cmp ah, 15    ;сравниваем значение ah с 15, и если значение регистра равно 15, то переходим к метке point
 je point

 mov eax, 4    ;выводим сообщение о том, что произведение двух введённых чисел меньше 50
 mov ebx, 1
 mov ecx, mess1
 mov edx, len1
 int 0x80

 mov eax, 1    ;завершаем программу
 mov ebx, 0
 int 0x80

point:
 mov eax, 4    ;выводим сообщение о том, что произведение двух введённых чисел больше 50
 mov ebx, 1
 mov ecx, mess2
 mov edx, len2
 int 0x80

 mov eax, 1
 mov ebx, 0
 int 0x80
