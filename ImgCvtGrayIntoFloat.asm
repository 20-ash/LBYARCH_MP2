section .data
    align 16
    const dd 255.0

section .text
global imgCvtGrayIntoFloat

    imgCvtGrayIntoFloat:
        push rbp
        mov rbp, rsp

        mov rax, rcx
        mov r10, rdx
        imul r9d, r8d
        cmp r9d, 0
        jle .done

        movss xmm1, [rel const]
        xor r11d, r11d
    
    .loop:
        mov edx, [rax + r11*4]
        cvtsi2ss xmm0, edx
        divss xmm0, xmm1
        movss [r10 + r11*4], xmm0

        inc r11d
        cmp r11d, r9d
        jl .loop
    
    .done:
        pop rbp
        ret

