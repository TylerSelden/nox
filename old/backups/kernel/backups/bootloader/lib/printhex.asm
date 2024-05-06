printhex:
  pusha
  mov cx, 0

printhex_loop:
  cmp cx, 4
  je printhex_done
  
  mov ax, dx
  and ax, 0x000f
  add al, 0x30
  cmp al, 0x39
  jle printhex_2
  add al, 7

printhex_2:
  mov bx, PRINTHEX_MEM + 5 ; base + length
  sub bx, cx
  mov [bx], al
  ror dx, 4

  add cx, 1
  jmp printhex_loop

printhex_done:
  mov bx, PRINTHEX_MEM
  call printstr

  popa
  ret

PRINTHEX_MEM:
  db '0x0000', 0
