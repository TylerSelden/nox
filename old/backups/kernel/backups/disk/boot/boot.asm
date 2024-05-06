[org 0x7c00]
; set the stack
mov bp, 0x8000
mov sp, bp

mov bx, 0x9000 ; where to load the disk into
mov dh, 1      ; read `dh` sectors

call loadsectors ; there ain't no way this is gonna work :D

mov dx, [0x9000]
call printhex


; inf loop

jmp $

; inclusions

%include "./lib/printstr.asm"
%include "./lib/printf.asm"
%include "./lib/printhex.asm"
%include "./lib/disk.asm"

; filler and magic number

times 510-($-$$) db 0
dw 0xaa55

times 256 dw 0xbeef
