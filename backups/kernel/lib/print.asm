[bits 16]

; printstr
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


; printf
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


; printhex

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


; print_pm
[bits 32]

VIDMEM equ 0xb8000
COLOR equ 0x0f

print_pm:
  pusha
  mov edx, VIDMEM

print_pm_loop:
  mov al, [ebx] ; [ebx] is the address of our char
  mov ah, COLOR

  cmp al, 0
  je print_pm_done

  mov [edx], ax
  add ebx, 1 ; next char
  add edx, 2 ; next vidmem addr

  jmp print_pm_loop

print_pm_done:
  popa
  ret
