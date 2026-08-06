section .data
    align 16
    const dd 255.0

    ; to test the .asm file
    testVals:     dd 64, 89, 114, 84, 140, 166, 191, 84, 216, 242, 38, 84
    floatResults: times 12 dd 0
    fmt:          db "%.6f ", 0
    newline:      db 10, 0

section .text
    extern printf
    global main
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

    ; added main for testing, remove when .c file is available
    main:
        push rbp
        mov rbp, rsp
        push r12
        sub rsp, 40
        
        lea rcx, [testVals]
        lea rdx, [floatResults]
        mov r8d, 4
        mov r9d, 3
        call imgCvtGrayIntoFloat

        xor r12d, r12d

    .print_loop:
        cmp r12d, 12
        jge .print_done

        lea rcx, [fmt]
        cvtss2sd xmm1, [floatResults + r12*4]
        movq rdx, xmm1                          
        
        call printf

        inc r12d
        jmp .print_loop
    
    .print_done:
        lea rcx, [newline]
        call printf

        add rsp, 40
        pop r12
        pop rbp
        xor eax, eax
        ret