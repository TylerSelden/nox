; Multiboot header
FLAGS    equ 0b00000111
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS) ; checksum + (flags + magic) should be 0

section .multiboot
align 4
  dd MAGIC
  dd FLAGS
  dd CHECKSUM

  ; padding to make it 16 bytes long
  times 5 dd 0

  ; video output flags
  dd 1  ; type (1 = text mode)
  dd 80 ; width
  dd 25 ; height
  dd 0  ; depth, always 0 in text mode


section .bss
stack_bottom:
resb 16384 ; 16 KiB stack
stack_top:

section .gdt
%include "./boot/gdt.asm"


%include "./boot/idt.asm"

section .text
global _start:function (_start.end - _start)
_start:
  mov esp, stack_top

  ; load GDT
  lgdt [gdt]

  ; set up segments
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

  sti
  call main

  ; halt
  ;cli
.hang:
  hlt
  jmp .hang
.end:
