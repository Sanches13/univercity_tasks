%include "mac.inc"
extern strlen

section .data
hello db "Hello", 0xA

section .bss
 char resb 1

section .text:
 global _start

_start:
 mov esi, hello
 push esi
 call strlen
 add eax, 0x30
 mov [char], eax
 print char
 mov eax, 1
 mov ebx, 0
