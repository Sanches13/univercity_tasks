section .data
 null db '0', 0       ;помещаем символ '0' в null
 len1 equ $ - null    ;длина null
 one db '1', 0        ;помещаем символ '1' в one
 len2 equ $ - one     ;длина one

section .text:
 global _start
_start:
 mov edx, 0b10101010110000110111001011100010   ;помещаем в edx случайную последовательность битов
 mov eax, 0      ;помещаем в eax число 0
 mov ecx, 32     ;помещаем в ecx число 32 - количество битов регистра edx
point:
 push ecx      ;кладём в стек значение регистра ecx      
 push eax      ;кладём в стек значение регистра eax 
 push edx      ;кладём в стек значение регистра edx 
 bt edx, eax   ;с помощью bt определяем значение бита под номером eax в регистре edx
 jc point2     ;если установлен carry flag, то определённый выше бит равен единице, поэтому перемещаемся к метке point2
 
 mov eax, 4    ;если carry flag равен нулю, то выводим на экран символ '0'
 mov ebx, 1
 mov ecx, null
 mov edx, len1
 int 0x80
 jmp point3    ;перемещаемся к метке point3

point2:       ;если carry flag не равен нулю, то выводим на экран символ '1'
 mov eax, 4
 mov ebx, 1
 mov ecx, one
 mov edx, len2
 int 0x80
 
point3: 
 pop edx      ;достаём из стека значение edx
 pop eax      ;достаём из стека значение eax
 pop ecx      ;достаём из стека значение ecx
 inc eax      ;увеличиваем значение регистра eax, то есть перемещаемся к следующему биту edx
 loop point   ;перемещаемся к метке point

 mov eax, 1    ;завершаем программу
 mov ebx, 0
 int 0x80 
