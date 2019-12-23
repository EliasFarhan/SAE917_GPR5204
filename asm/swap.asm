    global swap_int
swap_int:
    mov eax, DWORD [rsi]
    mov edx, DWORD [rdi]
    mov DWORD [rsi], edx
    mov DWORD [rdi], eax
    ret
    global swap_float
swap_float:
    movss xmm0, DWORD [rsi]
    movss xmm1, DWORD [rdi]
    movss DWORD [rsi], xmm1
    movss DWORD [rdi], xmm0
    ret

