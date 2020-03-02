pxart::simd256::mt19937::operator()():
        movsx   rdx, DWORD PTR [rdi+2528]
        mov     rax, rdx
        cmp     rdx, 623
        ja      .L2
        lea     rdx, [rdi+rdx*4]
        add     eax, 8
.L3:
        vmovdqa ymm1, YMMWORD PTR [rdx]
        mov     DWORD PTR [rdi+2528], eax
        vpsrld  ymm0, ymm1, 11
        vpxor   ymm1, ymm1, ymm0
        vpslld  ymm0, ymm1, 7
        vpand   ymm0, ymm0, YMMWORD PTR .LC4[rip]
        vpxor   ymm1, ymm0, ymm1
        vpslld  ymm0, ymm1, 15
        vpand   ymm0, ymm0, YMMWORD PTR .LC5[rip]
        vpxor   ymm0, ymm0, ymm1
        vpsrld  ymm1, ymm0, 18
        vpxor   ymm0, ymm0, ymm1
        ret
.L2:
        vmovdqa ymm2, YMMWORD PTR .LC0[rip]
        vmovdqa ymm3, YMMWORD PTR .LC1[rip]
        lea     rax, [rdi+32]
        lea     rdx, [rdi+928]
        vpand   ymm0, ymm3, YMMWORD PTR [rdi]
        vpand   ymm1, ymm2, YMMWORD PTR [rdi+4]
        vmovdqa ymm4, YMMWORD PTR .LC2[rip]
        vmovdqa ymm5, YMMWORD PTR .LC3[rip]
        vpor    ymm1, ymm1, ymm0
        vpsrld  ymm6, ymm1, 1
        vpand   ymm0, ymm1, ymm4
        vpxor   xmm1, xmm1, xmm1
        vpcmpgtd        ymm0, ymm0, ymm1
        vpand   ymm0, ymm5, ymm0
        vpxor   ymm0, ymm0, YMMWORD PTR [rdi+1588]
        vpxor   ymm0, ymm0, ymm6
        vmovdqa ymm6, ymm1
        vmovdqa YMMWORD PTR [rdi], ymm0
        vmovdqa YMMWORD PTR [rdi+2496], ymm0
.L4:
        vpand   ymm1, ymm3, YMMWORD PTR [rax]
        vpand   ymm0, ymm2, YMMWORD PTR [rax+4]
        add     rax, 32
        vpor    ymm0, ymm0, ymm1
        vpsrld  ymm1, ymm0, 1
        vpand   ymm0, ymm0, ymm4
        vpxor   ymm1, ymm1, YMMWORD PTR [rax+1556]
        vpcmpgtd        ymm0, ymm0, ymm6
        vpand   ymm0, ymm5, ymm0
        vpxor   ymm0, ymm0, ymm1
        vmovdqa YMMWORD PTR [rax-32], ymm0
        cmp     rdx, rax
        jne     .L4
        lea     rax, [rdi+20]
        lea     rdx, [rdi+1588]
        vpxor   xmm6, xmm6, xmm6
.L5:
        vpand   ymm1, ymm3, YMMWORD PTR [rax+908]
        vpand   ymm0, ymm2, YMMWORD PTR [rax+912]
        add     rax, 32
        vpor    ymm0, ymm0, ymm1
        vpsrld  ymm1, ymm0, 1
        vpand   ymm0, ymm0, ymm4
        vpxor   ymm1, ymm1, YMMWORD PTR [rax-32]
        vpcmpgtd        ymm0, ymm0, ymm6
        vpand   ymm0, ymm5, ymm0
        vpxor   ymm0, ymm0, ymm1
        vmovdqa YMMWORD PTR [rax+876], ymm0
        cmp     rdx, rax
        jne     .L5
        mov     DWORD PTR [rdi+2528], 0
        mov     rdx, rdi
        mov     eax, 8
        jmp     .L3