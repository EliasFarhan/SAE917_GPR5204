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
    global swap_vec4
swap_vec4:
    movups xmm0, [rsi]
    movups xmm1, [rdi]
    movups [rsi], xmm1
    movups [rdi], xmm0
    ret

