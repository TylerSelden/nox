[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; Where the kernel gets loaded to
; Store dl, in case it gets overwritten
mov [BOOT_DRIVE], dl
; Set the stack
mov bp, 0x9000
mov sp, bp


; Success message
mov bx, MSG_16_BOOT
call printf

; Start booting into 32 bit prot mode
call load_kernel ; Load the kernel from disk
call switch_to_pm


; In the bootloader, this point should never be reached
; It's put there to prevent inclusions from running
jmp $

; Inclusions
%include "./lib/print.asm"
%include "./lib/disk.asm"
%include "./lib/pm/gdt.asm"
%include "./lib/pm/switchtopm.asm"
[bits 16]


load_kernel:
  mov bx, MSG_LOAD_KERNEL
  call printf

  mov bx, KERNEL_OFFSET
  mov dh, 5 ; Number of sectors to load
  mov dl, [BOOT_DRIVE]
  call loadsectors
  ret

; 32 bit prot mode
[bits 32]
BEGIN_PM:
  mov ebx, MSG_32_BOOT
  call print_pm
  call KERNEL_OFFSET ; Go to kernel entrypoint
  ret


; Data
BOOT_DRIVE db 0 ; Reserved mem address

MSG_16_BOOT db "Successfully booted into 16-bit real mode.", 0
MSG_LOAD_KERNEL db "Loading kernel from disk...", 0
MSG_32_BOOT db "Successfully booted into 32-bit protected mode.", 0


; filler and magic number
times 510-($-$$) db 0
dw 0xaa55
