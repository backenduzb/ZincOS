global get_key

get_key:
.wait:
    in al, 0x60   
    test al, al
    jz .wait
    ret
