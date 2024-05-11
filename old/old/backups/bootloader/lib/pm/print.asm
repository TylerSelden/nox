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
