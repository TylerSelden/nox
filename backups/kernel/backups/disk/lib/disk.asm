loadsectors:
  pusha

  push dx

  mov ah, 0x02 ; read
  mov al, dh   ; # of sectors to read
  
  mov cl, 0x02 ; starting at sector 2
  mov ch, 0x00 ; cylinder 0
  mov dh, 0x00 ; head 0

  int 0x13 ; the actual interrupt
  jc loadsectors_disk_err

  pop dx
  cmp al, dh ; BIOS sets al to the number of sectors read
  jne loadsectors_sectors_err

  popa
  ret

loadsectors_disk_err:
  mov bx, DISK_ERR
  call printf

  mov dh, ah ; ah = error code, dl = disk drive where the err occurred
  call printhex

  jmp loadsectors_inf_loop

loadsectors_sectors_err:
  mov bx, SECTORS_ERR
  call printf

loadsectors_inf_loop:
  jmp $

DISK_ERR: db "Disk read error: ", 0
SECTORS_ERR: db "Incorrect number of sectors read :/", 0
