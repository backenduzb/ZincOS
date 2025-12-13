global get_key

get_key:
.wait:
    in al, 0x64        
    test al, 1         
    jz .wait

    in al, 0x60       
    movzx eax, al      
    ret
