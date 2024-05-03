[org 0x7c00]
; set the stack
mov bp, 0x9000
mov sp, bp

; enter 32-bit prot mode
cli ; disable interrupts :(
lgdt [gdt_descriptor]

mov eax, cr0
or eax, 0x1 ; set 32-bit mode bit in cr0
mov cr0, eax

jmp CODE_SEG:init_pm


; gdt stuff
gdt_start:
  dd 0x0
  dd 0x0
gdt_code:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10011010b ; flags (8 bits)
  db 11001111b ; flags (4 bits) + segment length
  db 0x0
gdt_data:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0
gdt_end:
gdt_descriptor:
  dw gdt_end - gdt_start - 1 ; size (16 bit), always 1 less of true size
  dd gdt_start ; address (32 bit)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start




; 32 bit mode stuff
[bits 32]
init_pm:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000 ; update the stack
  mov esp, ebp

  mov word [0xB8000], 0x0f69
  jmp $




; filler and magic number
times 510-($-$$) db 0
dw 0xaa55
