global avg
avg: ;@avg(int const*, unsigned long)
        test    rsi, rsi ; check if rsi(length) is 0
        je      .LBB0_1
        xor     ecx, ecx ;set ecx to 0
        xor     eax, eax ; set eax to zero
.LBB0_3:                                ; =>This Inner Loop Header: Depth=1
        movsxd  rdx, dword [rdi + 4*rcx]
        cdqe
        add     rax, rdx
        add     rcx, 1
        cmp     rsi, rcx
        jne     .LBB0_3
        xor     edx, edx
        div     rsi
        ret
.LBB0_1:
        xor     eax, eax
        ret
