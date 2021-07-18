	global main
	extern malloc, puts, exit

rot13:
        push    r13
        push    r12
        push    rbp
        push    rbx
        sub     rsp, 8
        cmp     BYTE [rdi], 0
        je      .malloc_str
        mov     rbx, rdi
        mov     eax, 1
.strlen:
        mov     edi, eax
        lea     ebp, [rax-1]
        mov     r13d, eax
        mov     r12, rax
        add     rax, 1
        cmp     BYTE [rbx-1+rax], 0
        jne     .strlen
        add     edi, 1
        movsx   rdi, edi
        call    malloc WRT ..plt
        mov     r8, rax
        test    r13d, r13d
        jle     .rot13_return
        mov     rsi, rax
        mov     rdx, rbx
        jmp     .if_lower
.set_sub_char:
        mov     BYTE [rsi], dil
.for_loop:
        add     rdx, 1
        add     rsi, 1
        mov     eax, edx
        sub     eax, ebx
        sub     eax, 1
        cmp     eax, ebp
        jge     .rot13_return
.if_lower:
        movzx   eax, BYTE [rdx]
        movsx   ecx, al
        lea     edi, [rcx-97]
        cmp     edi, 25
        ja      .if_upper
        lea     edi, [rax-13]
        movsx   ecx, dil
        sub     ecx, 97
        cmp     ecx, 25
        jbe     .set_sub_char
        add     eax, 13
        mov     BYTE [rsi], al
        jmp     .for_loop
.if_upper:
        sub     ecx, 65
        cmp     ecx, 25
        ja      .set_add_char_and_loop
        lea     edi, [rax-13]
        movsx   ecx, dil
        sub     ecx, 65
        cmp     ecx, 25
        jbe     .set_sub_char_and_loop
        add     eax, 13
        mov     BYTE [rsi], al
        jmp     .for_loop
.set_sub_char_and_loop:
        mov     BYTE [rsi], dil
        jmp     .for_loop
.set_add_char_and_loop:
        mov     BYTE [rsi], al
        jmp     .for_loop
.malloc_str:
        mov     edi, 1
        call    malloc WRT ..plt
        mov     r8, rax
        mov     r12d, 0
.rot13_return:
        mov     BYTE [r8+r12], 0
        mov     rax, r8
        add     rsp, 8
        pop     rbx
        pop     rbp
        pop     r12
        pop     r13
        ret
main:
        sub     rsp, 8                  ; align stack
        cmp     edi, 1                  ; rdi/edi = 1st arg, rsi = 2nd arg
        jle     .exit
        mov     rdi, QWORD [rsi+8]      ; rdi <- argv[1], qword = uint64
        call    rot13
        mov     rdi, rax
        call    puts WRT ..plt
        mov     eax, 0
        add     rsp, 8                  ; restore stack
        ret
.exit:
        mov     edi, 1                  ; set exit code
        call    exit WRT ..plt
