
section .data
pi equ 3.1419
section .text
global pythagoras
pythagoras:
    mulss xmm0, xmm0
    mulss xmm1, xmm1
    addss xmm0, xmm1
    sqrtss xmm0, xmm0
    ret
