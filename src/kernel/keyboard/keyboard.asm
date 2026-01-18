bits 32
global keyboard_interrupt
global keyboard_scancode
global default_interrupt

section .bss
keyboard_scancode: resb 1


section .text

default_interrupt:
    pusha
    popa
    iretd

keyboard_interrupt:
    pusha
    in al, 0x60
    test al, 0x80
    jnz .done
    mov [keyboard_scancode], al
.done:
    mov al, 0x20
    out 0x20, al
    popa
    iretd
