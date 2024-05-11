[org 0x7c00]
; set the stack
mov bp, 0x9000
mov sp, bp



mov bx, REAL_MODE_MSG
call printf

call switchtopm

; we won't actually get here
jmp $

; inclusions

;%include "./lib/printstr.asm"
%include "./lib/printf.asm"
;%include "./lib/printhex.asm"
;%include "./lib/disk.asm"
%include "./lib/pm/gdt.asm"
%include "./lib/pm/print.asm"
%include "./lib/pm/switchtopm.asm"



[bits 32]
BEGIN_PM:
  mov ebx, PROT_MODE_MSG
  call print_pm
  jmp $

REAL_MODE_MSG db "Startup in 16-bit real mode", 0
PROT_MODE_MSG db "Switched to 32-bit prot mode successfully :)", 0

; filler and magic number

times 510-($-$$) db 0
dw 0xaa55
