global bad_int2long
bad_int2long:
    mov eax, edi
    ret
global int2long
int2long:
    movsxd rax, edi
    ret
global bad_shiftr
bad_shiftr:
    mov eax, edi
    shr eax, 1
    ret
global shiftr
shiftr:
    mov eax, edi
    sar eax, 1
    ret

