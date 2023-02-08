section .bss
 x resb 4    ;выделяем 4 байта памяти для переменной х
 sym resb 4    ;выделяем 4 байта памяти для переменной sym

section .data
 space db ' '    ;строка, содержащая пробел
 len equ $ - space    ;длина предыдущей строки

section .text:
 global _start
_start:

 mov byte [sym], 'A'    ;заносим в переменную sym символ 'A'
 
 mov eax, 3    ;получаем значение переменной х
 mov ebx, 0
 mov ecx, x
 mov edx, 1
 int 0x80

 mov eax, [x]    ;заносим в регистр eax значение переменной х
 sub eax, 0x30    ;преобразуем символ в число

 cmp eax, 0    ;сравниваем значение полученного числа с нулём
 je point1      ;если значение равно нулю, переходим на метку point1 и завершаем программу
 
point2:        ;создаём метку point2
 push eax      ;кладём значение eax в стек

 mov eax, 4    ;выводим на экран значение переменной sym, то есть в первой итерации будет выведен символ 'A'
 mov ebx, 1
 mov ecx, sym
 mov edx, 1
 int 0x80

 mov eax, 4    ;выводим символ пробела
 mov ebx, 1
 mov ecx, space
 mov edx, len
 int 0x80

 mov eax, [sym]    ;помещаем в регистр eax значение переменной sym
 inc eax           ;увеличиваем это значение на 1
 mov [sym], eax    ;помещаем полученное значение обратно в sym
   
 pop eax        ;достаём из стека значение eax
 dec eax        ;уменьшаем значение eax на 1
 cmp eax, 0     ;сравниваем полученное значение с 0
 jnz point2     ;если не равно 0, то перемещаемся обратно к метке point2
 
point1:
 mov eax, 1
 mov ebx, 0
 int 0x80

 
