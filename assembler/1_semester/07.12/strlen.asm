global strlen

section .text:
 strlen:
  push ebp
  mov ebp, esp
  xor eax, eax
  mov esi, [ebp+8]
 .lp:
   cmp byte[esi], 0
   jz .quit
   inc esi
   inc eax
   jmp short .lp
 .quit:
  pop ebp
  ret
