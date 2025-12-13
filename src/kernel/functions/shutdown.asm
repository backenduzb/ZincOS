global shutdown

shutdown:
    mov dx, 0xB004
    mov ax, 0x2000
    out dx, ax
.halt:
    hlt
    jmp .halt
