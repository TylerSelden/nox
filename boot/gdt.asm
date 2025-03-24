gdt_start:
  ; null entry
  dd 0
  dd 0

gdt_code:
  dw 0xffff    ; 00-15 limit
  dw 0         ; 16-31 base
  db 0         ; 32-39 base
  db 10011010b ; 40-47 access
  db 11001111b ; 48-55 limit (48-51) and flags (52-55)
  db 0         ; 56-63 base

gdt_data:
  dw 0xffff    ; 00-15 limit
  dw 0         ; 16-31 base
  db 0         ; 32-39 base
  db 10010010b ; 40-47 access
  db 11001111b ; 48-55 limit (48-51) and flags (52-55)
  db 0         ; 56-63 base

gdt_end:


gdt:
  dw gdt_end - gdt_start - 1 ; length
  dd gdt_start               ; address

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
