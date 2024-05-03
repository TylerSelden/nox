printstr:
  pusha

printstr_start:
  mov al, [bx]
  cmp al, 0
  je printstr_done

  mov ah, 0x0e
  int 0x10

  add bx, 0x01
  jmp printstr_start


printstr_done:
  popa
  ret
