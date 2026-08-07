section .data
    align 16
    const dd 255.0

section .text
    global imgCvtGrayInttoFloat

    imgCvtGrayInttoFloat:
        push rbp
        mov rbp, rsp        ; setup pointers

        mov rax, rcx        ; rax = int
        mov r10, rdx        ; r10 = float
        imul r9d, r8d       ; r9d = height * width
        cmp r9d, 0          ; checks if total pixel count is 0
        jle .done           ; if yes ^ skips the loop, nothing to convert

        movss xmm1, [rel const]     ; load constant 255.0 to reuse in each loop iteration
        xor r11d, r11d              ; loop counter

    .loop:
        movzx edx, byte [rax + r11]     ; edx = int pixels
        cvtsi2ss xmm0, edx              ; xmm0 = converts int into float
        divss xmm0, xmm1                ; xmm0 = xmm0 / 255.0
        movss [r10 + r11*4], xmm0       ; stores result and moves it by 4 bytes for the next one

        inc r11d                        ; increase loop counter
        cmp r11d, r9d                   ; compare loop count to number of pixels
        jl .loop                        ; loops back as needed

    .done:
        pop rbp
        ret
