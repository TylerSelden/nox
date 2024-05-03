printf:
  pusha

printf_start:
  mov al, [bx]
  cmp al, 0
  je printf_done

  mov ah, 0x0e
  int 0x10

  add bx, 0x01
  jmp printf_start


printf_done:
  mov ah, 0x0e
  mov al, 0x0a ; newline
  int 0x10
  mov al, 0x0d ; carriage ret
  int 0x10

  popa
  ret
