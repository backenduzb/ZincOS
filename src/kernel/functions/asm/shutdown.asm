global shutdown

shutdown:
    ; Bochs/QEMU ACPI poweroff
    mov dx, 0xB004
    mov ax, 0x2000
    out dx, ax
    ; QEMU (modern)
    mov dx, 0x0604
    out dx, ax
    ; QEMU (legacy)
    mov dx, 0x4004
    out dx, ax
    hlt
