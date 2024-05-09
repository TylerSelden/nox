; Multiboot header
MBFLAGS   equ 0b000000111
MAGIC     equ 0x1BADB002
CHECKSUM  equ -(MAGIC + MBFLAGS)

section .multiboot
align 4
  dd MAGIC
  dd MBFLAGS
  dd CHECKSUM
 
  ; dummy data, these params are not used
  times 5 dd 0

  ; video output params
  dd 1 ; Text mode
  dd 80 ; Width
  dd 25 ; Height
  dd 0 ; Depth, 0 in text mode



section .bss
stack_bottom:
resb 16384 ; 8 KiB
stack_top:

section .gdt
%include "./boot/gdt.asm"


section .text
global _start:function (_start.end - _start)
_start:
  mov esp, stack_top


  ; initialize GDT
  lgdt [gdt]
  
  mov ax, DATA_SEG
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ; set CS
  jmp CODE_SEG:.call_main


.call_main:
  extern main
  call main

  cli

.hang:
  hlt
  jmp .hang
.end:
