global shutdown

shutdown:
    mov dx, 0xB004
    mov ax, 0x2000
    out dx, ax
    
    mov dx, 0x0604
    out dx, ax
    
    mov dx, 0x4004
    out dx, ax
    hlt
