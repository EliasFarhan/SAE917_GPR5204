global avg
avg:;(int const*, unsigned long):
        mov     eax, 0 ; eax is the return value
        test    rsi, rsi ; check if rsi (second argument) is 0
        je      .L1 ; we jump if it's true
        mov     rcx, rdi ; move rdi (pointer) to rcx
        lea     rdx, [rdi+rsi*4] ; load the address of the end to rdx
.L3:
        add     eax, DWORD [rcx] ; result += v[i]
        add     rcx, 4 ; i++
        cmp     rcx, rdx ; compare rcx address is end of array
        jne     .L3 ; if false we jump back
        cdqe ; we promote the eax register for the division to rax, RAX ← sign-extend of EAX.
        mov     rdx, 0
        div     rsi ; divide rdx:rax with rsi, length is always positive so div instead of idiv
.L1:
        ret ; rax:eax is return

global dot_vec3
dot_vec3:;(vec3, vec3):                          # @dot(vec3, vec3)
        mov     rax, rdx ; rdx contains x2 and y2
        imul    eax, edi ; result = x1 * x2
        shr     rdi, 32 ; x1 = y1
        shr     rdx, 32 ; x2 = y2
        imul    edx, edi ; y2 = y1 * y2
        imul    esi, ecx ; z1 = z1 * z2
        add     eax, esi ; result += z1 * z2
        add     eax, edx ; result += y1 * y2
        ret
global count_positive
count_positive:
    mov     rax, 0 ; eax is the return value
    test    rsi, rsi ; check if rsi (second argument) is 0
    je      end_count ; we jump if it's true
    mov     rcx, rdi ; move rdi (pointer) to rcx
    lea     rdx, [rdi+rsi*4] ; load the address of the end to rdx
    xorps xmm1, xmm1
count_loop:
    movss xmm0, [rdx]
    ucomiss xmm0, xmm1
    jbe end_count_loop
    inc rax
end_count_loop:
    add     rcx, 4 ; i++
    cmp     rcx, rdx ; compare rcx address is end of array
    jne     count_loop ; if false we jump back
end_count:
    ret

global count_positive_o1
count_positive_o1:;(float const*, unsigned long):
        test    rsi, rsi
        je      .countL4
        mov     rdx, rdi
        lea     rsi, [rdi+rsi*4]
        mov     eax, 0
        pxor    xmm1, xmm1
.countL3:
        movss   xmm0, DWORD [rdx]
        comiss  xmm0, xmm1
        seta    cl
        movzx   ecx, cl
        add     rax, rcx
        add     rdx, 4
        cmp     rdx, rsi
        jne     .countL3
        ret
.countL4:
        mov     rax, rsi
        ret