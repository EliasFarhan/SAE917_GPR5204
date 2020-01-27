global classroom_avg
classroom_avg:
    mov eax, 0 ; set result to 0
    mov rcx, 0 ; set i to 0
    cmp rsi, 0 ; if compare length to 0
    jne _inner_loop ; continue to inner loop
    ret ; return if length is 0
_inner_loop:
    add eax, dword [rdi+4*rcx] ; result += v[i]
    inc rcx ; i++
    cmp rcx, rsi ; if i != length
    jne _inner_loop ; continue the inner loop
    cdqe ; result int -> long
    cqo ; rax -> rdx:rax
    idiv rsi ; result / length
    ret ; result is already in rax


dot_product_vec3: ;(vec3, vec3):
    mov     rax, rdx ; rdx = v2.xy -> rax
    shr     rax, 32 ; eax = v2.y
    imul    edx, edi ; edx = v2.x = v2.x * v1.x
    shr     rdi, 32 ; edi = v1.y
    imul    edi, eax ; v1.y = v1.y * v2.y
    add     edx, edi ; edx = v1.xy * v2.xy
    imul    esi, ecx ; esi = v1.z * v2.z
    lea     eax, [rdx+rsi] ; eax = dot_product
    ret