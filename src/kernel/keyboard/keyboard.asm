global get_key

get_key:
.wait:
    in al, 0x64        ; read keyboard status
    test al, 1         ; check OBF (output buffer full)
    jz .wait

    in al, 0x60        ; read scancode
    movzx eax, al      ; return value in EAX
    ret
