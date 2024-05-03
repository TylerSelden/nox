[bits 16]
switch_to_pm:
  cli ; disable interrupts :(
  lgdt [gdt_descriptor]
  
  mov eax, cr0
  or eax, 0x1 ; set 32-bit mode bit in cr0
  mov cr0, eax

  jmp CODE_SEG:init_pm

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

  call BEGIN_PM
