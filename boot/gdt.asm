gdt_start:
  ; null descriptor
  dd 0
  dd 0

gdt_code:
  dw 0xffff     ; 00-15 limit
  dw 0x0        ; 16-31 base 0
  db 0x0        ; 32-39 base 1
  db 10011010b  ; 40-47 access byte (flag)
  db 11001111b  ; 48-55 limit (48-51) and flags (52-55)
  db 0x0        ; 56-63 base 2

gdt_data:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0

gdt_end:

gdt:
  dw gdt_end - gdt_start - 1 ; length of gdt
  dd gdt_start               ; gdt start addr

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
