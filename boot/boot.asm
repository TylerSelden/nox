MBALIGN   equ 1 << 0
MEMINFO   equ 1 << 1
MBFLAGS   equ MBALIGN | MEMINFO
MAGIC     equ 0x1BADB002
CHECKSUM  equ -(MAGIC + MBFLAGS)

section .multiboot
align 4
  dd MAGIC
  dd MBFLAGS
  dd CHECKSUM



section .bss
stack_bottom:
resb 16384 ; 8 KiB
stack_top:




section .text
global _start:function (_start.end - _start)
_start:
  mov esp, stack_top

  extern kernel_main
  call kernel_main



  cli
.hang:
  hlt
  jmp .hang
.end:
